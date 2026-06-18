/*
  <euler-tour>

    [実装/関数]
        - EulerTour et(vector<vector<int>> g, int root = 0)

        - vector<int> tin : 頂点vを初めて訪れた時刻(0,1,2,3,...,n-1)
        - vector<int> tout : 頂点vを抜けた直後の時刻(抜けた時のtinの値+1)
        - vector<int> euler : DFSの訪問順(サイズ2n-1 : 葉に対する値は1回のみ含む)
        - bool is_ancestor(int a, int b) : aがbの祖先であるか
        - int lca(int u, int v) : u,vの最小共通祖先
        - int dist(int u, int v) : uとvの辺数距離
        - int subtree_size(int v) : vを根とする部分木の頂点数

    [計算時間]
        - 
    
    [備考]
        - 
    
    [参照]
        - chatgpt

    [verified at]
        - 
    
    [使用例]
        
        
*/
struct EulerTour {
    int n, LOG;
    std::vector<int> depth, tin, tout, euler, first;
    std::vector<std::array<int, 2>> ranges;          // [tin, tout)
    std::vector<std::vector<int>> up;                // up[k][v]

    explicit EulerTour(const std::vector<std::vector<int>>& g,
                          int root = 0) : n(g.size()) {

        LOG = std::bit_width(n);
        depth.assign(n, 0);
        tin.assign(n, 0);
        tout.assign(n, 0);
        first.assign(n, -1);
        up.assign(LOG, std::vector<int>(n, -1));
        ranges.assign(n, {0,0});
        euler.reserve(2*n-1);

        int timer = 0;
        auto dfs = [&](auto&& self, int v, int p)->void {
            tin[v] = timer++;
            first[v] = euler.size();
            euler.push_back(v);
            up[0][v] = p;
            for (int k=1; k<LOG; ++k)
                up[k][v] = (up[k-1][v] == -1) ? -1 : up[k-1][ up[k-1][v] ];

            for (int to : g[v]) if (to != p){
                depth[to] = depth[v] + 1;
                self(self, to, v);
                euler.push_back(v);                   // 戻り
            }
            tout[v] = timer;
            ranges[v] = { tin[v], tout[v] };
        };
        dfs(dfs, root, -1);
    }

    /* ---- basic helpers ---- */
    bool is_ancestor(int a, int b) const {           // a が b の祖先？
        return tin[a] <= tin[b] && tout[b] <= tout[a];
    }
    int lca(int u, int v) const {
        if (is_ancestor(u, v)) return u;
        if (is_ancestor(v, u)) return v;
        for (int k = LOG-1; k >= 0; --k)
            if (up[k][u] != -1 && !is_ancestor(up[k][u], v))
                u = up[k][u];
        return up[0][u];
    }
    int dist(int u, int v) const {
        int w = lca(u,v);
        return depth[u] + depth[v] - 2*depth[w];
    }
    int subtree_size(int v) const { return tout[v] - tin[v]; }   // 頂点数
    int preorder_idx(int v) const { return tin[v]; }
    int postorder_idx(int v)  const { return tout[v]-1; }
};