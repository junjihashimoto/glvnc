#ifndef TEXTURE_H
#define TEXTURE_H


#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>
extern "C"{
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
}


#include <bmp.h>
#include <string>

#include "vnclib.h"

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
  Texture();
  Texture(const Texture& tex);
  ~Texture();
  void getcolor(int x,int y,int& r,int& g,int& b,int& a);
  void set(const BMPb& bmp);
  void set(const char* file);


  Texture(const BMPb& bmp);
  Texture(const char* file);
  Texture&
  operator = (const Texture& tex);
  void display();
};

extern void set_vsync(int interval);


extern void fps_display(int x=10,int y=10);
extern void fps_timer(int value=0);
extern Texture tex;

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


extern void get_cuttext(VNC_Client* vncp,const string& cuttext);

#endif
