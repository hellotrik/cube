static int DIGS=31;
static int mersennep=(1<<DIGS)-1;
int universalHash(int x,int A,int B,int M){
  long hashVal=(long)A*x+B;
  hashVal=((hashVal>>DIGS))+(hashVal&mersennep);
  if(hashVal>=mersennep) hashVal-=mersennep;
  return (int)hashVal%M;
  }
  
  #include "stdafx.h"
#include <string>
#include <iostream>
#include <windows.h>
using namespace std;
template <class T>
class Array{
private:
	T* list; int size;
public:
	Array(int sz = 50);
	Array(const Array<T>&a);
	~Array();
	Array<T>& operator= (const Array<T>&rhs);
	T& operator[] (int i);
	const T& operator[] (int i)const;
	operator T* ();
	operator const T*()const;
	int getSize()const;
	void resize(int sz);
};
template <class T>
Array<T>::Array(int sz){
	size = sz; list=new T[size];
}
template<class T>
Array<T>::~Array(){
	delete[] list;
}
template<class T>
Array<T>::Array(const Array<T>&a){
	size = a.size;
	list = new T[size];
	for (int i = 0; i < size; i++)
		list[i] = a.list[i];
}
template<class T>
Array<T>& Array<T>::operator=(const Array<T> &rhs){
	if (&rhs != this){
		if (size != rhs.size){
			delete[] list; size = rhs.size; list = new T[size];
		}
		for (int i = 0; i < size; i++)
			list[i] = rhs.list[i];
	}
	return *this;
}
template<class T>
T& Array<T>::operator[] (int n){
	return list[n];
}
template<class T>
const T &Array<T>::operator[] (int n)const{
	if (n >= 0 && n < size)
		return list[n];
	else return NULL;
}
template<class T>
Array<T>::operator const T *()const {
	return list;
}
template<class T>
Array<T>::operator T *(){
	return list;
}
template<class T>
int Array<T>::getSize()const{
	return size;
}
template<class T>
void Array<T>::resize(int sz){
	if (sz == size)
		return;
	T* newList = new T[sz];
	int n = (sz < size) ? sz : size;
	for (int i = 0; i < n; i++)
		newList[i] = list[i];
	delete[] list;
	list = newList;
	size = sz;
}

int _tmain(int argc, char *arg[]){
	Array<int> ar(10);
	for (int i = 0; i < 10; i++) ar[i] = i;
	ar.resize(5); cout << ar.getSize() << endl;
	cout << typeid(*ar).name() << endl;
	return 0;
}
