#ifndef LIBBMP
#define LIBBMP

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <stdint.h>

//#define PNG_DEBUG 3

extern "C"{
#ifndef WIN32
#include <unistd.h>
#endif  
#include <png.h>
#include <jpeglib.h>
  
}

#include <string>

#include <rgb.h>


#define bmp_for2(bmp)	 \
  for(int y=0;y<bmp.h;y++) \
    for(int x=0;x<bmp.w;x++)

#define bmp_for3(bmp) \
  for(int y=0;y<bmp.h;y++) \
    for(int x=0;x<bmp.w;x++) \
      for(int z=0;z<3;z++)

#define bmp_for(bmp) bmp_for2(bmp)



template<class T>
struct Bitmap{
  color<T>* rgb;
  int w;
  int h;
  
  Bitmap();
  Bitmap(int w,int h);
  Bitmap(const char* file);
  Bitmap(const Bitmap& bmp);
  ~Bitmap();
  
  int             clear();
  int             init(int w,int h);  
  int             init(const Bitmap& bmp);  
  int             copy(const Bitmap& bmp);
  int             swap(Bitmap& bmp);

  
  Bitmap&         operator =  (const Bitmap& b);
  color<T>&       operator [] (int k);
  const color<T>& operator [] (int k)const;
  color<T>&       operator () (int x,int y);
  const color<T>& operator () (int x,int y)const;
  T&              operator () (int x,int y,int z);
  const T&        operator () (int x,int y,int z)const;
  Bitmap&         operator *= (T v);
  Bitmap&         operator /= (T v);
  
  
  int       check(const char* file);
  
  void      read(const char* file,int w=-1,int h=-1);
  void      readBmp(const char* file);
  void      readPng(const char* file);
  void      readJpeg(const char* file);


  void      write(const char* file);
  void      writeBmp(const char* file);
  void      writePng(const char* file);
  void      writeJpeg(const char* file);
};

typedef Bitmap<int>           BMP;
typedef Bitmap<int>           BMPi;
typedef Bitmap<short>         BMPs;
typedef Bitmap<unsigned char> BMPb;
typedef Bitmap<float>         BMPf;
typedef Bitmap<double>        BMPd;

template<class T>
Bitmap<T>::Bitmap(){
  rgb=NULL;
  w=0;
  h=0;
}

template<class T>
Bitmap<T>::Bitmap(int w,int h){
  rgb=NULL;
  init(w,h);
}

template<class T>
Bitmap<T>::Bitmap(const char* file){
  rgb=NULL;
  w=0;
  h=0;
  read(file,-1,-1);
}

template<class T>
Bitmap<T>::Bitmap(const Bitmap& bmp){
  rgb=NULL;
  w=0;
  h=0;
  copy(bmp);
}

template<class T>
Bitmap<T>::~Bitmap(){
  if(rgb!=NULL){
    free(rgb);
    rgb=NULL;
  }
}

template<class T>
int
Bitmap<T>::clear(){
  memset(rgb,0,sizeof(color<T>)*w*h);
  return 0;
}



template<class T>
int
Bitmap<T>::init(int w,int h){
  if(rgb==NULL){
    this->w=w;
    this->h=h;
    rgb=(color<T>*)malloc(sizeof(color<T>)*w*h);
    assert(rgb!=NULL);
  }else{
    free(rgb);
    this->w=w;
    this->h=h;
    rgb=(color<T>*)malloc(sizeof(color<T>)*w*h);
    assert(rgb!=NULL);
  }
  clear();
  return 0;
}


template<class T>
int
Bitmap<T>::init(const Bitmap<T>& bmp){
  return init(bmp.w,bmp.h);
}


template<class T>
int
Bitmap<T>::copy(const Bitmap<T>& bmp){
  init(bmp.w,bmp.h);
  memcpy(rgb,bmp.rgb,sizeof(color<T>)*w*h);
  return 0;
}

template<class T>
int
Bitmap<T>::swap(Bitmap& bmp){
  assert(rgb==NULL);
  assert(bmp.rgb==NULL);
  assert(bmp.w*bmp.h==w*h);

  color<T>* t=rgb;
  rgb=bmp.rgb;
  bmp.rgb=t;
  return 0;

}



template<class T>
Bitmap<T>&
Bitmap<T>::operator = (const Bitmap& b){
  init(b);
  memcpy(rgb,b.rgb,sizeof(color<T>)*h*w);
  return *this;
}

template<class T>
Bitmap<T>&
Bitmap<T>::operator *= (T v){
  bmp_for3((*this))
    (*this)(x,y,z)*=v;
  return *this;
}

template<class T>
Bitmap<T>&
Bitmap<T>::operator /= (T v){
  bmp_for3((*this))
    (*this)(x,y,z)/=v;
  return *this;
}
  

