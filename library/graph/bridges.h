//高速に関節点、橋が欲しい時に使う
class LowLink {
	int n;
	vector<vector<int>>e;
	void dfs(int x, int p, int& cnt) {
		par[x] = p;
		ord[x] = low[x] = cnt++;
		int child = 0;
		for (auto& g : e[x]) {
			if (ord[g] < 0) {
				child++;
				dfs(g, x, cnt);
				low[x] = min(low[x], low[g]);
				if (~p && ord[x] <= low[g]) art_child[x]++;
				if (ord[x] < low[g]) bridge.insert(make_pair(min(x, g), max(x, g)));
			}
			else if (g != p) {
				low[x] = min(low[x], ord[g]);
			}
		}
		if (p < 0) art_child[x] = child - 1;
		if (art_child[x] > 0) {
			art.push_back(x);
		}
	}
public:
	vector<int>par, ord, low, art, art_child;
	//parent, order, lowlink, 関節点(削除すると成分が*増える*頂点), 削除すると増える成分の数（子供が0な親の時は減る）
	set<pair<int, int>>bridge;
	//橋の一覧
	LowLink() {}
	LowLink(int n, vector<vector<int>>e) :n(n), e(e), par(n, -1), ord(n, -1), low(n), art_child(n) {
		int cnt = 0;
		for (int i = 0;i < n;i++) {
			if (ord[i] < 0) {
				dfs(i, -1, cnt);
			}
		}
	}
};