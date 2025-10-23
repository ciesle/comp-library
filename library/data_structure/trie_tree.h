#pragma once
#include<vector>
#include<string>
#include<tuple>
using namespace std;
//tree size, characters, edge, points
tuple<int, vector<char>, vector<vector<int>>, vector<vector<int>>>Trie_tree(vector<string>& s) {
	int n = s.size(), m = 1;
	constexpr char base = 'a';
	constexpr char char_size = 26;
	vector<char>c(1, 0);
	vector<vector<int>>e(1, vector<int>(char_size, -1));
	vector<vector<int>>pos(1);
	for (int i = 0;i < n;i++) {
		int t = 0;
		for (int j = 0;j < s[i].size();j++) {
			if (e[t][s[i][j] - base] < 0) {
				c.emplace_back(s[i][j]);
				e.emplace_back(vector<int>(char_size, -1));
				pos.emplace_back();
				e[t][s[i][j] - base] = m;
				t = m++;
			}
			else t = e[t][s[i][j] - base];
		}
		pos[t].emplace_back(i);
	}
	return forward_as_tuple(m, c, e, pos);
}