#pragma once
#include<bits/stdc++.h>
using namespace std;
template<typename T>
struct cycvec : vector<T> {
	using std::vector<T>::vector;
	T& operator[](int n) {
		int tmp = n % this->size();
		return vector<T>::operator[]((tmp >= 0 ? tmp : tmp + this->size()));
	}
	const T& operator[](int n) const {
		int tmp = n % this->size();
		return vector<T>::operator[]((tmp >= 0 ? tmp : tmp + this->size()));
	}
};
template<typename A, size_t N, typename T>
void fill_array(A(&array)[N], const T& val) {
	std::fill((T*)array, (T*)(array + N), val);
}