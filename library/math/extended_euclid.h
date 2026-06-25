#pragma once
#include<bits/stdc++.h>
using namespace std;
// ax + by = 1 となる pair{x,y}を返す
template<typename T>
pair<T, T> extended_euclid(T a, T b) {
	if (b == 0) return { (T)1, (T)0 };
	auto p = extended_euclid(b, a % b);
	T x = p.second;
	T y = p.first - (a / b) * p.second;
	return { x, y };
}