//n, edges
//ret: roots, edge(to children), edge(to ancestors except for parent)
tuple<vector<int>, vector<vector<int>>, vector<vector<int>>>dfs_tree(int n, vector<vector<int>>& e, int default_root = -1) {
	vector<int>dept(n, -1);
	vector<int>roots;
	vector<vector<int>>f(n), rev(n);
	auto dfs = [&](int x, int p, auto& dfs)->void {
		if (~p) dept[x] = dept[p] + 1;
		else dept[x] = 0;
		for (auto g : e[x]) if (g != p) {
			if (dept[g] < 0) {
				f[x].push_back(g);
				dfs(g, x, dfs);
			}
			else if (dept[g] < dept[x]) {
				rev[x].push_back(g);
			}
		}
		};
	if (~default_root) {
		roots.push_back(default_root);
		dfs(default_root, -1, dfs);
	}
	for (int i = 0;i < n;i++) if (dept[i] < 0) {
		roots.push_back(i);
		dfs(i, -1, dfs);
	}
	return forward_as_tuple(roots, f, rev);
}