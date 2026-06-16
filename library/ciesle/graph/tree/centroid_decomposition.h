/*
- n: vertex size
- e: edge vector
- ret val: root, edge vector
*/
tuple<int, vector<vector<int>>> centroid_decomposition(int n, vector<vector<int>>& e) {
	int root;
	vector<vector<int>>f(n);
	vector<int>size(n, 0);
	vector<bool>used(n, false);
	auto one_centroid = [&](int r)->int {
		int cnt = 0;
		auto dfs = [&](int x, int p, auto& dfs)->int {
			size[x] = 1, cnt++;
			for (auto g : e[x]) if (g != p && !used[g]) {
				size[x] += dfs(g, x, dfs);
			}
			return size[x];
			};
		dfs(r, -1, dfs);
		auto dfs2 = [&](int x, int p, auto& dfs2)->int {
			int ret = x;
			for (auto g : e[x])if (g != p && !used[g]) {
				if (size[g] > cnt / 2) {
					ret = dfs2(g, x, dfs2);
				}
			}
			return ret;
			};
		return dfs2(r, -1, dfs2);
		};

	auto rec = [&](int x, auto& rec)->int {
		int t = one_centroid(x);
		used[t] = true;
		for (auto g : e[t])if (!used[g]) {
			f[t].push_back(rec(g, rec));
		}
		return t;
		};
	root = rec(0, rec);
	return forward_as_tuple(root, f);
}