template<class T>
color<T>&
Bitmap<T>::operator [] (int k){
  return rgb[k];
}

template<class T>
const color<T>&
Bitmap<T>::operator [] (int k)const{
  return rgb[k];
}

template<class T>
color<T>&
Bitmap<T>::operator () (int x,int y){
  return rgb[y*w+x];
}

template<class T>
const color<T>&
Bitmap<T>::operator () (int x,int y)const{
  return rgb[y*w+x];
}

template<class T>
T&
Bitmap<T>::operator () (int x,int y,int z){
  return rgb[y*w+x][z];
}

template<class T>
const T&
Bitmap<T>::operator () (int x,int y,int z)const{
  return rgb[y*w+x][z];
}
  
  
template<class T>
int
Bitmap<T>::check(const char* file){
  FILE *fp;
  char buf[4];
  std::string fname=file;
  fp=fopen(fname.c_str(),"rb");
  if(fp==NULL)
    return 0;
  memset(buf,0,sizeof(buf));
  fread(buf,1,4,fp);
  fclose(fp);

  if(buf[0]=='B' &&
     buf[1]=='M')
    return 1;
  else if(buf[1]=='P' &&
	  buf[2]=='N' &&
	  buf[3]=='G')
    return 1;
  else if(strstr(fname.c_str(),".jpg")!=NULL ||
	  strstr(fname.c_str(),".jpeg")!=NULL)
    return 1;
  else
    return 0;
}
  
template<class T>
void
Bitmap<T>::read(const char* file,int w,int h){
  FILE *fp;
  char buf[4];
  std::string fname=file;
  fp=fopen(fname.c_str(),"rb");
  memset(buf,0,sizeof(buf));
  fread(buf,1,4,fp);
  fclose(fp);

  if(buf[0]=='B' &&
     buf[1]=='M')
    readBmp(fname.c_str());
  else if(buf[1]=='P' &&
	  buf[2]=='N' &&
	  buf[3]=='G')
    readPng(fname.c_str());
  else if(strstr(fname.c_str(),".jpg")!=NULL ||
	  strstr(fname.c_str(),".jpeg")!=NULL)
    readJpeg(fname.c_str());
  return;
}

template<class T>
void
Bitmap<T>::readBmp(const char* filename){
  int16_t bpp;
  unsigned char *begin,*pos;
  unsigned char *parret;
  int offset;
  unsigned char *data=NULL;
  long size=0;
  FILE* file=fopen(filename,"rb");
  //  int32_t headersize=0;
  if(file==NULL)
    return;
  
  fseek(file,0,SEEK_END);
  size=ftell(file);
  data=(unsigned char*)malloc(size);
  
  fseek(file,0,SEEK_SET);
  fread(data,size,1,file);

  offset=*(int32_t*)(data+10);
  w=*(int32_t*)(data+18);
  h=*(int32_t*)(data+22);
  bpp=*(int16_t*)(data+28);
  begin=(unsigned char*)data+offset;
  parret=(unsigned char*)data+54;
  init(w,h);
  
  unsigned char ppos;
  
  for(int y=0;y<h;y++){

    pos=begin+
      (w * bpp/8+
       (4-(w * bpp/8)%4)%4
       )*(h-y-1);
    for(int x=0;x<w;x++){
      color<T>& c=rgb[w*y+x];
      switch(bpp){
      case 1:
	ppos=(*pos>>(7-x%8))&0x1;
	c.b=*(parret+(ppos*4));
	c.g=*(parret+(ppos*4)+1);
	c.r=*(parret+(ppos*4)+2);
	if(x%8==7)
	  pos++;
	break;
      case 4:
	ppos=(*pos>>((1-x%2)*4))&0xf;
	c.b=*(parret+(ppos*4));
	c.g=*(parret+(ppos*4)+1);
	c.r=*(parret+(ppos*4)+2);
	if(x%2==1)
	  pos++;
	break;
      case 8:
	ppos=*pos;
	c.b=*(parret+(ppos*4));
	c.g=*(parret+(ppos*4)+1);
	c.r=*(parret+(ppos*4)+2);
	pos++;
	break;
      case 16:
	unsigned short int p;
	p=*(unsigned short int*)pos;
	c.b=0x1f&p;
	c.g=(0x3e0&p)>>5;
	c.r=(0x7c00&p)>>10;
	pos+=2;
	break;
      case 24:
	c.b=*pos++;
	c.g=*pos++;
	c.r=*pos++;
	break;
      case 32:
	c.b=*pos++;
	c.g=*pos++;
	c.r=*pos++;
	pos++;
	break;
      default:
	printf("Does not support bpp:%d\n",bpp);
	exit(1);
      }
    }
  }
  free(data);
  fclose(file);
}

