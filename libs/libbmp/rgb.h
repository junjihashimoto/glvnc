
template<class T>
struct color{
  T r;
  T g;
  T b;
  void init(T r,T g,T b);
  color();
  color(T r,T g,T b);
  T& operator[](int k);
};

template<class T> int operator ==(color<T>& a,color<T>& b);
template<class T> int operator !=(color<T>& a,color<T>& b);

typedef color<int> RGB;

template<class T>
void
color<T>::init(T r,T g,T b){
  this->r=r;
  this->g=g;
  this->b=b;
}

template<class T>
color<T>::color(){
  init(0,0,0);
}

template<class T>
color<T>::color(T r,T g,T b){
  init(r,g,b);
}

template<class T>
T& 
color<T>::operator[](int k){
  switch(k){
  case 0:
    return r;
  case 1:
    return g;
  default:
    return b;
  }
}


template<class T>
int
operator ==(color<T>& a,color<T>& b){
  return
    a.r==b.r &&
    a.g==b.g &&
    a.b==b.b;
}

template<class T>
int
operator !=(color<T>& a,color<T>& b){
  return
    !(a.r==b.r &&
      a.g==b.g &&
      a.b==b.b);
}
