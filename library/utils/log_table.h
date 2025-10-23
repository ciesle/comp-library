/*
in: n: max_size
ret: log table of 1 <= i <= n
*/
vector<int>log_table(int n) {
	int m = 0;
	while ((1 << (m + 1)) <= n) m++;
	vector<int>ret(n + 1, -1);
	for (int i = 0;i <= m;i++) {
		for (int j = (1 << i);j < (1 << (i + 1)) && j <= n;j++) {
			ret[j] = i;
		}
	}
	return ret;
}