#include <string>
#include <bmp.h>
#include <queue>
using namespace std;
#include "thread.h"

struct Dat{
  unsigned char* p;
  int   len;
};

struct VNC_Client{
  int fd;
  int width;//2
  int height;//2
  int bits_per_pixel;//1
  int depth;//1
  int big_endian_flag;//1
  int true_colour_flag;//1
  int red_max;//2
  int green_max;//2
  int blue_max;//2
  int red_shift;//1
  int green_shift;//1
  int blue_shift;//1
  int padding;//3
  int name_length;//4

  int vnc_main_ver;
  int vnc_minor_ver;

  char* imgbuf;
  BMPb  img;
  BMPb  img2;
  BMPb  img3;

  Thread thread;
  Thread thread_read;
  int    exitp;

  Mutex  img_mutex;
  Mutex  set_mutex;
  Mutex  get_mutex;
  Mutex  q_mutex;
  Cond   q_cond;
  queue<Dat> que;

  VNC_Client();

  int init(const std::string& server,int port,const std::string& pass);
  int close();
  int set_display(int inc);
  int set_point(int x,int y,int button);
  int set_key(int key,int down);
  int set_cuttext(const std::string& text);
  
  const BMPb& get_display();
  int         get_colormap();
  /* int         get_bell(); */
  std::string get_cuttext();

};

