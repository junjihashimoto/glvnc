
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <string>



using namespace std;


extern "C"{
#ifndef  WIN32
#include <dlfcn.h>
#include <libgen.h>
#include <execinfo.h>
#include <signal.h>
#include <iconv.h>
#include <errno.h>
#endif
  
#include <sys/stat.h>
#include <sys/types.h>


}


#include "utils.h"



string
strfs(const char* format, ...){
  char strf_buf[1024];
  va_list ap;
  va_start(ap, format);
  vsnprintf(strf_buf,sizeof(strf_buf), format, ap);
  va_end(ap);
  return string(strf_buf);
}


#ifndef  WIN32


void
handler(int sig) {
  void *array[10];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 10);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, (int)size, 2);
  exit(1);
}

class HANDLER_INIT{
public:
  HANDLER_INIT(){
    //    signal(SIGSEGV, handler);
  }
}handler_init;

#endif

#ifndef WIN32
void
Eval::source(char* file){
  strcpy(dll_file,"aaXXXXXX");
  mktemp(dll_file);
  
  if(hdl!=NULL){
    return;
  }
  // strcpy(tmpchars,"source");
  // strcpy(dll_file,mktemp(tmpchars));
  //  strcpy(dll_file,"param.so");


  string dir;
  for(unsigned int i=0;i<incdirs.size();i++){
    dir+="-I"+incdirs[i]+" ";
  }
  
  system(strf("g++ -g -shared -o %s  %s -shared -fPIC %s",
	      dll_file,
	      file,
	      dir.c_str()
	      )
	 );

  char buf[1024]="./";
  strcat(buf,dll_file);
  
  void* hdl=dlopen(buf,RTLD_LAZY|RTLD_GLOBAL);
  if(hdl==NULL){
    printf("%s\n",dlerror());
    exit(1);
  }

  void (*param_init)()=(void(*)())dlsym(hdl,"init");
  if(param_init==NULL){
    printf("%s\n",dlerror());
    exit(1);
  }
  param_init();
}


void
Eval::call(char* func){
  void (*param_init)()=(void(*)())dlsym(hdl,func);
  if(param_init==NULL){
    printf("%s\n",dlerror());
    exit(1);
  }
}

void*
Eval::fnc_ptr(char* func){
  void *param_init=(void(*))dlsym(hdl,func);
  if(param_init==NULL){
    printf("%s\n",dlerror());
    exit(1);
  }
  return param_init;
}

void
Eval::close(){
  if(hdl!=NULL)
    dlclose(hdl);
  hdl=NULL;
  unlink(dll_file);
}

Eval::Eval(){
  hdl=NULL;
}
Eval::~Eval(){
  close();
}
#endif


string
convert_string(const string& src_format,
	       const string& dst_format,
	       const string& src){
#ifdef WIN32
#warning "convert_string is not implemented."  
  return src;
#else
  
  iconv_t icd;
  char s_dst[1024+1];
  char *p_src, *p_dst;
  size_t n_src, n_dst;
  string dst;

  //  icd = iconv_open("UTF-8", "Shift_JIS");
  icd = iconv_open(dst_format.c_str(),
		   src_format.c_str()
  		   );
  p_src = (char*)src.c_str();
  n_src = src.length();
  

  while(0 < n_src){
    //    printf("iconv\n");
    p_dst = s_dst;
    n_dst = sizeof(s_dst)-1;
    int err=iconv(icd, &p_src, &n_src, &p_dst, &n_dst);
    if(err==-1){
      perror("convert_string error\n");
      if(errno==E2BIG)
  	printf("E2BIG:buffer is too small\n");
      else if(errno==EILSEQ)
  	printf("EILSEQ:invalid multibyte sequence\n");
      else if(errno==EINVAL)
  	printf("EILSEQ:incomplete multibyte sequence\n");
      break;
    }
    
    *p_dst = '\0';
    dst+=s_dst;
  }

  iconv_close(icd);
  return dst;
#endif
}


int
file_size(const char* file){
  struct stat sb;
  if (stat(file, &sb) == -1) {
    perror("stat");
    exit(EXIT_FAILURE);
    return -1;
  }
  return sb.st_size;
  
}

int
filesize(const char* file){
  struct stat sb;
  if (stat(file, &sb) == -1) {
    perror("stat");
    exit(EXIT_FAILURE);
    return -1;
  }
  return sb.st_size;
  
}

int
file_exist(const char* file){
  struct stat sb;
  if (stat(file, &sb) != 0)
    return 0;
  else
    return 1;
}

int
file_reg(const char* file){
  struct stat sb;
  return stat(file, &sb) == S_IFREG;
}
int
file_dir(const char* file){
  struct stat sb;
  return stat(file, &sb) == S_IFDIR;
}

#ifdef WIN32
int
mkdir(const char *pathname, mode_t mode){
  return mkdir(pathname);
}
#endif
