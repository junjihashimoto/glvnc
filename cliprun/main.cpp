#include "vnclib.h"

int
main(int argc,char** argv){
  VNC_Client vnc;
  vnc.init(argv[1],atoi(argv[2]),argv[3]);
  char buf[1024];
  gets(buf);
  vnc.close();
  return 0;
}
