template<class T>
struct AutoPtr{
  T*  p;
  int size;
  AutoPtr(size_t s);
  ~AutoPtr();
  operator T*();
  T& operator [](int i);
};

