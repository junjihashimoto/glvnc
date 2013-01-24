

#include <cassert>
#include <cstring>
#include <cstdio>
#include <cstdlib>
extern "C"{
#include <stdint.h>
#include <sys/types.h>
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#else
#include <winsock2.h>
#endif
}
//#include "glcommon.h"

#include "d3des.h"
#include "vnclib.h"

using namespace std;

#ifdef WIN32

#define read(socket,buf,len) recv(socket,(char*)buf,len,0)
#define write(socket,buf,len) send(socket,(char*)buf,len,0)
#define fsync(socket)
#define socket_init()   {WSADATA wsaData;WSAStartup(MAKEWORD(2,0), &wsaData);}
#define socket_finish() WSACleanup()

#else

#define closesocket(socket) ::close(socket)
#define socket_init()  
#define socket_finish()  

#endif

void
myread(SOCKET fd,unsigned char* buf,int n) {
  int total=0;
  while(total!=n){
    int v;
    total+=(v=read(fd,buf+total,n-total));
    assert(v>=0);
  }
}



#define read8(val) {				\
    unsigned char buf[1];			\
    int s=read(fd,buf,1);			\
    assert(s==1);				\
    val=buf[0];					\
  }

#define read16(val) {				\
    unsigned char buf[2];			\
    myread(fd,buf,2);                            \
    val=nto_uint16(buf);			\
  }

#define read32(val) {				\
    unsigned char buf[4];			\
    myread(fd,buf,4);                            \
    val=nto_uint32(buf);			\
  }



#define readn(len) {				\
    unsigned char buf[1024];			\
    int s=read(fd,buf,len);				\
    assert(s==len);				\
    buf[len]=0;					\
  }

#define write8(val) {				\
    unsigned char buf[1];			\
    buf[0]=val;					\
    int s=write(fd,buf,1);				\
    assert(s==1);				\
  }
#define write16(val) {				\
    unsigned char buf[2];			\
    buf[0]=val>>8;				\
    buf[1]=val&0xff;				\
    int s=write(fd,buf,2);				\
    assert(s==2);				\
  }
#define write32(val) {				\
    unsigned char buf[4];			\
    buf[0]=val>>24;				\
    buf[1]=(val>>16)&0xff;			\
    buf[2]=(val>>8)&0xff;			\
    buf[3]=val&0xff;				\
    int s=write(fd,buf,4);				\
    assert(s==4);				\
  }

void
vncEncryptBytes(unsigned char *bytes,const  char *passwd){
  unsigned char key[8];
  unsigned int i;

  for (i = 0; i < 8; i++) {
    if (i < strlen(passwd)) {
      key[i] = passwd[i];
    } else {
      key[i] = 0;
    }
  }

  deskey(key, EN0);

  for (i = 0; i < 16; i += 8) {
    des(bytes+i, bytes+i);
  }
}


uint32_t
nto_uint32(unsigned char* buf){
  return
    (buf[0] << 24)|
    (buf[1] << 16)|
    (buf[2] << 8)|
    (buf[3] );
}

uint16_t
nto_uint16(unsigned char* buf){
  return
    (buf[0] << 8)|
    (buf[1] );
}

THREAD_CALLBACK(run)(void* vncp){
  VNC_Client& vnc=*(VNC_Client*)vncp;
  while(vnc.exitp==0){
    int mtype;
    {
      unsigned char buf[1];
      int s=read(vnc.fd,(char*)buf,1);
      assert(s==1);
      mtype=buf[0];
    }
    //    printf("mtype:%d\n",mtype);
    switch(mtype){
    case 0:
      vnc.get_display();

      {
	Lock lock(vnc.img_mutex);
	vnc.img=vnc.img_buf;
      }

      {
	Lock lock(vnc.q_mutex);
	if(vnc.q_empty){
	  vnc.info_img_buf=vnc.img_buf;
	  vnc.q_empty=0;
	  vnc.q_cond.notify();
	}
      }
      vnc.set_display(1);

      if(vnc.wait_get_display_flag){
	Lock lock(vnc.get_display_mutex);
	vnc.get_display_cond.notify();
      }
      
      break;
    case 1:
      vnc.get_colormap();
      break;
    case 2:
      printf("bell\n");
      break;
    case 3:
      {
	string str=vnc.get_cuttext();
	//	printf("clip board:%s\n",str.c_str());
	if(vnc.get_cuttext_callback)
	  vnc.get_cuttext_callback((VNC_Client*)vncp,str);
      }
      break;
    default:
      assert(0);
      break;
    }
  }
  
}


