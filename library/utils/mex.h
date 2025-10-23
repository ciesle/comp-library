#pragma once
#include<vector>
#include<algorithm>
using std::vector;
//for grundy number
int mex(vector<int>& v) {
	sort(v.begin(), v.end());
	int cur = 0;
	for (int i = 0;i < v.size();i++) {
		if (v[i] == cur) cur++;
		if (cur < v[i]) break;
	}
	return cur;
}