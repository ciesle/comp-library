vector<int>z_algorithm(string& s) {
	int n = s.size();
	int i = 1, j = 0;
	vector<int>ret(n);
	while (i < n) {
		while (i + j < n && s[j] == s[i + j]) j++;
		ret[i] = j;
		if (j == 0) { i++; continue; }
		int k = 1;
		while (i + k < n && k + ret[k] < j) ret[i + k] = ret[k], k++;
		i += k;j -= k;
	}
	return ret;
}