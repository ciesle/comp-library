/*
  <Bellman-Ford / ベルマンフォード法>
    - 負辺を含む重み付き有向グラフに対して，単一始点最短路を求める
    - 始点から到達可能な負閉路が存在する場合，その負閉路の影響を受ける頂点も判定する
    - グラフ型は WGraph<T> を用いる

    [前提となるグラフ型]
        template <typename T>
        using WGraph = vector<vector<Edge<T>>>;

        Edge<T> は次の情報を持つことを想定する:
            - int from
            - int to
            - int index
            - T cost

        ただし，隣接リスト g[v] に入っている辺 e について，
        e.from が未設定でも，現在の頂点 v を始点として扱う．

    [問題]
        - 重み付き有向グラフ G と始点 s が与えられる
        - s から各頂点 v への最短距離を求める
        - 負辺があってもよい
        - ただし，s から到達可能な負閉路の影響を受ける頂点については，
          最短距離は定義されず，-∞ とみなす

    [実装/関数]
        - BellmanFordResult<T> bellman_ford(WGraph<T> g, int start, T INF)

            始点 start から Bellman-Ford 法を行う

            返り値 res について:
                res.dist[v]
                    start から v への距離
                    到達不能なら INF

                res.prev[v]
                    最短路上で v の直前に来る頂点
                    存在しなければ -1

                res.prev_edge[v]
                    最短路上で v に入る辺の index
                    存在しなければ -1

                res.negative[v]
                    v が始点から到達可能な負閉路の影響を受けるなら true

                res.has_negative_cycle
                    始点から到達可能な負閉路が存在すれば true

        - vector<int> res.restore_path(int t)

            start から t への最短路を頂点列で返す

            ただし，
                - t が到達不能
                - t が負閉路の影響を受ける
            場合は空配列を返す

        - bool res.reachable(int v)

            start から v に到達可能なら true

        - bool res.has_finite_distance(int v)

            start から v に到達可能で，かつ負閉路の影響を受けないなら true

    [計算時間]
        - O(VE)

    [メモリ]
        - O(V)

    [要件]
        - T は以下を満たすこと
            - T(0) が使える
            - +, <, == が使える
            - 十分大きい INF を指定できる

        - 辺の重みは負でもよい
        - 有向グラフとして扱う
          無向辺を入れたい場合は両方向に辺を追加する

    [備考]
        - Dijkstra 法と異なり，負辺があっても動作する
        - ただし，負閉路の影響を受ける頂点の最短距離は定義されない
        - negative[v] == true のとき，v への距離は -∞ とみなす
        - dist[v] の値自体は，negative[v] == true の場合には信用しないこと
        - INF は十分大きく取ること
          long long なら 4e18 程度を使うことが多い

    [使用例]
        int n, m;
        cin >> n >> m;

        WGraph<long long> g(n);

        for (int i = 0; i < m; i++) {
            int u, v;
            long long w;
            cin >> u >> v >> w;
            g[u].push_back(Edge<long long>(u, v, i, w));
        }

        const long long INF = 4e18;

        auto res = bellman_ford<long long>(g, 0, INF);

        for (int v = 0; v < n; v++) {
            if (!res.reachable(v)) {
                cout << "INF\n";
            } else if (res.negative[v]) {
                cout << "-INF\n";
            } else {
                cout << res.dist[v] << '\n';
            }
        }

    [verified at]
        -
*/

template <typename T>
struct BellmanFordResult {
    T INF;
    vector<T> dist;
    vector<int> prev;
    vector<int> prev_edge;
    vector<char> negative;
    bool has_negative_cycle;

    bool reachable(int v) const {
        return dist[v] != INF;
    }

    bool has_finite_distance(int v) const {
        return dist[v] != INF && !negative[v];
    }

    vector<int> restore_path(int t) const {
        if (!has_finite_distance(t)) return {};

        vector<int> path;

        for (int v = t; v != -1; v = prev[v]) {
            path.push_back(v);
        }

        reverse(path.begin(), path.end());
        return path;
    }
};

template <typename T>
BellmanFordResult<T> bellman_ford(const WGraph<T> &g, int start, T INF) {
    int n = (int)g.size();
    assert(0 <= start && start < n);

    vector<T> dist(n, INF);
    vector<int> prev(n, -1);
    vector<int> prev_edge(n, -1);

    dist[start] = T(0);

    // 通常の緩和を n-1 回行う
    for (int iter = 0; iter < n - 1; iter++) {
        bool updated = false;

        for (int v = 0; v < n; v++) {
            if (dist[v] == INF) continue;

            for (const auto &e : g[v]) {
                int to = e.to;
                T nd = dist[v] + e.cost;

                if (nd < dist[to]) {
                    dist[to] = nd;
                    prev[to] = v;
                    prev_edge[to] = e.index;
                    updated = true;
                }
            }
        }

        if (!updated) break;
    }

    // 負閉路の影響を受ける頂点を検出する
    vector<char> negative(n, false);

    for (int iter = 0; iter < n; iter++) {
        for (int v = 0; v < n; v++) {
            if (dist[v] == INF && !negative[v]) continue;

            for (const auto &e : g[v]) {
                int to = e.to;

                if (negative[v]) {
                    negative[to] = true;
                } else {
                    T nd = dist[v] + e.cost;

                    if (nd < dist[to]) {
                        negative[to] = true;
                    }
                }
            }
        }
    }

    bool has_negative_cycle = false;

    for (int v = 0; v < n; v++) {
        if (negative[v]) {
            has_negative_cycle = true;
            break;
        }
    }

    return BellmanFordResult<T>{
        INF,
        dist,
        prev,
        prev_edge,
        negative,
        has_negative_cycle
    };
}