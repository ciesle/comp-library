#pragma once
#include<vector>
#include<tuple>
using namespace std;
/*
頂点数、辺、深さ、ある頂点のノードリストをタプルとして返す
辺の行き先の番号が-1のとき、その頂点（の先にいるノード）はない
頂点は抽象的な概念で、そこから伸びる辺がビットに対応する。
頂点の深さは、そこから伸びる辺のビットの番号に対応している（ので、葉の深さは-1になる）
ノードリストは重いので、不要なら消すのもあり
*/
template<typename T>
tuple<int, vector<array<int, 2>>, vector<int>, vector<vector<int>>>binary_trie_tree(vector<T>& s) {
	using U = typename std::make_unsigned<T>::type;
	constexpr int BITS = sizeof(U) * 8;
	int n = s.size(), m = 1;
	vector<array<int, 2>>e(1, { -1,-1 });
	vector<int>depth(1, BITS - 1);
	vector<vector<int>>pos(1);
	for (int i = 0;i < n;i++) {
		int t = 0;
		for (int j = BITS - 1; j >= 0; --j) {
			int k = (static_cast<U>(s[i]) >> j) & 1;
			if (e[t][k] < 0) {
				depth.emplace_back(depth[t] - 1);
				e.emplace_back(array{ -1, -1 });
				pos.emplace_back();
				e[t][k] = m;
				t = m++;
			}
			else t = e[t][k];
		}
		pos[t].emplace_back(i);
	}
	return forward_as_tuple(m, e, depth, pos);
}