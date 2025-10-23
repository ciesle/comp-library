bool is_dag(int n, vector<vector<int>>& e) {
	queue<int>q;
	vector<int>cnt(n);
	for (int i = 0;i < n;i++)for (auto& g : e[i]) {
		cnt[g]++;
	}
	for (int i = 0;i < n;i++) if (!cnt[i]) q.push(i);
	int sum = 0;
	while (!q.empty()) {
		int t = q.front();q.pop();
		sum++;
		for (auto& g : e[t]) {
			cnt[g]--;
			if (!cnt[g]) q.push(g);
		}
	}
	return sum == n;
}