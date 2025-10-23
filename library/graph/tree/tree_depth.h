vector<int>tree_depth(const vector<vector<int>>& e, int root = 0) {
	vector<int>ret(e.size(), 0);
	auto dfs = [&](int x, int p, int d, auto& dfs) ->void {
		ret[x] = d;
		for (auto& g : e[x]) if (g != p) {
			dfs(g, x, d + 1, dfs);
		}
		};
	dfs(root, -1, 0, dfs);
	return ret;
}