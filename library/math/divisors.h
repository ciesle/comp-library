vector<vector<int>>divisors(int n) {
	vector<vector<int>>ret(n + 1);
	for (int i = 1;i <= n;i++) {
		for (int j = 1;i * j <= n;j++) {
			ret[i * j].emplace_back(i);
		}
		sort(ret[i].begin(), ret[i].end());
	}
	return ret;
}