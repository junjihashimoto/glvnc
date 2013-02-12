extern "C"{
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
}

#include "glcommon.h"
#include "freetype_util_c.h"

int  fps_count=0;
int  fps_Max=0;
  
void
fps_display(int x,int y){
  fps_count++;
  Printf(x,y,"FPS[%03d] FRAME[%03d] ",fps_Max,fps_count);
}
  
void
fps_timer(int value) {
  fps_Max=fps_count;
  fps_count=0;
  glutTimerFunc(1000 , fps_timer , 0);
}


void
Printf(int x,int y,const char* format, ...){
  static char strf_buf[1024];
  va_list ap;
  va_start(ap, format);
  vsnprintf(strf_buf,sizeof(strf_buf), format, ap);
  va_end(ap);

  glPushMatrix();
  glRasterPos2f( x , y );
  
  void* freetypep=freetype_new();
  freetype_init(freetypep);
  freetype_set_font(freetypep,"/usr/share/fonts/truetype/fonts-japanese-gothic.ttf");
  freetype_set_size(freetypep,14,14);
  
  //  font                    = freetypep;//GLUT_BITMAP_HELVETICA_12;
  //  glutBitmapString( GLUT_BITMAP_HELVETICA_10 , (const unsigned char*)strf_buf );
  glutBitmapString( freetypep , (const unsigned char*)strf_buf );
  // glutBitmapCharacter( freetypep , 'A');
  freetype_close(freetypep);
  freetype_delete(freetypep);
  
  glPopMatrix();
}

void
Square(int x1,int y1,int x2, int y2){
  glBegin(GL_QUADS);
  glVertex2i(x1,y1);
  glVertex2i(x2,y1);
  glVertex2i(x2,y2);
  glVertex2i(x1,y2);
  glEnd();
}
