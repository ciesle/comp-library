#pragma once
#include<bits/stdc++.h>
using namespace std;

template<typename T>
vector<T>pow(vector<T>v, long long x) {
	vector<T>ret{ 1 };
	while (x > 0) {
		if (x & 1 == 1) {
			ret = convolution(ret, v);
		}
		v = convolution(v, v);
		x >>= 1;
	}
	return ret;
}

template<typename T>
vector<T>div(vector<T>a, vector<T>b) {
	if (a.size() < b.size()) return vector<T>{};
	vector<T>ret(a.size() - b.size() + 1);

	for (int i = 0;i < ret.size();i++) {
		ret[i] = a[i];
		for (int j = 1; j < b.size(); j++) {
			if (i - j < 0) break;
			ret[i] -= ret[i - j] * b[j];
		}
		ret[i] /= b[0];
	}
	return ret;
}