THREAD_CALLBACK(run_info)(void* vncp){
  VNC_Client& vnc=*(VNC_Client*)vncp;
  BMP4b next_img;
  
  while(vnc.exitp==0){
    {
      Lock lock(vnc.q_mutex);
      while(vnc.q_empty){
	vnc.q_cond.wait(lock);
      }
    }
    next_img.init(vnc.info_img_buf.w,vnc.info_img_buf.h);
    if(vnc.img_filter_callback)
      vnc.img_filter_callback((VNC_Client*)vncp,vnc.info_img_buf,next_img);
    
    {
      Lock lock(vnc.img_mutex);
      vnc.info_img=next_img;
    }

    {
      Lock lock(vnc.q_mutex);
      vnc.q_empty=1;
    }
    
  }
}


VNC_Client::VNC_Client():thread(run),thread_info(run_info){
  get_cuttext_callback=NULL;
  img_filter_callback=NULL;
  wait_get_display_flag=0;
}

int
VNC_Client::init(const std::string& server,int port,const std::string& pass){
  int r;
  exitp=0;
  socket_init();
  
  fd=socket(AF_INET,SOCK_STREAM,0);
  //  int port=5901;
  
  struct sockaddr_in dstAddr;
  
  memset(&dstAddr, 0, sizeof(dstAddr));
  dstAddr.sin_port = htons(port);
  dstAddr.sin_family = AF_INET;
  dstAddr.sin_addr.s_addr = inet_addr(server.c_str());
  if (dstAddr.sin_addr.s_addr == INADDR_NONE) {
    struct hostent *host;
    host = gethostbyname(server.c_str());
    if (host == NULL) {
      return 1;
    }
    dstAddr.sin_addr.s_addr =
      *(unsigned int *)host->h_addr_list[0];
  }

  
 
  assert(fd>=0);

  r=connect(fd,(sockaddr*)&dstAddr,sizeof(dstAddr));
  assert(r>=0);


  int one = 1;
  setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (const char*)&one, sizeof(one));  
  assert(r>=0);

  unsigned char buf[1024];
  int s;
  {
    s=read(fd,buf,12);
    assert(s==12);
    buf[12]=0;
    printf("%s",buf);
    sscanf((const char*)buf,"RFB %03d.%03d",&vnc_main_ver,&vnc_minor_ver);
    
  }

  {
    //    const char ver[]="RFB 003.008\n";
    // s=write(fd,buf,12);
    // assert(s==12);
    const char ver[]="RFB 003.003\n";
    vnc_main_ver=3;
    vnc_minor_ver=3;
    s=write(fd,ver,sizeof(ver)-1);
    assert(s==12);
  }


  if(vnc_minor_ver >= 7){
    {
      s=read(fd,buf,1);
      int len=buf[0];
      s=read(fd,buf,len);
    
      printf("security\n");
      for(int i=0;i<len;i++){
	printf(" %d\n",(int)buf[i]);
      }
    }
    {
      char stype=2;
      s=write(fd,&stype,1);
    }
    
  }else{
    int security_type;
    read32(security_type);
    assert(security_type==2);
  }


  
  {//read change code
    s=read(fd,buf,16);
    assert(s==16);
    vncEncryptBytes((unsigned char*)buf,pass.c_str());
    s=write(fd,buf,16);
    assert(s==16);
    
  }


  {//check result
    s=read(fd,buf,4);
    assert(s==4);
    uint32_t r=nto_uint32(buf);
    assert(r==0);
  }

  {//client init
    char stype=0;//0:single 1:share
    s=write(fd,&stype,1);
  }
  




  //server init
  
  read16(width);
  read16(height);
  read8(bits_per_pixel);
  read8(depth);
  read8(big_endian_flag);
  read8(true_colour_flag);
  read16(red_max);
  read16(green_max);
  read16(blue_max);
  read8(red_shift);
  read8(green_shift);
  read8(blue_shift);
  read8(padding);
  read8(padding);
  read8(padding);
  read32(name_length);
  readn(name_length);

  img_buf.init(width,height);
  img.init(width,height);
  info_img_buf.init(width,height);
  info_img.init(width,height);

  printf("%d %d %d %d %s\n",width,height,big_endian_flag,bits_per_pixel,buf);

  {
    //setpixelformat
    write8(0);//message-type
    write8(0);//padding
    write8(0);//padding
    write8(0);//padding
  
    write8(32);//pit_per_pixel
    write8(24);//depth
    write8(0);//big-endian
    write8(1);//true-color
  
    write16(255);//reg
    write16(255);//green
  
    write16(255);//blue
    write8(16);//reg-shift
    write8(8);//green-shift
  
    write8(0);//blue-shift
    write8(0);//padding
    write8(0);//padding
    write8(0);//padding

    bits_per_pixel=32;//1
    depth=24;//1
    big_endian_flag=0;//1
    true_colour_flag=1;//1
    red_max=255;//2
    green_max=255;//2
    blue_max=255;//2
    red_shift=16;//1
    green_shift=8;//1
    blue_shift=0;//1
  }
  

  {
    //setencodings
    write8(2);//message-type
    write8(0);//padding
    write16(1);//num of encodings
    write32(0);//Raw

  }


  imgbuf=(uint8_t*)malloc(width*height*bits_per_pixel/8);
  q_empty=1;
  
  set_display(0);
  thread.run(this);
  thread_info.run(this);
  
  return 0;
}

