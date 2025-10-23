#pragma once
#include <bits/stdc++.h>
using namespace std;

//Mo's algorithm
//template: type of array, type of interval index
template<typename T, typename F1, typename F2, typename F3, typename F4, typename U>
class Mo {
	void init(vector<T>&& v) {
		m = sqrt(n);
		sort(v.begin(), v.end(), [&](T& a, T& b) {
			if (*((U*)&a) / m != *((U*)&b) / m) return *(U*)&a < *(U*)&b;
			return ((*((U*)&a) / m) & 1) ? (*((U*)&a + 1) > *((U*)&b + 1)) : (*((U*)&a + 1) < *((U*)&b + 1));
			});
		a = v;
		index = 0;
	}
public:
	vector<T>a;
	U n, m, maxn;
	pair<U, U>current = { 0,0 };
	size_t index;
	F1 lshrink;
	F2 lexpand;
	F3 rshrink;
	F4 rexpand;
	/*
	input: array, maximam value  of interval
	reset: もし必要なデータ構造が外にあれば、その初期化を行わないといけない
	lshrink, lexpand, rexpand: lambda functions for changing interval
	*/
	Mo(vector<T>v, U n, F1 lshrink, F2 lexpand, F3 rshrink, F4 rexpand) : n(n),
		lshrink(forward<F1>(lshrink)), lexpand(forward<F2>(lexpand)),
		rshrink(forward<F3>(rshrink)), rexpand(forward<F4>(rexpand)) {
		init(move(v));
	}
	Mo(vector<T>v, U n, F1 lshrink, F2 lexpand) : n(n),
		lshrink(forward<F1>(lshrink)), lexpand(forward<F2>(lexpand)),
		rshrink(forward<F3>(lshrink)), rexpand(forward<F4>(lexpand)) {
		init(move(v));
	}

	//execute lambda functions and return new interval
	T process() {
		if (index == a.size()) return T();
		while (current.first > *(U*)(&a[index])) lexpand(--current.first);
		while (current.second < *((U*)&a[index] + 1)) rexpand(current.second++);
		while (current.first < *(U*)&a[index]) lshrink(current.first++);
		while (current.second > *((U*)&a[index] + 1)) rshrink(--current.second);
		return a[index++];
	}
};
template<typename T, typename F1, typename F2, typename F3, typename F4, typename U>
Mo(vector<T>, U, F1&&, F2&&, F3&&, F4&&) -> Mo<T, F1, F2, F3, F4, U>;
template<typename T, typename F1, typename F2, typename U>
Mo(vector<T>, U, F1, F2) -> Mo<T, F1, F2, F1, F2, U>;