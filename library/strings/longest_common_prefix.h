//common length of s[sa[i]...], s[sa[i+1]...]
vector<int>longest_common_prefix(string& s, vector<int>& sa) {
	int n = s.size();
	vector<int>rank(sa.size()), lcp(n);
	for (int i = 0;i <= n;i++) rank[sa[i]] = i;
	for (int i = 0, h = 0;i < n;i++) {
		int k = sa[rank[i] - 1];
		if (h > 0) h--;
		for (;k + h < n && i + h < n && s[k + h] == s[i + h];h++);
		lcp[rank[i] - 1] = h;
	}
	return lcp;
}