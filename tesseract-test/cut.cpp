#!/usr/bin/env c-script

#pragma c-script:use data
#pragma c-script:use freetype_util

#define BBOX "\"bbox "
#define STRONGB "<strong>"
#define STRONGE "</strong>"



int
main(int argc,char** argv){
  BMP img(argv[1]);
  FILE* in = fopen(argv[2],"r");
  DATA data;
  char buf[1024];
  int num=0;
  while(num=fread(buf,1,sizeof(buf),in)){
    data.push_back(buf,num);
  }
  char* pos=data.dat;
  char* box=data.dat;
  char* p2;
  char* end=data.dat+data.len;
  char* p;
  char* str;
  Freetype font;
  int xy[4];
  font.init();
  font.set_font( "/usr/share/fonts/truetype/fonts-japanese-gothic.ttf");
  font.set_size(16,16);

  while((p=(char*)memmem(pos,end-pos,STRONGB,strlen(STRONGB)))!=NULL){
    pos=p+strlen(STRONGB);
    str=pos;
    if((p=(char*)memmem(pos,end-pos,STRONGE,strlen(STRONGE)))!=NULL){
      *p=0;
      while((p2=(char*)memmem(box,pos-box,BBOX,strlen(BBOX)))!=NULL){
	p2+=strlen(BBOX);
	sscanf(p2,"%d %d %d %d",xy,xy+1,xy+2,xy+3);
	box=p2;
      }
      
      // printf("%d %d %d %d '%s'\n",
      // 	     xy[0],
      // 	     xy[1],
      // 	     xy[2],
      // 	     xy[3],
      // 	     pos
      // 	     );
      font.getstring(str);
      bmp_for(font.img){
	if((x+xy[0]<img.w)&&(y+xy[1]<img.h)){
	  if(font.img(x,y,0)>128){
	    img(x+xy[0],y+xy[1],0)=0;
	    img(x+xy[0],y+xy[1],1)=0;
	    img(x+xy[0],y+xy[1],2)=0;
	  }else{
	  }
	}
      }
      pos=p+strlen(STRONGE);
    }
    
  }

  // while((p=(char*)memmem(pos,end-pos,BBOX,strlen(BBOX)))!=NULL){
  //   pos=p+strlen(BBOX);
  //   int xy[4];
  //   if(sscanf(pos,"%d %d %d %d",xy,xy+1,xy+2,xy+3)==4){
  //     if((p=(char*)memmem(pos,end-pos,STRONGB,strlen(STRONGB)))!=NULL){
  // 	pos=p+strlen(STRONGB);
  // 	if((p=(char*)memmem(pos,end-pos,STRONGE,strlen(STRONGE)))!=NULL){
  // 	  *p=0;
  // 	  printf("%d %d %d %d '%s'\n",
  // 		 xy[0],
  // 		 xy[1],
  // 		 xy[2],
  // 		 xy[3],
  // 		 pos
  // 		 );
  // 	  font.getstring(pos);
  // 	  bmp_for(font.img){
  // 	    if((x+xy[0]<img.w)&&(y+xy[1]<img.h)){

  // 	      if(font.img(x,y,0)>128){
  // 		img(x+xy[0],y+xy[1],0)=0;
  // 		img(x+xy[0],y+xy[1],1)=0;
  // 		img(x+xy[0],y+xy[1],2)=0;
  // 	      }else{
  // 	      }
  // 	    }
  // 	  }

  // 	  pos=p+strlen(STRONGE);
  // 	}else{
  // 	  break;
  // 	}
  //     }else{
  // 	break;
  //     }
  //   }else{
  //     break;
  //   }
  // }
  img.write("org_txt.png");
  return 0;
}
