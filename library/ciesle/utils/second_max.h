#pragma once
#include<bits/stdc++.h>
using namespace std;

// return the index of max element, second-max element
template<bool max = true, typename T>
pair<int, int>second_max(vector<T>& v) {
	int t[2] = { -1,-1 };
	for (int i = 0;i < v.size();i++) {
		if constexpr (max) {
			if (t[0]<0 || v[i]>v[t[0]]) t[1] = t[0], t[0] = i;
			else if (t[1]<0 || v[i]>v[t[1]]) t[1] = i;
		}
		else {
			if (t[0] < 0 || v[i] < v[t[0]]) t[1] = t[0], t[0] = i;
			else if (t[1] < 0 || v[i] < v[t[1]]) t[1] = i;
		}
	}
	return make_pair(t[0], t[1]);
}