template<class T>
void
Bitmap<T>::readPng(const char* file){
  char header[8];// 8 is the maximum size that can be checked
  png_structp  png_ptr;
  png_infop    info_ptr;
 
  FILE *fp = fopen(file, "rb");

  fread(header, 1, 8, fp);
  assert(png_sig_cmp((png_byte*)header, 0, 8)==0);

  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  assert(png_ptr!=NULL);

  info_ptr = png_create_info_struct(png_ptr);
  assert(info_ptr!=NULL);

  if (setjmp(png_jmpbuf(png_ptr))){
    fprintf(stderr,"[read_png_file] Error during init_io");
    abort();
  }

  png_init_io(png_ptr, fp);
  png_set_sig_bytes(png_ptr, 8);

  png_read_info(png_ptr, info_ptr);

  
  init((int)info_ptr->width,(int)info_ptr->height);
  int color_type = info_ptr->color_type;
  int bit_depth  = info_ptr->bit_depth;

  png_set_interlace_handling(png_ptr);
  if (color_type & PNG_COLOR_MASK_ALPHA)
    png_set_strip_alpha(png_ptr);

  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png_ptr);

  if (color_type == PNG_COLOR_TYPE_GRAY &&
      bit_depth < 8) png_set_gray_1_2_4_to_8(png_ptr);

  png_read_update_info(png_ptr, info_ptr);

  if (setjmp(png_jmpbuf(png_ptr))){
    fprintf(stderr,"[read_png_file] Error during read_image");
    abort();
  }

  int    r  = (int)info_ptr->rowbytes;
  int    hs = (int)sizeof(png_bytep)  * h;
  int    ts = r * h;
  unsigned char*  t  = (unsigned char*) malloc(hs+ts);
  unsigned char* p   = t+hs;
  unsigned char** hp = (unsigned char**)t;
  assert(t!=NULL);

  for(int i=0;i<h;i++)
    hp[i]=p+r*i;
  
  png_read_image(png_ptr, (png_byte**)t);

  for(int i=0;i<h*w;i++){
    rgb[i][0]=p[i*3];
    rgb[i][1]=p[i*3+1];
    rgb[i][2]=p[i*3+2];
  }

  free(t);
  
  png_destroy_read_struct(&png_ptr, &info_ptr,(png_infopp)NULL);

  fclose(fp);
}

template<class T>
void
Bitmap<T>::readJpeg(const char* file){
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  JSAMPARRAY img;
  int width;
  int height;


  cinfo.err = jpeg_std_error( &jerr );
  jpeg_create_decompress( &cinfo );

  FILE* infile = fopen(file, "rb" );
  jpeg_stdio_src( &cinfo, infile );

  jpeg_read_header( &cinfo, TRUE );
  jpeg_start_decompress( &cinfo );

  width = cinfo.output_width;
  height = cinfo.output_height;

  img = (JSAMPARRAY)malloc( sizeof( JSAMPROW ) * height );
  for (int i = 0; i < height; i++ ) 
    img[i] = (JSAMPROW)malloc( sizeof( JSAMPLE )* 3 * width );


  while( cinfo.output_scanline < cinfo.output_height ) {
    jpeg_read_scanlines( &cinfo,
			 img + cinfo.output_scanline,
			 cinfo.output_height - cinfo.output_scanline
			 );
  }

  jpeg_finish_decompress( &cinfo );
  jpeg_destroy_decompress( &cinfo );
  fclose( infile );


  init(width,height);
  for (int i = 0; i < height; i++ ){
    for (int j = 0; j < width; j++ ) {
      rgb[i*width+j].r=img[i][j * 3 + 0];
      rgb[i*width+j].g=img[i][j * 3 + 1];
      rgb[i*width+j].b=img[i][j * 3 + 2];
    }
  }


  for (int i = 0; i < height; i++ )
    free( img[i] );
  free( img );
}



template<class T>
void
Bitmap<T>::write(const char* file){
  if(strstr(file,".bmp")!=NULL)
    writeBmp(file);
  else if(strstr(file,".png")!=NULL)
    writePng(file);
  else if(strstr(file,".jpg")!=NULL ||
	  strstr(file,".jpeg")!=NULL)
    writeJpeg(file);
  else
    writeBmp(file);
}

