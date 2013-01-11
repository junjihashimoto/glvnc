#include <bmp.h>


extern void facedetect(const BMPb& in,BMPb& out);
extern void houghlines(const BMPb& in,BMPb& out);
extern void houghcircles(const BMPb& in,BMPb& out);
extern void opticalflow(const BMPb& pre,const BMPb& cur,BMPb& out);

struct Camera{
  void* cap;
  BMPb  img;
  Camera();
  ~Camera();
  int   open(int w=640,int h=480);
  BMPb& get();
};
