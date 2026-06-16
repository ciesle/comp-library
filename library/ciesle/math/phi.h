vector<int>phi(int n) {
	vector<int>ret(n + 1);
	for (int i = 0;i <= n;i++) ret[i] = i;
	for (int i = 2;i <= n;i++) {
		if (ret[i] == i) {
			for (int j = 1;i * j <= n;j++) {
				ret[i * j] /= i;
				ret[i * j] *= i - 1;
			}
		}
	}
	return ret;
}