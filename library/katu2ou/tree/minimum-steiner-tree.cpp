/*
  <Minimum Steiner Tree>
    - 重み付き無向グラフ上で，指定された terminal 集合をすべて含む
      最小コストの連結部分グラフを求める
    - 辺コストは非負である必要がある
    - terminal の個数 K が小さい場合に使う

    [計算量]
        - O(3^K * N + 2^K * M log N)
        - K = terminal.size()

    [実装/関数]
        - SteinerTree<T> st(g, terminals)
            : グラフ g と terminal 集合 terminals から作る

        - typename SteinerTree<T>::Result st.solve()
            : 最小シュタイナー木を求める

    [Result]
        - bool connected
            : terminal をすべて連結できるなら true
        - T cost
            : 最小コスト
        - int root
            : DP 上で選ばれた根
        - vector<int> vertices
            : 復元されたシュタイナー木に含まれる頂点集合
        - vector<int> edge_indices
            : 復元されたシュタイナー木に含まれる辺番号
              ※ Edge.index >= 0 を設定している場合のみ有効
        - vector<pair<int,int>> edges
            : 復元された辺の端点集合
*/

template<typename T>
struct SteinerTree {
    struct Prev {
        // type = 0 : 初期状態
        // type = 1 : merge
        // type = 2 : edge
        unsigned char type;
        int sub;
        int pv;
        int eid;
        int from, to;

        Prev() : type(0), sub(-1), pv(-1), eid(-1), from(-1), to(-1) {}
    };

    struct Result {
        bool connected;
        T cost;
        int root;
        vector<int> vertices;
        vector<int> edge_indices;
        vector<pair<int,int>> edges;
    };

    int n;
    int k;
    const WGraph<T>& g;
    vector<int> terminals;
    T INF;

    vector<vector<T>> dp;
    vector<vector<Prev>> pre;

    SteinerTree(const WGraph<T>& _g, vector<int> _terminals,
                T _INF = numeric_limits<T>::max() / 4)
        : n((int)_g.size()), g(_g), terminals(_terminals), INF(_INF) {

        sort(terminals.begin(), terminals.end());
        terminals.erase(unique(terminals.begin(), terminals.end()), terminals.end());
        k = (int)terminals.size();
    }

    Result solve() {
        Result res;
        res.connected = false;
        res.cost = INF;
        res.root = -1;

        if (k == 0) {
            res.connected = true;
            res.cost = T(0);
            return res;
        }

        int S = 1 << k;

        dp.assign(S, vector<T>(n, INF));
        pre.assign(S, vector<Prev>(n));

        for (int i = 0; i < k; i++) {
            int v = terminals[i];
            dp[1 << i][v] = T(0);
        }

        for (int mask = 1; mask < S; mask++) {
            // 同じ頂点で部分集合同士を merge する
            for (int sub = (mask - 1) & mask; sub > 0; sub = (sub - 1) & mask) {
                int other = mask ^ sub;

                // 同じ分割を2回見ないため
                if (sub > other) continue;

                for (int v = 0; v < n; v++) {
                    if (dp[sub][v] == INF || dp[other][v] == INF) continue;

                    T cand = dp[sub][v] + dp[other][v];
                    if (cand < dp[mask][v]) {
                        dp[mask][v] = cand;
                        pre[mask][v].type = 1;
                        pre[mask][v].sub = sub;
                    }
                }
            }

            // 現在の mask について，多始点 Dijkstra
            dijkstra(mask);
        }

        int full = S - 1;

        for (int v = 0; v < n; v++) {
            if (dp[full][v] < res.cost) {
                res.cost = dp[full][v];
                res.root = v;
            }
        }

        if (res.cost == INF) {
            return res;
        }

        res.connected = true;
        restore(full, res.root, res);

        sort(res.vertices.begin(), res.vertices.end());
        res.vertices.erase(unique(res.vertices.begin(), res.vertices.end()), res.vertices.end());

        sort(res.edge_indices.begin(), res.edge_indices.end());
        res.edge_indices.erase(unique(res.edge_indices.begin(), res.edge_indices.end()), res.edge_indices.end());

        return res;
    }

private:
    void dijkstra(int mask) {
        using P = pair<T, int>;
        priority_queue<P, vector<P>, greater<P>> pq;

        for (int v = 0; v < n; v++) {
            if (dp[mask][v] != INF) {
                pq.emplace(dp[mask][v], v);
            }
        }

        while (!pq.empty()) {
            auto [dist, v] = pq.top();
            pq.pop();

            if (dist != dp[mask][v]) continue;

            for (const auto& e : g[v]) {
                int to = e.to;
                T nd = dist + e.cost;

                if (nd < dp[mask][to]) {
                    dp[mask][to] = nd;

                    pre[mask][to].type = 2;
                    pre[mask][to].pv = v;
                    pre[mask][to].eid = e.index;
                    pre[mask][to].from = v;
                    pre[mask][to].to = to;

                    pq.emplace(nd, to);
                }
            }
        }
    }

    void restore(int full, int root, Result& res) {
        vector<char> used_v(n, false);
        unordered_set<int> used_eid;
        unordered_set<long long> used_pair;

        auto edge_key = [&](int a, int b) -> long long {
            if (a > b) swap(a, b);
            return (1LL * a << 32) ^ (unsigned int)b;
        };

        unordered_set<long long> seen_state;

        auto state_key = [&](int mask, int v) -> long long {
            return (1LL * mask << 32) ^ (unsigned int)v;
        };

        vector<pair<int,int>> st;
        st.emplace_back(full, root);

        while (!st.empty()) {
            auto [mask, v] = st.back();
            st.pop_back();

            long long sk = state_key(mask, v);
            if (seen_state.count(sk)) continue;
            seen_state.insert(sk);

            used_v[v] = true;

            const Prev& p = pre[mask][v];

            if (p.type == 0) {
                continue;
            }

            if (p.type == 1) {
                int sub = p.sub;
                int other = mask ^ sub;

                st.emplace_back(sub, v);
                st.emplace_back(other, v);
            }

            if (p.type == 2) {
                used_v[p.from] = true;
                used_v[p.to] = true;

                res.edges.emplace_back(p.from, p.to);

                if (p.eid >= 0) {
                    if (!used_eid.count(p.eid)) {
                        used_eid.insert(p.eid);
                        res.edge_indices.push_back(p.eid);
                    }
                } else {
                    // index が設定されていない場合用
                    // 多重辺を区別したい場合は Edge.index を設定すること
                    long long ek = edge_key(p.from, p.to);
                    used_pair.insert(ek);
                }

                st.emplace_back(mask, p.pv);
            }
        }

        for (int v = 0; v < n; v++) {
            if (used_v[v]) res.vertices.push_back(v);
        }
    }
};