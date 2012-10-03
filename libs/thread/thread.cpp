#include "thread.h"

Thread::Thread(thread_callback do_ptr){
  this->do_ptr=do_ptr;
#ifdef WIN32
  tid=0;
#else
  tid=-1;
#endif
}
void Thread::run(void* arg){
#ifdef WIN32
  tid = (HANDLE)_beginthreadex(NULL, 0, do_ptr,arg, 0, NULL);
#else
  pthread_create(&tid,NULL,do_ptr,arg);
#endif
}
void Thread::run(){
  run(NULL);
}
void Thread::join(){
#ifdef WIN32
  WaitForSingleObject(tid,INFINITE);
  CloseHandle(tid);
#else
  pthread_join(tid,NULL);
#endif
}
void Thread::detach(){
#ifdef WIN32
  CloseHandle(tid);
#else
  pthread_detach(tid);
#endif
    
}

Mutex::Mutex(){
#ifdef WIN32
  m = CreateMutex(NULL,FALSE,NULL);
#else
  pthread_mutex_init(&m,NULL);
#endif
}
Mutex::~Mutex(){
#ifdef WIN32
  CloseHandle(m);
#else
  pthread_mutex_destroy(&m);
#endif
}
void Mutex::lock(){
#ifdef WIN32
  WaitForSingleObject(m,INFINITE);
#else
  pthread_mutex_lock(&m);
#endif
}
void Mutex::unlock(){
#ifdef WIN32
  ReleaseMutex(m);
#else
  pthread_mutex_unlock(&m);
#endif
}


Lock::Lock(Mutex& m):m(m){
  m.lock();
}
Lock::~Lock(){
  m.unlock();
}
void Lock::lock(){
  m.lock();
}
void Lock::unlock(){
  m.unlock();
}

Cond::Cond(){
#ifdef WIN32
  cv = CreateEvent (NULL, TRUE, FALSE, NULL);
#else
  pthread_cond_init(&cv, NULL);
#endif
}
Cond::~Cond(){
#ifdef WIN32
  CloseHandle(cv);
#else
  pthread_cond_destroy(&cv);
#endif
}
void Cond::wait(Mutex& m){
#ifdef WIN32
  ResetEvent(cv);
  SignalObjectAndWait( m.m, cv, INFINITE, FALSE);
#else
  pthread_cond_wait(&cv,&m.m); 
#endif
}
void Cond::wait(Lock& lk){
  wait(lk.m);
}
void Cond::notify(){
#ifdef WIN32
  SetEvent(cv);
#else
  pthread_cond_broadcast(&cv);
#endif
}

