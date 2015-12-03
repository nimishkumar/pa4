#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <iostream>

template <class T>
class Array{
private:
	T *ary;
	int size;
	int max;
	void extend(){
		max+=size*.3+10;
		T* tmp=new T[max];
		for(int i=0;i<size;i++)
			tmp[i]=ary[i];
		delete[] ary;
		ary=tmp;
	}
public:
	Array(){
		size=0;
		max=10;
		ary=new T[max];
	}
	~Array(){
		delete ary;
	}
	void add(T val){
		ary[size]=val;
		size++;
		if(size==max){
			extend();
		}
	}
	int getSize(){
		return size;
	}
	T removeLast(){
		if(size==0){
			std::cout << "ERROR TRIED TO REMOVE FROM AN EMPTY LIST!!" << std::endl;
			return ary[0];
		}
		size--;
		T last= ary[size];
		return last;
	}
	T remove(int ind){
		if(size < ind){
			std::cout << "ERROR TRIED TO REPLACE INVALID MEMORY" << std::endl;
			return ary[0];
		}
		T replaced = ary[ind];
		size--;
		for(int i=ind;i<size;i++)
			replace(i,ary[i+1]);
		//size--;
		return replaced;
	}
	T replace(int ind,T newVal){
		if(size < ind){
			std::cout << "ERROR TRIED TO REPLACE INVALID MEMORY" << std::endl;
			return ary[0];
		}
		T replaced = ary[ind];
		ary[ind] = newVal;
		return replaced;
	}
	T get(int index){
		return ary[index];
	}
	T& operator [](const int index) const{
		if(index < 0 || index > size)
			return ary[0];
		exit(1);
	}
};

#endif
