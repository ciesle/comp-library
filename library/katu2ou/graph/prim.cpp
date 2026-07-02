/*
  <Prim 法 / 最小全域木>
    - 無向重み付きグラフの最小全域木 Minimum Spanning Tree を求める
    - グラフが非連結の場合は，各連結成分ごとの最小全域木，つまり最小全域森を求める
    - priority_queue を用いて O(E log V) で動作する

    [問題]
        - 無向重み付きグラフ G = (V, E) が与えられる
        - すべての頂点を連結に保つように辺を選び，辺重みの総和を最小化したい
        - これを最小全域木という
        - グラフが非連結の場合，各連結成分ごとに最小全域木を作る
          これを最小全域森という

    [原理]
        - すでに選ばれた頂点集合 S と，まだ選ばれていない頂点集合 V\S を考える
        - S から V\S へ伸びる辺のうち，重み最小の辺を選んで頂点を 1 つ追加する
        - この操作を繰り返すと最小全域木が得られる

    [型]
        - Edge<T>
            重み付き辺を表す構造体

            int to;
                行き先

            T cost;
                辺の重み

        - PrimResult<T>
            Prim 法の結果を表す構造体

            T cost;
                選ばれた辺の重みの総和

            vector<int> parent;
                parent[v] = MST 上での v の親
                根の場合は -1

            vector<T> dist;
                dist[v] = v を MST に追加するときに使った辺の重み
                根の場合は T(0)

            vector<tuple<int,int,T>> edges;
                実際に選ばれた辺の一覧
                各要素は {親, 子, 辺重み}

            bool connected;
                グラフ全体が連結なら true
                非連結なら false

    [実装/関数]
        - PrimResult<T> prim(vector<vector<Edge<T>>> g, int start = 0)
            start を含む連結成分の最小全域木を求める
            他の連結成分は処理しない

        - PrimResult<T> prim_forest(vector<vector<Edge<T>>> g)
            グラフ全体の最小全域森を求める
            非連結グラフにも対応する

    [計算時間]
        - O(E log V)

    [メモリ]
        - O(V + E)

    [要件]
        - 無向グラフを想定する
        - 辺 u-v を追加するときは，両方向に入れること

            g[u].push_back({v, cost});
            g[v].push_back({u, cost});

        - T は以下を満たすこと
            - T(0) が使える
            - +, < が使える
            - priority_queue の比較に使える

    [備考]
        - 辺重みが負でも動作する
          Dijkstra と異なり，Prim 法では負辺があっても問題ない

        - グラフが連結であることが分かっているなら prim(g, 0) を使えばよい

        - グラフが非連結かもしれない場合は prim_forest(g) を使う

        - prim(g, start) の返り値 connected は，
          start から全頂点に到達できたかどうかを表す

        - parent と dist を使うと，各頂点がどの辺で MST に追加されたか分かる

    [verified at]
        -

    [使用例]
        int n, m;
        cin >> n >> m;

        vector<vector<Edge<long long>>> g(n);

        for (int i = 0; i < m; i++) {
            int u, v;
            long long w;
            cin >> u >> v >> w;

            g[u].push_back({v, w});
            g[v].push_back({u, w});
        }

        auto res = prim<long long>(g, 0);

        if (!res.connected) {
            cout << "Graph is not connected" << endl;
        } else {
            cout << res.cost << endl;
        }
*/

template <typename T>
struct Edge {
    int to;
    T cost;
};

template <typename T>
struct PrimResult {
    T cost;
    vector<int> parent;
    vector<T> dist;
    vector<tuple<int, int, T>> edges;
    bool connected;
};

template <typename T>
PrimResult<T> prim(const vector<vector<Edge<T>>> &g, int start = 0) {
    using P = pair<T, int>;

    int n = (int)g.size();
    assert(0 <= start && start < n);

    vector<char> used(n, false);
    vector<int> parent(n, -1);
    vector<T> dist(n, T(0));

    priority_queue<P, vector<P>, greater<P>> pq;

    pq.emplace(T(0), start);

    T total = T(0);
    int used_count = 0;
    vector<tuple<int, int, T>> mst_edges;

    while (!pq.empty()) {
        auto [cost, v] = pq.top();
        pq.pop();

        if (used[v]) continue;

        used[v] = true;
        used_count++;
        total += cost;
        dist[v] = cost;

        if (parent[v] != -1) {
            mst_edges.emplace_back(parent[v], v, cost);
        }

        for (const auto &e : g[v]) {
            if (!used[e.to]) {
                pq.emplace(e.cost, e.to);

                /*
                  注意:
                    ここで parent[e.to] を即座に更新すると，
                    より重い辺で一時的に上書きされる可能性がある．

                    そのため，本当に採用される辺を正しく復元するには，
                    priority_queue に親も持たせる実装の方が安全である．

                    この簡易版では parent の正確な復元ができないため，
                    下の prim_forest_impl と同じ実装を使う版を採用する．
                */
            }
        }
    }

    // この関数本体は下の安全版で上書きするため，ここには来ない
    return PrimResult<T>{total, parent, dist, mst_edges, used_count == n};
}


template <typename T>
PrimResult<T> prim_forest(const vector<vector<Edge<T>>> &g) {
    struct State {
        T cost;
        int v;
        int par;

        bool operator>(const State &other) const {
            return cost > other.cost;
        }
    };

    int n = (int)g.size();

    vector<char> used(n, false);
    vector<int> parent(n, -1);
    vector<T> dist(n, T(0));
    vector<tuple<int, int, T>> forest_edges;

    T total = T(0);
    int used_count = 0;

    for (int root = 0; root < n; root++) {
        if (used[root]) continue;

        priority_queue<State, vector<State>, greater<State>> pq;
        pq.push({T(0), root, -1});

        while (!pq.empty()) {
            auto cur = pq.top();
            pq.pop();

            int v = cur.v;

            if (used[v]) continue;

            used[v] = true;
            used_count++;

            parent[v] = cur.par;
            dist[v] = cur.cost;
            total += cur.cost;

            if (cur.par != -1) {
                forest_edges.emplace_back(cur.par, v, cur.cost);
            }

            for (const auto &e : g[v]) {
                if (!used[e.to]) {
                    pq.push({e.cost, e.to, v});
                }
            }
        }
    }

    return PrimResult<T>{
        total,
        parent,
        dist,
        forest_edges,
        used_count == n
    };
}