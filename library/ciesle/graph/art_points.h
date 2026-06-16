//n, roots, edge(to children edge from dfs_tree()), ord, low(from low_link())
//ret: 関節点のリスト
//自由度が欲しい時はこちらを使う。高速に関節点が欲しい時はbridgesを使うべき。
vector<int>art_points(int n, vector<int>& roots, vector<vector<int>>& f, vector<int>& ord, vector<int>& low) {
	vector<bool>isroot(n, false);
	for (auto i : roots) isroot[i] = true;
	vector<int>ret;
	for (int i = 0;i < n;i++) {
		if (!isroot[i]) {
			int child = 0;
			for (auto g : f[i]) if (ord[i] <= low[g]) child++;
			if (child > 0) ret.push_back(i);
		}
		else {
			if (f[i].size() > 1) ret.push_back(i);
		}
	}
	return ret;
}