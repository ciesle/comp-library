/*最小点被覆
edg: 辺の一覧
match: その辺は最大マッチで使用されるか
ret: それぞれの頂点が最大点被覆に含まれるか
*/
vector<bool>minimum_vertex_cover(int n, int m, const vector<pair<int, int>>& edg, const vector<bool>& match) {
	vector<bool>used(n, false), visited(n + m, false);
	vector<vector<int>>e(n + m);
	for (int i = 0;i < edg.size();i++) {
		if (match[i]) {
			used[edg[i].first] = true;
			e[edg[i].second].emplace_back(edg[i].first);
		}
		else {
			e[edg[i].first].emplace_back(edg[i].second);
		}
	}

	for (int i = 0;i < n;i++)if (!used[i] && !visited[i]) {
		queue<int>q;
		q.push(i);
		visited[i] = true;
		while (!q.empty()) {
			int t = q.front();q.pop();
			for (auto& g : e[t]) if (!visited[g]) {
				visited[g] = true;
				q.push(g);
			}
		}
	}

	vector<bool>ret(n + m, false);
	for (int i = 0;i < n;i++) if (!visited[i]) ret[i] = true;
	for (int i = 0;i < m;i++) if (visited[n + i]) ret[n + i] = true;
	return ret;
}