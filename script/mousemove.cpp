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
  if(argc<4){
    printf ("Usage: ./mousemove hostname port(5900+vncport) password\n");
    return 1;
  }
  
  vnc.init(argv[1],atoi(argv[2]),argv[3]);
  for(int i=0;i<1024*100;i++){
    vnc.set_point(i%1024,i/768,0);
  }
  vnc.close();
  return 0;
}
