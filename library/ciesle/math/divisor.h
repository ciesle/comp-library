#pragma once
#include<bits/stdc++.h>
using namespace std;

vector<int>divisor(int n) {
	vector<int>ret;
	for (int i = 1;i * i <= n;i++) {
		if (n % i == 0) {
			ret.push_back(i);
			if (i * i != n) ret.emplace_back(n / i);
		}
	}
	sort(ret.begin(), ret.end());
	return ret;
}

vector<vector<int>>divisors(int n) {
	vector<vector<int>>ret(n + 1);
	for (int i = 1;i <= n;i++) {
		for (int j = 1;i * j <= n;j++) {
			ret[i * j].emplace_back(i);
		}
		sort(ret[i].begin(), ret[i].end());
	}
	return ret;
}