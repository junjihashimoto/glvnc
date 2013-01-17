#include "vnclib.h"

extern void get_cuttext(VNC_Client* vncp,const std::string& cuttext);
int
main(int argc,char** argv){
  VNC_Client vnc;
  vnc.get_cuttext_callback=get_cuttext;
  vnc.init(argv[1],atoi(argv[2]),argv[3]);
  char buf[1024];
  gets(buf);
  vnc.close();
  return 0;
}