int
VNC_Client::set_display(int inc)
{
  Lock lock(set_mutex);
  unsigned char array[]={
    0x3,
    inc,
    0,0,
    0,0,
    width>>8,width&0xff,
    height>>8,height&0xff
  };
  write(fd,array,sizeof(array));
  fsync(fd);

  // //update request
  // write8(3);       //id
  // write8(inc);       //incremental
  // write16(0);//xpos
  // write16(0);//ypos
  // write16(width);//width
  // write16(height);//height
  return 0;
}


const BMP4b&
VNC_Client::get_display(){
  Lock lock(get_mutex);
  // int mtype;
  int num_of_rec;
  // read8(mtype);
  // assert(mtype==0);
  read8(padding);
  read16(num_of_rec);
  
  //  printf("num_of_rec %d\n",num_of_rec);

  for(int i=0;i<num_of_rec;i++){
    int x,y,w,h,enc;
    int enc0;
    int enc1;
    read16(x);
    read16(y);
    read16(w);
    read16(h);
    read32(enc);

    int  total=0;
    do{
      int num=read(fd,imgbuf+total,h*w*bits_per_pixel/8-total);
      assert(num!=-1);
      total+=num;
    }while(h*w*bits_per_pixel/8!=total);
      
    if(bits_per_pixel==16&&big_endian_flag==0){
      int idx=0;
      for(int j=0;j<h;j++){
	for(int k=0;k<w;k++){
	  uint8_t* t=(uint8_t*)(imgbuf+idx);
	  unsigned short v=(t[1]<<8)|t[0];
	  img_buf(x+k,y+j)[0]=((v>>(red_shift))&(red_max))*(256)/(red_max+1);
	  img_buf(x+k,y+j)[1]=(v>>(green_shift))&(green_max)*(256)/(green_max+1);
	  img_buf(x+k,y+j)[2]=(v>>(blue_shift))&(blue_max)*(256)/(blue_max+1);
	  img_buf(x+k,y+j)[3]=255;
	  idx+=2;
	}
      }
    }else if(bits_per_pixel==32&&
	     big_endian_flag==0&&
	     red_shift==16&&
	     green_shift==8&&
	     blue_shift==0&&
	     red_max==255&&
	     green_max==255&&
	     blue_max==255
	     ){
      int idx=0;
      for(int j=0;j<h;j++){
	for(int k=0;k<w;k++){
	  img_buf.rgb[x+k+img_buf.w*(y+j)].b=imgbuf[idx];
	  img_buf.rgb[x+k+img_buf.w*(y+j)].g=imgbuf[idx+1];
	  img_buf.rgb[x+k+img_buf.w*(y+j)].r=imgbuf[idx+2];
	  img_buf.rgb[x+k+img_buf.w*(y+j)].a=255;
	  idx+=4;
	}
      }
    }else if(bits_per_pixel==32&&big_endian_flag==0){
      for(int j=0;j<h;j++){
	for(int k=0;k<w;k++){
	  int idx=(w*j+k)*4;
	  uint32_t v=*(uint32_t*)(imgbuf+idx);
	  img_buf(x+k,y+j)[0]=((v>>(red_shift))  &(red_max))  *(256)/(red_max+1);
	  img_buf(x+k,y+j)[1]=((v>>(green_shift))&(green_max))*(256)/(green_max+1);
	  img_buf(x+k,y+j)[2]=((v>>(blue_shift)) &(blue_max)) *(256)/(blue_max+1);
	  img_buf(x+k,y+j)[3]=255;
	}
      }
    }else{
      assert(0);
    }
  }
  return img_buf;
}



