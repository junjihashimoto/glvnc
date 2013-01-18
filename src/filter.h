#ifndef WIN32
#include <bmp.h>


extern void facedetect(const BMP4b& in,BMP4b& out);
extern void houghlines(const BMP4b& in,BMP4b& out);
extern void houghcircles(const BMP4b& in,BMP4b& out);
extern void opticalflow(const BMP4b& pre,const BMP4b& cur,BMP4b& out);
extern BMP4b getcamera(int w=640,int h=480,int devid=0);

struct Camera{
  void* cap;
  BMP4b  img;
  Camera();
  ~Camera();
  int   open(int w=640,int h=480,int devid=0);
  BMP4b& get();
};

extern int get_num_cams();
#endif
