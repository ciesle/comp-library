//in, out
//部分木としてのオイラーツアー
tuple<vector<int>, vector<int>>euler_tour(int n, vector<vector<int>>& e, int root = 0) {
	int cnt = 0;
	vector<int>in(e.size()), out(e.size());
	auto dfs = [&](int x, int p, auto& dfs)->void {
		in[x] = cnt++;
		for (auto g : e[x]) if (g != p) {
			dfs(g, x, dfs);
		}
		out[x] = cnt;
		};
	dfs(root, -1, dfs);
	return forward_as_tuple(in, out);
}