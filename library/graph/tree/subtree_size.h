vector<int>subtree_size(const vector<vector<int>>& e, int root = 0) {
	vector<int>ret(e.size(), 0);
	auto dfs = [&](int x, int p, int d, auto& dfs) ->int {
		ret[x] = 1;
		for (auto& g : e[x]) if (g != p) {
			ret[x] += dfs(g, x, d + 1, dfs);
		}
		return ret[x];
		};
	dfs(root, -1, 0, dfs);
	return ret;
}