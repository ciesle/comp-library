//辺、初期位置
//閉路を構成する頂点群を閉路の順に返す
//同じ回路について2度呼び出さない
vector<int>namori_circuit(vector<int>& e, int start = 0) {
	static vector<bool>used(e.size(), false);
	vector<int>ret;
	auto dfs = [&](int x, auto& dfs) ->int {
		if (used[x]) {
			return x;
		}
		used[x] = true;
		int t = dfs(e[x], dfs);
		if (~t) {
			ret.push_back(x);
			if (t != x) return t;
		}
		return -1;
		};
	dfs(start, dfs);
	reverse(ret.begin(), ret.end());
	return ret;
}