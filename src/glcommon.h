#ifndef TEXTURE_H
#define TEXTURE_H

#include <bmp.h>
#include <string>
using namespace std;

#define LEFT_3D  0
#define RIGHT_3D 1


//GL_TEXTURE_2D
//GL_TEXTURE_RECTANGLE_ARB
#ifndef GL_TEXTURE_RECTANGLE_ARB
#define GL_TEXTURE_RECTANGLE_ARB 0x84F5
#endif


struct Texture{
public:
  unsigned int sizeX;  //横
  unsigned int sizeY; //縦
  unsigned char *data;  //画像データ格納
  GLuint texture;
  int    texturep;
  string filename;
  Texture():data(NULL){
    data=NULL;
    texturep=0;
  }
  Texture(const Texture& tex):data(NULL){
    sizeX=tex.sizeX;
    sizeY=tex.sizeY;
    texture=tex.texture;
    texturep=1;
    filename=tex.filename;
    data=(unsigned char*)malloc(sizeX*sizeY*3);
    memcpy(data,tex.data,sizeX*sizeY*3);
  }
  ~Texture(){
    if(data!=NULL){
      free(data);
      data=NULL;
    }
  }
  void getcolor(int x,int y,int& r,int& g,int& b){
    int align=(sizeX*3)%4==0? 0 : 4-((sizeX*3)%4);
    if(0<=x&&x<sizeX&&
       0<=y&&y<sizeY){
      r=data[x*3+y*(3*sizeX+align)+0];
      g=data[x*3+y*(3*sizeX+align)+1];
      b=data[x*3+y*(3*sizeX+align)+2];
    }
  }
  void set(const BMPb& bmp){
    sizeX=bmp.w;
    sizeY=bmp.h;
    if(sizeX==0||sizeY==0)
      return;
    //    sizeX=sizeX%4 == 0 ? sizeX : (4-(sizeX%4))+sizeX;
    int align=(sizeX*3)%4==0? 0 : 4-((sizeX*3)%4);

    
    if(data==NULL){
      data=NULL;
      data=(unsigned char*)malloc(sizeof(unsigned char)*(sizeX+align)*bmp.h*3);
    }
    if(align==0&&(sizeof(bmp.rgb[0])==3)){
      //      memcpy(data,bmp.rgb,3*sizeX*sizeY);
    }else{
      bmp_for3(bmp)
	data[x*3+y*(3*(sizeX)+align)+z]=bmp(x,y,z);
    }
    //    glEnable( GL_TEXTURE_RECTANGLE_ARB );
    if(!texturep){
      glGenTextures( 1, &texture );
      texturep=1;
    }
    glBindTexture( GL_TEXTURE_RECTANGLE_ARB, texture );
    //  glTexParameterf( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    /* glTexParameterf( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST ); */
    /* glTexParameterf( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST ); */
       //       printf("align %d\n",align);
       
    if(align==0&&(sizeof(bmp.rgb[0])==3)){
      //      memcpy(data,bmp.rgb,3*sizeX*sizeY);
      glTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp.rgb );
    }else{
      glTexImage2D( GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
    }
    //    glBindTexture( GL_TEXTURE_RECTANGLE_ARB, 0 );
    //    glDisable(GL_TEXTURE_RECTANGLE_ARB);//テクスチャ無効
  }
  void set(const char* file){
    BMPb in(file);
    set(in);
    filename=file;
  }

  Texture(const BMPb& bmp):data(NULL){
    set(bmp);
  }
  Texture(const char* file):data(NULL){
    set(file);
  }
  Texture&
  operator = (const Texture& tex){
    if(data!=NULL)
      free(data);
    data=NULL;
    sizeX=tex.sizeX;
    sizeY=tex.sizeY;
    texture=tex.texture;
    texturep=1;
    filename=tex.filename;
    data=(unsigned char*)malloc(sizeX*sizeY*3);
    memcpy(data,tex.data,sizeX*sizeY*3);
    return *this;
  }
  
  void display(){
    if(sizeX==0||sizeY==0)
      return;
    glEnable(GL_TEXTURE_RECTANGLE_ARB);//テクスチャ有効
    glBindTexture( GL_TEXTURE_RECTANGLE_ARB, texture );
    //    glEnable(GL_ALPHA_TEST);//アルファテスト開始
    //    glBegin(GL_POLYGON);
    glBegin(GL_QUADS);
    /* glTexCoord2f(0.0f, 0.0f); glVertex2d(0 , sizeY);//左下 */
    /* glTexCoord2f(0.0f, 1.0f); glVertex2d(0 ,  0);//左上 */
    /* glTexCoord2f(1.0f, 1.0f); glVertex2d( sizeX ,  0);//右上 */
    /* glTexCoord2f(1.0f, 0.0f); glVertex2d( sizeX , sizeY);//右下 */

    /* glTexCoord2f(0.0f, 0.0f); glVertex2d(0 ,  0); */
    /* glTexCoord2f(0.0f, 1.0f); glVertex2d(0 , sizeY); */
    /* glTexCoord2f(1.0f, 1.0f); glVertex2d( sizeX , sizeY); */
    /* glTexCoord2f(1.0f, 0.0f); glVertex2d( sizeX ,  0); */


    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0,0);

    glTexCoord2f(sizeX,0);
    glVertex2f(sizeX,0);

    glTexCoord2f(sizeX,sizeY);
    glVertex2f(sizeX,sizeY);

    glTexCoord2f(0,sizeY);
    glVertex2f(0,sizeY);

    /* glTexCoord2f(0.0f, 1.0f); glVertex2d(0 , sizeY);//左下 */
    /* glTexCoord2f(1.0f, 1.0f); glVertex2d(0 ,  0);//左上 */
    /* glTexCoord2f(1.0f, 0.0f); glVertex2d( sizeX ,  0);//右上 */
    /* glTexCoord2f(0.0f, 0.0f); glVertex2d( sizeX , sizeY);//右下 */
    glEnd();
    //    glDisable(GL_ALPHA_TEST);//アルファテスト終了
    glDisable(GL_TEXTURE_RECTANGLE_ARB);//テクスチャ無効
  }
};

extern void set_vsync(int interval);


extern void fps_display(int x=10,int y=10);
extern void fps_timer(int value=0);


void Printf(int x,int y,const char* format, ...);

void Square(int x1,int y1,int x2, int y2);



inline int
lmt16(int x){
  if(x<0)
    return 0;
  else if(x>16)
    return 16;
  else
    return x;
}




struct MouseDat{
  int x;
  int y;
  int state;
  int button;
  int transx;
  int transy;
  int shift;
  int ctrl;
  int alt;
  
  inline MouseDat(){
    x=0;
    y=0;
    state=-1;
    button=-1;
    shift=0;
    ctrl=0;
    alt=0;
  }

  inline int rx(){
    return x-transx;
  }
  inline int ry(){
    return y-transy;
  }
  inline void rinit(){
    transx=0;
    transy=0;
  }
  inline void translate(int x,int y){
    transx+=x;
    transy+=y;
    glTranslatef((float)x,(float)y,0.0f);
  }
  inline int
  hit(int x0,int x1,int y0,int y1){
    return
      x0<rx()&&
      rx()<x1 &&
      y0<=ry()&&
      ry()<y1;
  }
};



#endif
