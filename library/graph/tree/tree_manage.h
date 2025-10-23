//n-th parent, lca
//distance of two points, diameter
class tree_manage {
	int n, root, log;
	vector<vector<int>>e, pa;
	vector<int>dpt;
public:
	tree_manage() {}
	tree_manage(int n, int root = 0) :n(n), root(root) {
		e.resize(n), dpt.resize(n);
		log = 0;while ((1 << log) <= n) log++;
		pa.assign(log, vector<int>(n, -1));
	}
	tree_manage(int n, vector<vector<int>>& e, int root = 0) :n(n), e(e), root(root) {
		dpt.resize(n);
		log = 0;while ((1 << log) <= n) log++;
		pa.assign(log, vector<int>(n, -1));
		build();
	}
	void add_edge(int x, int y) {
		e[x].emplace_back(y);
		e[y].emplace_back(x);
	}
	void build() {
		dpt[root] = 0;
		dfs(root, -1);
		for (int i = 1;i < log;i++)for (int j = 0;j < n;j++) {
			if (~pa[i - 1][j]) pa[i][j] = pa[i - 1][pa[i - 1][j]];
		}
	}
	int par(int x, int dist = 1) {
		for (int i = log - 1;i >= 0;i--) {
			if ((dist >> i) & 1) x = pa[i][x];
		}
		return x;
	}
	int lca(int x, int y) {
		if (dpt[x] < dpt[y]) swap(x, y);
		for (int i = log - 1;i >= 0;i--) {
			if (((dpt[x] - dpt[y]) >> i) & 1) x = pa[i][x];
		}
		if (x == y) return x;
		for (int i = log - 1;i >= 0;i--) {
			if (pa[i][x] != pa[i][y]) x = pa[i][x], y = pa[i][y];
		}
		return pa[0][x];
	}
	int dept(int x) {
		return dpt[x];
	}
	int dist(int x, int y) {
		int k = lca(x, y);
		return dpt[x] + dpt[y] - 2 * dpt[k];
	}
	//pointer of endpoint1, pointer of endpoint2
	int diameter(int* x = NULL, int* y = NULL) {
		vector<int>dept(n, 0);
		auto dfs = [&](int x, int p, int d, auto& dfs)->void {
			dept[x] = d;
			for (auto g : e[x]) if (g != p) {
				dfs(g, x, d + 1, dfs);
			}
			};
		dfs(0, -1, 0, dfs);
		int root = max_element(dept.begin(), dept.end()) - dept.begin();
		dfs(root, -1, 0, dfs);
		int root2 = max_element(dept.begin(), dept.end()) - dept.begin();
		if (x != NULL && y != NULL) *x = root, * y = root2;
		return dept[root2];
	}
private:
	void dfs(int x, int p) {
		pa[0][x] = p;
		for (auto& g : e[x]) if (g != p) {
			dpt[g] = dpt[x] + 1;
			dfs(g, x);
		}
	}
};