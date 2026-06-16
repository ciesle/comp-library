vector<int>tree_parent(const vector<vector<int>>& e, int root = 0) {
	vector<int>ret(e.size(), -1);
	auto dfs = [&](int x, int p, auto& dfs) ->void {
		ret[x] = p;
		for (auto& g : e[x]) if (g != p) {
			dfs(g, x, dfs);
		}
		};
	dfs(root, -1, dfs);
	return ret;
}