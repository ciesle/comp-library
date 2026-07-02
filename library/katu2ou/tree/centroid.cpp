/* 木の重心を求める
 * https://ferin-tech.hatenablog.com/entry/2020/03/06/162311
 * 
 * - その頂点を取り除いた時にできる部分木の要素数が元の半分以下になるもの
 * - 重心は1個か2個必ず存在する
 * 
 * - アルゴリズム O(n)
 *      - 適当な木の頂点を根とする
 *      - 各頂点を根とする部分木のサイズを取得する
 *      - (任意のvの子ノードの部分木サイズ) <= n/2 かつ，
 *         n - (vの部分木サイズ) <= n/2
 *         を満たすようなvを列挙する
 * 
 * - 列の中心の代わりに木の重心を考えると列に対する手法を木に応用できる(重心分解)
 * 
 * 
 */

struct TreeCentroid {
    int n;
    vector<vector<int>> g;
    vector<int> subtree_size;
    vector<int> centroids;

    TreeCentroid(int n_) : n(n_), g(n_), subtree_size(n_, 0) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs(int v, int p) {
        subtree_size[v] = 1;

        // v を取り除いたときにできる成分の最大サイズ
        int max_component_size = 0;

        for (int to : g[v]) {
            if (to == p) continue;

            dfs(to, v);

            subtree_size[v] += subtree_size[to];
            max_component_size = max(max_component_size, subtree_size[to]);
        }

        // 親側の成分サイズ
        int parent_side_size = n - subtree_size[v];
        max_component_size = max(max_component_size, parent_side_size);

        if (max_component_size <= n / 2) {
            centroids.push_back(v);
        }
    }

    vector<int> build() {
        centroids.clear();
        if (n == 0) return centroids;

        dfs(0, -1);
        return centroids;
    }
};