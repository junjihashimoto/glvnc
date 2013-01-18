
template<class T>
struct color4{
  T r;
  T g;
  T b;
  T a;
  void init(T r,T g,T b,T a);
  color4();
  color4(T r,T g,T b,T a);
  int size() const;
  T& operator[](int k);
  const T& operator[](int k)const ;
};


template<class T> T        my_abs	(const T& a);
template<class T> T        nrm		(const color4<T>& a);
template<class T> color4<T> abs		(const color4<T>& a);
template<class T> color4<T> operator -	(const color4<T>& a,const color4<T>& b);
template<class T> color4<T> operator +	(const color4<T>& a,const color4<T>& b);
template<class T> color4<T> operator *	(const color4<T>& a,const color4<T>& b);
template<class T> color4<T> operator /	(const color4<T>& a,const color4<T>& b);
template<class T> int      operator ==	(const color4<T>& a,const color4<T>& b);
template<class T> int      operator !=	(const color4<T>& a,const color4<T>& b);

typedef color4<int> RGBA;
typedef color4<unsigned char> RGBAb;

template<class T>
void
color4<T>::init(T r,T g,T b,T a){
  this->r=r;
  this->g=g;
  this->b=b;
  this->a=a;
}

template<class T>
color4<T>::color4(){
  init(0,0,0,0);
}

template<class T>
color4<T>::color4(T r,T g,T b,T a){
  init(r,g,b,a);
}

template<class T>
int 
color4<T>::size() const{
  return 4;
}

template<class T>
T& 
color4<T>::operator[](int k){
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
const T& 
color4<T>::operator[](int k)const {
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
T
nrm(const color4<T>& a){
  color4<T> ab=abs(a);
  return ab.r+ab.g+ab.b+ab.a;
}

template<class T>
color4<T>
abs(const color4<T>& a){
  return color4<T>(my_abs(a.r),my_abs(a.g),my_abs(a.b),my_abs(a.a));
}

template<class T>
color4<T>
operator -(const color4<T>& a,const color4<T>& b){
  return color4<T>(a.r-b.r,
		   a.g-b.g,
		   a.b-b.b,
		   a.a-b.a
		  );
}

template<class T>
color4<T>
operator +(const color4<T>& a,const color4<T>& b){
  return color4<T>(a.r+b.r,
		   a.g+b.g,
		   a.b+b.b,
		   a.a+b.a
		  );
}

template<class T>
color4<T>
operator *(const color4<T>& a,const color4<T>& b){
  return color4<T>(a.r*b.r,
		   a.g*b.g,
		   a.b*b.b,
		   a.a*b.a
		  );
}

template<class T>
color4<T>
operator /(const color4<T>& a,const color4<T>& b){
  return color4<T>(a.r/b.r,
		   a.g/b.g,
		   a.b/b.b,
		   a.a/b.a
		  );
}


template<class T>
int
operator ==(const color4<T>& a,const color4<T>& b){
  return
    a.r==b.r &&
    a.g==b.g &&
    a.b==b.b &&
    a.a==b.a;
}

template<class T>
int
operator !=(const color4<T>& a,const color4<T>& b){
  return
    !(a.r==b.r &&
      a.g==b.g &&
      a.b==b.b &&
      a.a==b.a);
}
