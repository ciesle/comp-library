#pragma once
#include<vector>
#include<tuple>
using namespace std;
//tree size, characters, edge, points (depth = size of T bit)
template<typename T>
tuple<int, vector<bool>, vector<vector<int>>, vector<vector<int>>>Binary_trie_tree(vector<T>& s) {
	using U = typename std::make_unsigned<T>::type;
	constexpr int BITS = sizeof(U) * 8;
	int n = s.size(), m = 1;
	vector<bool>c(1, 0);
	vector<vector<int>>e(1, vector<int>(2, -1));
	vector<vector<int>>pos(1);
	for (int i = 0;i < n;i++) {
		int t = 0;
		for (int j = BITS - 1; j >= 0; --j) {
			int k = (static_cast<U>(s[i]) >> j) & 1;
			if (e[t][k] < 0) {
				c.emplace_back(k);
				e.emplace_back(vector<int>(2, -1));
				pos.emplace_back();
				e[t][k] = m;
				t = m++;
			}
			else t = e[t][k];
		}
		pos[t].emplace_back(i);
	}
	return forward_as_tuple(m, c, e, pos);
}