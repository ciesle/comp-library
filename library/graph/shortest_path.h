/*
n, edge, cost(from dijkstra), start, end
ret: shortest path from start to end
*/
vector<int>shortest_path(int n, vector<vector<edg>>& e, vector<long long>& cost, int s, int t) {
	vector<int>ret;
	if (cost[t] == 1e18) return ret;
	vector<vector<edg>>f(n);
	for (int i = 0;i < n;i++)for (const auto& [to, cst] : e[i]) {
		f[to].emplace_back(edg{ i,cst });
	}
	int cur = t;
	while (cur != s) {
		ret.emplace_back(cur);
		for (auto& [to, cst] : e[cur]) {
			if (cost[to] + cst == cost[cur]) {
				cur = to;
				break;
			}
		}
	}
	ret.emplace_back(cur);
	reverse(ret.begin(), ret.end());
	return ret;
}