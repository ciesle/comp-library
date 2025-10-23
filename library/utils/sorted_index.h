#pragma once
#include<bits/stdc++.h>
using namespace std;

// vec a -> vec<pair<T, int>> b( value, index )
template<typename T>
vector<pair<T, int>>sorted_index(vector<T>a) {
	vector<pair<T, int>>ret(a.size());
	for (int i = 0;i < a.size();i++) {
		ret[i].first = a[i];
		ret[i].second = i;
	}
	sort(ret.begin(), ret.end());
	return ret;
}