
template<class T>
struct color{
  T r;
  T g;
  T b;
  void init(T r,T g,T b);
  color();
  color(T r,T g,T b);
  int size() const;
  T& operator[](int k);
  const T& operator[](int k)const ;
};


template<class T> T        my_abs	(const T& a);
template<class T> T        nrm		(const color<T>& a);
template<class T> color<T> abs		(const color<T>& a);
template<class T> color<T> operator -	(const color<T>& a,const color<T>& b);
template<class T> color<T> operator +	(const color<T>& a,const color<T>& b);
template<class T> color<T> operator *	(const color<T>& a,const color<T>& b);
template<class T> color<T> operator /	(const color<T>& a,const color<T>& b);
template<class T> int      operator ==	(const color<T>& a,const color<T>& b);
template<class T> int      operator !=	(const color<T>& a,const color<T>& b);

typedef color<int> RGB;
typedef color<unsigned char> RGBb;

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
int 
color<T>::size() const{
  return 3;
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
const T& 
color<T>::operator[](int k)const {
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
T
my_abs(const T& a){
  return a<0? -a:a;
}

template<class T>
T
nrm(const color<T>& a){
  color<T> ab=abs(a);
  return ab.r+ab.g+ab.b;
}

template<class T>
color<T>
abs(const color<T>& a){
  return color<T>(my_abs(a.r),my_abs(a.g),my_abs(a.b));
}

template<class T>
color<T>
operator -(const color<T>& a,const color<T>& b){
  return color<T>(a.r-b.r,
		  a.g-b.g,
		  a.b-b.b
		  );
}

template<class T>
color<T>
operator +(const color<T>& a,const color<T>& b){
  return color<T>(a.r+b.r,
		  a.g+b.g,
		  a.b+b.b
		  );
}

template<class T>
color<T>
operator *(const color<T>& a,const color<T>& b){
  return color<T>(a.r*b.r,
		  a.g*b.g,
		  a.b*b.b
		  );
}

template<class T>
color<T>
operator /(const color<T>& a,const color<T>& b){
  return color<T>(a.r/b.r,
		  a.g/b.g,
		  a.b/b.b
		  );
}


template<class T>
int
operator ==(const color<T>& a,const color<T>& b){
  return
    a.r==b.r &&
    a.g==b.g &&
    a.b==b.b;
}

template<class T>
int
operator !=(const color<T>& a,const color<T>& b){
  return
    !(a.r==b.r &&
      a.g==b.g &&
      a.b==b.b);
}
