/*最大マッチを求める
n: 左側頂点の最大番号-1
m: 右側頂点の数
e: 辺(左->右)
ret: 最大マッチに辺が使われたか
*/
vector<bool>maximam_match(int n, int m, const vector<pair<int, int>>& e) {
	mf_graph<int>graph(n + m + 2);
	for (auto& g : e) {
		graph.add_edge(g.first, g.second, 1);
	}
	for (int i = 0;i < n;i++) {
		graph.add_edge(n + m, i, 1);
	}
	for (int i = 0;i < m;i++) {
		graph.add_edge(n + i, n + m + 1, 1);
	}

	graph.flow(n + m, n + m + 1);
	vector<bool>ret(e.size());
	auto edge = graph.edges();
	for (int i = 0;i < e.size();i++) {
		if (edge[i].flow == 1) ret[i] = true;
	}
	return ret;
}