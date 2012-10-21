#include "glcommon.h"
Texture::Texture():data(NULL){
  data=NULL;
  texturep=0;
}
Texture::Texture(const Texture& tex):data(NULL){
  sizeX=tex.sizeX;
  sizeY=tex.sizeY;
  texture=tex.texture;
  texturep=1;
  filename=tex.filename;
  data=(unsigned char*)malloc(sizeX*sizeY*3);
  memcpy(data,tex.data,sizeX*sizeY*3);
}
Texture::~Texture(){
  if(data!=NULL){
    free(data);
    data=NULL;
  }
}
void Texture::getcolor(int x,int y,int& r,int& g,int& b){
  int align=(sizeX*3)%4==0? 0 : 4-((sizeX*3)%4);
  if(0<=x&&x<sizeX&&
     0<=y&&y<sizeY){
    r=data[x*3+y*(3*sizeX+align)+0];
    g=data[x*3+y*(3*sizeX+align)+1];
    b=data[x*3+y*(3*sizeX+align)+2];
  }
}
void Texture::set(const BMPb& bmp){

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
void Texture::set(const char* file){
  BMPb in(file);
  set(in);
  filename=file;
}

Texture::Texture(const BMPb& bmp):data(NULL){
  set(bmp);
}
Texture::Texture(const char* file):data(NULL){
  set(file);
}
Texture&
Texture::operator = (const Texture& tex){
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
  
void Texture::display(){
  if(sizeX==0||sizeY==0|texturep==0)
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
