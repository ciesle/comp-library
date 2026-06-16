vector<int>suffix_array(string& s) {
	int n = s.size(), k;
	vector<int>sa(n + 1), tmp(n + 1), rank(n + 1);
	auto compare = [&](int i, int j)->bool {
		if (rank[i] != rank[j]) return rank[i] < rank[j];
		return (i + k <= n ? rank[i + k] : -1) < (j + k <= n ? rank[j + k] : -1);
		};
	for (int i = 0;i <= n;i++) {
		sa[i] = i;
		rank[i] = i < n ? s[i] : -1;
	}
	for (k = 1;k <= n;k *= 2) {
		sort(sa.begin(), sa.end(), compare);
		tmp[sa[0]] = 0;
		for (int i = 1;i <= n;i++) tmp[sa[i]] = tmp[sa[i - 1]] + (compare(sa[i - 1], sa[i]) ? 1 : 0);
		rank = tmp;
	}
	return sa;
}