template<class T>
void
Bitmap<T>::writeBmp(const char* file){
  unsigned char* data=NULL;
  int16_t bpp=24;
  int32_t fsize=54+(w *bpp/8+(4-(w * bpp/8)%4)%4)*h;
  data=(unsigned char*)malloc(fsize);
  memset(data,0,fsize);
  int pos=0;
  
#define SET4(dat) {int32_t li=(int32_t)(dat);memcpy(data+pos,&li ,4);pos+=4;}
#define SET2(dat) {int16_t si=(int16_t)(dat);memcpy(data+pos,&si ,2);pos+=2;}
#define SET1(dat) {unsigned char ci=(unsigned char)(dat);data[pos++]=ci;}
  
  SET1('B');
  SET1('M');
  SET4(fsize);
  SET2(0);
  SET2(0);
  SET4(54);
  SET4(40);
  SET4(w);
  SET4(h);
  SET2(1);
  SET2(bpp);
  SET4(0);
  SET4(fsize-54);
  SET4(0);
  SET4(0);
  SET4(0);
  SET4(0);

  int diff=(4-(w*(bpp/8))%4)%4;
  for(int y=h-1;y>=0;y--){
    for(int x=0;x<w;x++){
      color<T>& c=rgb[w*y+x];
      SET1(c.b);
      SET1(c.g);
      SET1(c.r);
    }
    for(int j=0;j<diff;j++){
      SET1(0);
    }
  }
  if(fsize!=pos){
    printf("fsize:%d != pos:%d\n",(int)fsize,(int)pos);
    exit(1);
  }

  FILE* f=fopen(file,"wb");
  fwrite(data,fsize,1,f);
  fclose(f);
  free(data);
#undef SET4
#undef SET2
#undef SET1

}

template<class T>
void
Bitmap<T>::writePng(const char* file){
  png_structp  png_ptr;
  png_infop    info_ptr;
  FILE *fp = fopen(file, "wb");
  assert(fp!=NULL);

  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  assert(png_ptr!=NULL);

  info_ptr = png_create_info_struct(png_ptr);
  assert(info_ptr!=NULL);

  if (setjmp(png_jmpbuf(png_ptr))){
    fprintf(stderr,"[write_png_file] Error during init_io");
    abort();
  }

  png_init_io(png_ptr, fp);

  if (setjmp(png_jmpbuf(png_ptr))){
    fprintf(stderr,"[write_png_file] Error during writing header");
    abort();
  }

  int bit_depth=8;
  int color_type=PNG_COLOR_TYPE_RGB;
  
  png_set_IHDR(png_ptr, info_ptr, w, h,
	       bit_depth, color_type, PNG_INTERLACE_NONE,
	       PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);


  if (setjmp(png_jmpbuf(png_ptr))){
    fprintf(stderr,"[write_png_file] Error during writing bytes");
    abort();
  }

  int    r  = (int)info_ptr->rowbytes;
  int    hs = (int)sizeof(png_bytep)  * h;
  int    ts = r * h;
  unsigned char*  t  = (unsigned char*) malloc(hs+ts);
  unsigned char*  p  = t+hs;
  unsigned char** hp = (unsigned char**)t;
  assert(t!=NULL);

  for(int i=0;i<h;i++)
    hp[i]=p+r*i;
  
  for(int i=0;i<h*w;i++){
    p[i*3]  =(unsigned char)rgb[i][0];
    p[i*3+1]=(unsigned char)rgb[i][1];
    p[i*3+2]=(unsigned char)rgb[i][2];
  }

  png_write_image(png_ptr,(png_byte**)t);


  if (setjmp(png_jmpbuf(png_ptr))){
    fprintf(stderr,"[write_png_file] Error during end of write");
    abort();
  }

  png_write_end(png_ptr, NULL);

  
  free(t);

  png_destroy_write_struct(&png_ptr, &info_ptr);

  fclose(fp);
}

template<class T>
void
Bitmap<T>::writeJpeg(const char* file){
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE *outfile;

  JSAMPARRAY img;
  int i, j;

  img = (JSAMPARRAY)malloc( sizeof( JSAMPROW ) * h );
  for ( i = 0; i < h; i++ ) {
    img[i] = (JSAMPROW)malloc( sizeof( JSAMPLE ) * 3 * w );
    for ( j = 0; j < w; j++ ) {
      img[i][ j * 3 + 0 ] = (unsigned char)rgb[i*w+j].r;
      img[i][ j * 3 + 1 ] = (unsigned char)rgb[i*w+j].g;
      img[i][ j * 3 + 2 ] = (unsigned char)rgb[i*w+j].b;
    }
  }

  cinfo.err = jpeg_std_error( &jerr );
  jpeg_create_compress( &cinfo );

  outfile = fopen( file, "wb" );
  jpeg_stdio_dest( &cinfo, outfile );

  cinfo.image_width = w;
  cinfo.image_height = h;
  cinfo.input_components = 3;
  cinfo.in_color_space = JCS_RGB;

  jpeg_set_defaults( &cinfo );
  jpeg_start_compress( &cinfo, TRUE );

  jpeg_write_scanlines( &cinfo, img, h );
  jpeg_finish_compress( &cinfo );

  jpeg_destroy_compress( &cinfo );

  fclose( outfile );
  for ( i = 0; i < h; i++ )
    free( img[i] );
  free( img );
}


#endif

