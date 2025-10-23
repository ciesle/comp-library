#pragma once
#include<bits/stdc++.h>
using namespace std;
template<typename T>
class SquareDecompositionBase {
public:
	int n, m;
	vector<T>data;
	vector<T>blocks;
protected:
	T _zero;
	void update_interval(int id) {
		T val = data[id * m];
		for (int i = id * m + 1; i < min(n, (id + 1) * m); i++) {
			val = func(val, data[i]);
		}
		blocks[id] = val;
	}
	virtual inline T product(T& l, T& r) = 0;
public:
	void reset() {
		data.assign(n, _zero);
		blocks.assign((n + m - 1) / m, _zero);
	}
	SquareDecompositionBase() {}
	SquareDecompositionBase(int n) : n(n) {
		m = sqrt(n);
	}
	SquareDecompositionBase(vector<T>&& v) : n(v.size()), data(v) {
		m = sqrt(n);
		blocks.resize((n + m - 1) / m);
		for (int i = 0; i < blocks.size(); i++) {
			update_interval(i);
		}
	}

	virtual void update(int pos, T x) = 0;
	T query(int l, int r) {
		if (l >= r) return _zero;
		T res;
		if (l / m == (r - 1) / m) {
			res = data[l];
			for (int i = l + 1; i < r; i++) {
				res = product(res, data[i]);
			}
			return res;
		}
		if (l % m == 0) res = blocks[l / m], l += m;
		else {
			res = data[l++];
			int tmp = ((l + m - 1) / m) * m;
			for (;l < tmp;l++) res = product(res, data[l]);
		}
		int id = l / m;
		while (l + m <= r) {
			res = product(res, blocks[id++]);
			l += m;
		}
		while (l < r) {
			res = product(res, data[l++]);
		}
		return res;
	}
};