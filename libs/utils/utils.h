#ifndef LIBUTILS
#define LIBUTILS

#include <string>
#include <vector>

extern "C"{
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
}


#define MAX(a,b) ((a)>=(b) ? (a) : (b))
#define MIN(a,b) ((a)< (b) ? (a) : (b))
#define MAX3(a)  MAX(MAX(a[0],a[1]),a[2])
#define MIN3(a)  MIN(MIN(a[0],a[1]),a[2])
#define LMT(a)  ((a)>255 ? 255 : ((a)<0 ? 0: a))
inline int lmt(int a){
  if(a>255)
    return 255;
  else if(a<0)
    return 0;
  return a;
}

extern std::string strfs(const char* format, ...);
#define strf(x,...) strfs(x,__VA_ARGS__).c_str()


extern int filesize(const char* file);
extern int file_size(const char* file);
extern int file_exist(const char* file);
extern int file_reg(const char* file);
extern int file_dir(const char* file);

extern int check_mac_addr();

#ifndef WIN32
class Eval{
  void* hdl;
  char  dll_file[1024];
public:
  std::vector<std::string> incdirs;
  void source(char* file);
  void call(char* func);
  void* fnc_ptr(char* func);  
  void close();
  Eval();
  ~Eval();
};
#endif


#ifdef WIN32
extern int mkdir (const char *pathname, mode_t mode);
#endif

#endif

