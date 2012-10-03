#ifdef WIN32

#include <windows.h>
#include <process.h>

typedef   unsigned (__stdcall *thread_callback)(void*);
#define THREAD_CALLBACK(func) unsigned __stdcall func

#else

#include <pthread.h>

typedef  void* (*thread_callback)(void*);
#define THREAD_CALLBACK(func) void* func

#endif


struct Thread{
#ifdef WIN32
  HANDLE tid;
#else
  pthread_t tid;
#endif
  thread_callback do_ptr;
  Thread(thread_callback do_ptr);
  void run(void* arg);
  void run();
  void join();
  void detach();
};

struct Mutex{
#ifdef WIN32
  HANDLE m;
#else
  pthread_mutex_t m;
#endif
  Mutex();
  ~Mutex();
  void lock();
  void unlock();
};

struct Lock{
  Mutex& m;
  Lock(Mutex& m);
  ~Lock();
  void lock();
  void unlock();
};
  
struct Cond{
#ifdef WIN32
  HANDLE cv;
#else
  pthread_cond_t cv;
#endif
  Cond();
  ~Cond();
  void wait(Mutex& m);
  void wait(Lock& lk);
  void notify();
};

