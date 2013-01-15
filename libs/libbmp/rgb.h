
template<class T>
struct color{
  T r;
  T g;
  T b;
  T a;
  void init(T r,T g,T b,T a);
  color();
  color(T r,T g,T b,T a);
  T& operator[](int k);
};

template<class T> int operator ==(color<T>& a,color<T>& b);
template<class T> int operator !=(color<T>& a,color<T>& b);

typedef color<int> RGB;
typedef color<unsigned char> RGBb;

template<class T>
void
color<T>::init(T r,T g,T b,T a){
  this->r=r;
  this->g=g;
  this->b=b;
  this->a=a;
}

template<class T>
color<T>::color(){
  init(0,0,0,0);
}

template<class T>
color<T>::color(T r,T g,T b,T a){
  init(r,g,b,a);
}

template<class T>
T& 
color<T>::operator[](int k){
  switch(k){
  case 0:
    return r;
  case 1:
    return g;
  case 2:
    return b;
  default:
    return a;
  }
}


template<class T>
int
operator ==(color<T>& a,color<T>& b){
  return
    a.r==b.r &&
    a.g==b.g &&
    a.b==b.b &&
    a.a==b.a
  ;
}

template<class T>
int
operator !=(color<T>& a,color<T>& b){
  return
    !(a.r==b.r &&
      a.g==b.g &&
      a.b==b.b &&
      a.a==b.a
      );
}
