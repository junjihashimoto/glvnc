#!/usr/bin/env c-script

#pragma c-script:libs $HOME/glvnc/libs/vnc/libvnc.a
#pragma c-script:libs $HOME/glvnc/libs/thread/libthread.a
#pragma c-script:opt  -I$HOME/glvnc/libs/vnc
#pragma c-script:opt  -I$HOME/glvnc/libs/thread

#include "vnclib.h"
#include <unistd.h>
int
main(int argc,char** argv){
  VNC_Client vnc;
  if(argc<5){
    printf ("Usage: ./getimg hostname port(5900+vncport) password img-name\n");
    return 1;
  }
  
  vnc.init(argv[1],atoi(argv[2]),argv[3]);
  vnc.wait_get_display();
  {
    Lock lock(vnc.img_mutex);
    vnc.img.write(argv[4]);
  }
  vnc.close();
  return 0;
}
