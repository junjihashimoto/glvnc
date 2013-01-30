#ifndef VNCLIB_H
#define VNCLIB_H

#include <string>
/* #include <queue> */
/* using namespace std; */
#include "thread.h"

#ifndef WIN32
#define SOCKET int
#else
#include <winsock2.h>
#define HAVE_BOOLEAN
#endif

#include <bmp.h>


struct Dat{
  unsigned char* p;
  int   len;
};

struct VNC_Client{
  SOCKET fd;
  int width;//2
  int height;//2
  int bits_per_pixel;//1
  int depth;//1
  int big_endian_flag;//1
  int true_color_flag;//1
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

  uint8_t* imgbuf;
  BMP4b  img_buf;
  BMP4b  img;
  BMP4b  info_img_buf;
  BMP4b  info_img;

  Thread thread;
  
  Thread thread_info;
  int    exitp;
  int    mode;

  Mutex  img_mutex;
  Mutex  set_mutex;
  Mutex  get_mutex;
  
  Mutex  q_mutex;
  Cond   q_cond;
  int    q_empty;

  int    wait_get_display_flag;
  Mutex  get_display_mutex;
  Cond   get_display_cond;
  

  VNC_Client();

  int init(const std::string& server,int port,const std::string& pass);
  int close();
  int set_display(int inc);
  int set_point(int x,int y,int button);
  int set_key(int key,int down);
  int set_cuttext(const std::string& text);
  
  void wait_get_display();

  void (*get_cuttext_callback)(VNC_Client* vncp,const std::string& str);
  void (*get_display_callback)(VNC_Client* vncp,const BMP4b& in);
  void (*img_filter_callback)(VNC_Client* vncp,const BMP4b& in,BMP4b& out);
  
  const BMP4b& get_display();
  int         get_colormap();
  /* int         get_bell(); */
  std::string get_cuttext();
};

#endif

