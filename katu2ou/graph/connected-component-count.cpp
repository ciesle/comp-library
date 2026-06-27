/*
  <連結成分分解>
    - グラフを連結成分ごとに分解する
    - 無向グラフの隣接リストを受け取り，各連結成分に属する頂点集合を返す

    [概要]
        - BFS により，まだ訪れていない頂点から探索を開始する
        - 1 回の BFS で到達できる頂点全体が 1 つの連結成分になる
        - これをすべての頂点について行うことで，グラフ全体を連結成分に分解する

    [実装/関数]
        - vector<vector<int>> partitioned_graph(GT g)
            グラフ g を連結成分ごとに分解する

            g[v] は頂点 v に隣接する頂点のリストであるとする
            返り値 res は連結成分の列であり，
            res[i] は i 番目の連結成分に属する頂点集合を表す

    [計算時間]
        - O(N + M)

        ここで，
            N = 頂点数
            M = 辺数

    [要件]
        - g.size() が頂点数
        - g[v] を range-based for で走査できること
            例:
                vector<vector<int>>
                vector<set<int>>
                vector<unordered_set<int>>

        - 無向グラフとして連結成分分解したい場合，
          各辺 u-v について両方向に追加しておく必要がある

            g[u].push_back(v);
            g[v].push_back(u);

    [備考]
        - 返り値の各連結成分内の頂点順は BFS の探索順
        - 連結成分同士の順序は，最初に見つかった未訪問頂点の昇順
        - 孤立点もサイズ 1 の連結成分として返る
        - 有向グラフを渡した場合は，強連結成分分解ではなく，
          「辺の向きに従って到達できる範囲」を BFS するだけなので注意

    [verified at]
        - g++ 14.2.0, -std=gnu++23
        - 固定ケースとランダムグラフ 10000 ケースで愚直 BFS と比較

    [使用例]
        int n, m;
        cin >> n >> m;

        vector<vector<int>> g(n);

        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            g[u].push_back(v);
            g[v].push_back(u);
        }

        auto comps = partitioned_graph(g);

        cout << comps.size() << endl;

        for (auto comp : comps) {
            for (int v : comp) cout << v << ' ';
            cout << endl;
        }
*/

template <typename GT>
vector<vector<int>> partitioned_graph(const GT& g) {
    int n = (int)g.size();

    vector<int> comp_id(n, -1);
    vector<vector<int>> res;

    for (int i = 0; i < n; i++) {
        if (comp_id[i] != -1) continue;

        int id = (int)res.size();
        res.push_back(vector<int>());

        queue<int> que;
        comp_id[i] = id;
        que.push(i);

        while (!que.empty()) {
            int v = que.front();
            que.pop();

            res[id].push_back(v);

            for (auto to : g[v]) {
                if (comp_id[to] == -1) {
                    comp_id[to] = id;
                    que.push(to);
                }
            }
        }
    }

    return res;
}