int
VNC_Client::close(){
  exitp=1;
  thread_info.join();
  thread.join();
  closesocket(fd);
  free(imgbuf);
  socket_finish();
  
  return 0;
}

int
VNC_Client::set_point(int x,int y,int button){
  //  Lock lock(set_mutex);
  unsigned char array[]={
    0x5,
    button,
    x>>8,x&0xff,
    y>>8,y&0xff
  };
  write(fd,array,sizeof(array));
  fsync(fd);
  return 0;
}

int
VNC_Client::set_key(int key,int down){
  Lock lock(set_mutex);
  unsigned char array[]={
    0x4,
    down,
    0,0,
    (key>>24)&0xff,
    (key>>16)&0xff,
    (key>>8)&0xff,
    key&0xff
  };
  write(fd,array,sizeof(array));
  fsync(fd);
  
  // write8(4);//message-type
  // write8(down);//padding
  // write16(0);//num of encodings
  // write32(key);//Raw
  return 0;
}

int
VNC_Client::set_cuttext(const std::string& text){
  Lock lock(set_mutex);
  write8(6);//message-type
  write8(0);//padding
  write8(0);//padding
  write8(0);//padding
  write32(text.length());//num of encodings
  int s=write(fd,text.c_str(),text.length());
  assert(s==text.length());
}

int
VNC_Client::get_colormap(){
  Lock lock(get_mutex);
  int len;
  read8(padding);
  read32(len);
  for(int i=0;i<len;i++){
    unsigned char c;
    read8(c);
  }
  
  return 0;
}

std::string
VNC_Client::get_cuttext(){
  Lock lock(get_mutex);
  std::string r;
  read8(padding);
  read8(padding);
  read8(padding);
  read32(name_length);
  for(int i=0;i<name_length;i++){
    unsigned char c;
    read8(c);
    r+=c;
  }
  return r;
}

void
VNC_Client::wait_get_display(){
  wait_get_display_flag=1;
  Lock lock(get_display_mutex);
  get_display_cond.wait(lock);
}
