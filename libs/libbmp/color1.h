
template<class T>
struct color1{
  T r;
  void init(T r);
  color1();
  color1(T r);
  int size() const;
  T& operator[](int k);
  const T& operator[](int k)const ;
};


template<class T> T        my_abs	(const T& a);
template<class T> T        nrm		(const color1<T>& a);
template<class T> color1<T> abs		(const color1<T>& a);
template<class T> color1<T> operator -	(const color1<T>& a,const color1<T>& b);
template<class T> color1<T> operator +	(const color1<T>& a,const color1<T>& b);
template<class T> color1<T> operator *	(const color1<T>& a,const color1<T>& b);
template<class T> color1<T> operator /	(const color1<T>& a,const color1<T>& b);
template<class T> int      operator ==	(const color1<T>& a,const color1<T>& b);
template<class T> int      operator !=	(const color1<T>& a,const color1<T>& b);


template<class T>
void
color1<T>::init(T r){
  this->r=r;
}

template<class T>
color1<T>::color1(){
  init(0);
}

template<class T>
color1<T>::color1(T r){
  init(r);
}

template<class T>
int 
color1<T>::size() const{
  return 1;
}

template<class T>
T& 
color1<T>::operator[](int k){
  return r;
}

template<class T>
const T& 
color1<T>::operator[](int k)const {
  return r;
}

template<class T>
T
nrm(const color1<T>& a){
  color1<T> ab=abs(a);
  return ab.r;
}

template<class T>
color1<T>
abs(const color1<T>& a){
  return color1<T>(my_abs(a.r));
}

template<class T>
color1<T>
operator -(const color1<T>& a,const color1<T>& b){
  return color1<T>(a.r-b.r
		  );
}

template<class T>
color1<T>
operator +(const color1<T>& a,const color1<T>& b){
  return color1<T>(a.r+b.r
		  );
}

template<class T>
color1<T>
operator *(const color1<T>& a,const color1<T>& b){
  return color1<T>(a.r*b.r
		  );
}

template<class T>
color1<T>
operator /(const color1<T>& a,const color1<T>& b){
  return color1<T>(a.r/b.r
		  );
}


template<class T>
int
operator ==(const color1<T>& a,const color1<T>& b){
  return
    a.r==b.r;
}

template<class T>
int
operator !=(const color1<T>& a,const color1<T>& b){
  return
    !(a.r==b.r);
}
