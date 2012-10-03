#include <cstdlib>
#include <cstdio>
#include <cstring>

extern "C" {
#define  GLX_GLXEXT_PROTOTYPES 1
#ifndef WIN32
#define LINUX 1
#include <X11/Xlib.h>
#else
#include <windows.h>
#include <wingdi.h>
#endif
#include <GL/gl.h>

#ifndef WIN32
#include <GL/glx.h>
#include <GL/glxext.h>
#include <dlfcn.h>
#endif
}


typedef void
(*glXSwapIntervalSGIFunc) (int);

typedef void
(*glXSwapIntervalMESAFunc) (int);

void set_vsync(int interval){
#ifdef WIN32
  typedef bool (*func)(int);
  typedef char* (*wglext)();
  func wglSwapIntervalEXT=NULL;
  wglext wglGetExtensionsString=NULL;
  
  
  if((wglGetExtensionsString = (wglext)wglGetProcAddress("wglGetExtensionsString")) && 
     strstr( (char*)wglGetExtensionsString(), "WGL_EXT_swap_control") == 0) {
    if(wglGetExtensionsString==NULL)
      printf ("Does not support wglGetExtensionsString\n");
    else{
      printf ("Does not support WGL_EXT_swap_control\n");
      printf ("%s\n",(char*)wglGetExtensionsString());
    }
    return;
  }else {
    wglSwapIntervalEXT = (func)wglGetProcAddress("wglSwapIntervalEXT");
    if( wglSwapIntervalEXT ) {
      wglSwapIntervalEXT(interval);
      printf ("Changed invertal:%d\n",interval);
    }else{
      printf ("Does not find wglSwapIntervalEXT\n");
    }
  }
#endif

#ifdef OSX
  long sync = interval;
  CGLSetParameter (CGLGetCurrentContext(), kCGLCPSwapInterval, &sync);
#endif

#ifdef LINUX
  char* pcDummy = NULL;
  glXSwapIntervalSGIFunc glXSwapIntervalSGI = 0;
  glXSwapIntervalMESAFunc glXSwapIntervalMESA = 0;

  pcDummy = (char*)glXQueryExtensionsString (glXGetCurrentDisplay(), 0);

  if (strstr (pcDummy, "GLX_SGI_swap_control") != NULL) {
    glXSwapIntervalSGI = (glXSwapIntervalSGIFunc) dlsym(RTLD_DEFAULT,"glXSwapIntervalSGI");

    if (glXSwapIntervalSGI)
      glXSwapIntervalSGI (interval);
    else
      printf ("Could not get glXSwapIntervalSGI()\n");
  } else if (strstr (pcDummy, "GLX_MESA_swap_control") != NULL) {
    glXSwapIntervalMESA = (glXSwapIntervalMESAFunc) dlsym(RTLD_DEFAULT,"glXSwapIntervalMESA");

    if (glXSwapIntervalMESA)
      glXSwapIntervalMESA (interval);
    else
      printf ("Could not get glXSwapIntervalMESA()\n");
  } else
    printf ("Unable to switch sync-to-vblank on.\n");
#endif

}
