//n, roots, edges, edges (from dfs_tree())
//ret: vertex order, low_link
//自由度が欲しい時はこちらを使う。高速に関節点が欲しい時はbridgesを使うべき。
tuple<vector<int>, vector<int>>low_link(int n, vector<int>& roots, vector<vector<int>>& f, vector<vector<int>>& rev) {
	vector<int>low(n), ord(n);
	int count = 0;
	auto dfs = [&](int x, auto& dfs)->void {
		ord[x] = low[x] = count++;
		for (auto g : f[x]) {
			dfs(g, dfs);
			low[x] = min(low[x], low[g]);
		}
		for (auto g : rev[x]) {
			low[x] = min(low[x], ord[g]);
		}
		};
	for (auto i : roots) {
		dfs(i, dfs);
	}
	return forward_as_tuple(ord, low);
}