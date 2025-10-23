vector<vector<long long>> pascal_triangle(int n, long long m) {
	vector<vector<long long>>table(n + 1, vector<long long>(n + 1));
	table[0][0] = table[1][0] = table[1][1] = 1;
	for (int i = 2;i <= n;i++) {
		table[i][0] = 1;
		for (int j = 1;j <= i;j++) {
			(table[i][j] = table[i - 1][j - 1] + table[i - 1][j]) %= m;
		}
	}
	return table;
}