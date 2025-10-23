#pragma once
#include <bits/stdc++.h>
using namespace std;
template<typename T>
class BIT {
	int size;
	vector<T>dat;
public:
	BIT() {}
	BIT(vector<T>v) {
		init(v);
	}
	BIT(int n) { init(n); }
	void init(int n) {
		size = n;
		dat.assign(size + 1, 0);
	}
	void init(vector<T>v) {
		init(v.size());
		for (int i = 1; i <= size; i++) {
			dat[i] += v[i - 1];
			if (i + (i & -i) <= size) dat[i + (i & -i)] += dat[i];
		}
	}
	void add(int i, long long x) {
		i++;
		while (i <= size) {
			dat[i] += x;
			i += i & -i;
		}
	}
	void change(int i, long long x) {
		i++; int t = x - dat[i];
		while (i <= size) {
			int k = dat[i];
			dat[i] += t;
			t = dat[i] - k;
			i += i & -i;
		}
	}
	//[l,r)
	void add(int l, int r, long long x) {
		add(l, x); add(r, -x);
	}
	//[0,i]
	T query(int i) {
		i++;
		T sum = 0;
		while (i > 0) {
			sum += dat[i];
			i -= i & -i;
		}
		return sum;
	}
	//[l,r)
	T query(int l, int r) {
		return query(r - 1) - query(l - 1);
	}
	//[0,x] >= w
	int lower_bound(T w) {
		if (w <= 0) return 0;
		int x = 0, k = 1;
		while ((k << 1) < size) k <<= 1;
		for (; k > 0; k >>= 1) {
			if (x + k <= size && dat[x + k] < w) {
				w -= dat[x + k];
				x += k;
			}
		}
		return x;
	}
};//size, 0-indexed