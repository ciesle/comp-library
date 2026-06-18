/*
  <マトロイド交差問題>
    - (重み付き)マトロイド交差問題を解く

    [実装/関数]
        - vector<bool> I = MatroidIntersection(Matroid g1, Matroid g2, vector<ll> weights) : 二つのマトロイドを
            渡して解く(重み配列weightsを渡すと重み付きを解く)
        - 各要素が含まれているかを持つ vector<bool> を返す

        - Matroid : struct(class)として実装する．以下のような要素が必要
            - int size() : 全体の要素数を返す
            - void set(vector<bool> I) : 現在の独立集合I(vector<bool>で与えられる)に対して内部状態を構築
            - vector<int> circuit(int e) : Iに要素eを加えた集合に含まれるサーキットの点集合をvector<int>で得る
    
    [計算時間]
        - 重みなし : O(nr(n+c)) (rはランクのmax, cはサーキットクエリにかかる時間)
        - 重みあり : O(nr(n^2+c)) (O(nr(r+c+logn))に改善可能)
    
    [備考]
        - 
    
    [depends on]
        - shortest-path(このコードに記載)

    [参照]
        - https://hitonanode.github.io/cplib-cpp/combinatorial_opt/matroid_intersection.hpp.html

    [verified at]
        - https://codeforces.com/contest/1284/my
        
*/


template <typename T, T INFV = std::numeric_limits<T>::max() / 2, int INVALID = -1>
struct shortest_path {
    int V, E;
    bool single_positive_weight;
    T wmin, wmax;

    std::vector<std::pair<int, T>> tos;
    std::vector<int> head;
    std::vector<std::tuple<int, int, T>> edges;

    void build_() {
        if (int(tos.size()) == E and int(head.size()) == V + 1) return;
        tos.resize(E);
        head.assign(V + 1, 0);
        for (const auto &e : edges) ++head[std::get<0>(e) + 1];
        for (int i = 0; i < V; ++i) head[i + 1] += head[i];
        auto cur = head;
        for (const auto &e : edges) {
            tos[cur[std::get<0>(e)]++] = std::make_pair(std::get<1>(e), std::get<2>(e));
        }
    }

    shortest_path(int V = 0) : V(V), E(0), single_positive_weight(true), wmin(0), wmax(0) {}
    void add_edge(int s, int t, T w) {
        assert(0 <= s and s < V);
        assert(0 <= t and t < V);
        edges.emplace_back(s, t, w);
        ++E;
        if (w > 0 and wmax > 0 and wmax != w) single_positive_weight = false;
        wmin = std::min(wmin, w);
        wmax = std::max(wmax, w);
    }

    void add_bi_edge(int u, int v, T w) {
        add_edge(u, v, w);
        add_edge(v, u, w);
    }

    std::vector<T> dist;
    std::vector<int> prev;

    // Dijkstra algorithm
    // - Requirement: wmin >= 0
    // - Complexity: O(E log E)
    using Pque = std::priority_queue<std::pair<T, int>, std::vector<std::pair<T, int>>,
                                     std::greater<std::pair<T, int>>>;
    template <class Heap = Pque> void dijkstra(int s, int t = INVALID) {
        assert(0 <= s and s < V);
        build_();
        dist.assign(V, INFV);
        prev.assign(V, INVALID);
        dist[s] = 0;
        Heap pq;
        pq.emplace(0, s);
        while (!pq.empty()) {
            T d;
            int v;
            std::tie(d, v) = pq.top();
            pq.pop();
            if (t == v) return;
            if (dist[v] < d) continue;
            for (int e = head[v]; e < head[v + 1]; ++e) {
                const auto &nx = tos[e];
                T dnx = d + nx.second;
                if (dist[nx.first] > dnx) {
                    dist[nx.first] = dnx, prev[nx.first] = v;
                    pq.emplace(dnx, nx.first);
                }
            }
        }
    }

    // Dijkstra algorithm
    // - Requirement: wmin >= 0
    // - Complexity: O(V^2 + E)
    void dijkstra_vquad(int s, int t = INVALID) {
        assert(0 <= s and s < V);
        build_();
        dist.assign(V, INFV);
        prev.assign(V, INVALID);
        dist[s] = 0;
        std::vector<char> fixed(V, false);
        while (true) {
            int r = INVALID;
            T dr = INFV;
            for (int i = 0; i < V; i++) {
                if (!fixed[i] and dist[i] < dr) r = i, dr = dist[i];
            }
            if (r == INVALID or r == t) break;
            fixed[r] = true;
            int nxt;
            T dx;
            for (int e = head[r]; e < head[r + 1]; ++e) {
                std::tie(nxt, dx) = tos[e];
                if (dist[nxt] > dist[r] + dx) dist[nxt] = dist[r] + dx, prev[nxt] = r;
            }
        }
    }

    // Bellman-Ford algorithm
    // - Requirement: no negative loop
    // - Complexity: O(VE)
    bool bellman_ford(int s, int nb_loop) {
        assert(0 <= s and s < V);
        build_();
        dist.assign(V, INFV), prev.assign(V, INVALID);
        dist[s] = 0;
        for (int l = 0; l < nb_loop; l++) {
            bool upd = false;
            for (int v = 0; v < V; v++) {
                if (dist[v] == INFV) continue;
                for (int e = head[v]; e < head[v + 1]; ++e) {
                    const auto &nx = tos[e];
                    T dnx = dist[v] + nx.second;
                    if (dist[nx.first] > dnx) dist[nx.first] = dnx, prev[nx.first] = v, upd = true;
                }
            }
            if (!upd) return true;
        }
        return false;
    }

    // Bellman-ford algorithm using deque
    // - Requirement: no negative loop
    // - Complexity: O(VE)
    void spfa(int s) {
        assert(0 <= s and s < V);
        build_();
        dist.assign(V, INFV);
        prev.assign(V, INVALID);
        dist[s] = 0;
        std::deque<int> q;
        std::vector<char> in_queue(V);
        q.push_back(s), in_queue[s] = 1;
        while (!q.empty()) {
            int now = q.front();
            q.pop_front(), in_queue[now] = 0;
            for (int e = head[now]; e < head[now + 1]; ++e) {
                const auto &nx = tos[e];
                T dnx = dist[now] + nx.second;
                int nxt = nx.first;
                if (dist[nxt] > dnx) {
                    dist[nxt] = dnx;
                    if (!in_queue[nxt]) {
                        if (q.size() and dnx < dist[q.front()]) { // Small label first optimization
                            q.push_front(nxt);
                        } else {
                            q.push_back(nxt);
                        }
                        prev[nxt] = now, in_queue[nxt] = 1;
                    }
                }
            }
        }
    }

    // 01-BFS
    // - Requirement: all weights must be 0 or w (positive constant).
    // - Complexity: O(V + E)
    void zero_one_bfs(int s, int t = INVALID) {
        assert(0 <= s and s < V);
        build_();
        dist.assign(V, INFV), prev.assign(V, INVALID);
        dist[s] = 0;
        std::vector<int> q(V * 4);
        int ql = V * 2, qr = V * 2;
        q[qr++] = s;
        while (ql < qr) {
            int v = q[ql++];
            if (v == t) return;
            for (int e = head[v]; e < head[v + 1]; ++e) {
                const auto &nx = tos[e];
                T dnx = dist[v] + nx.second;
                if (dist[nx.first] > dnx) {
                    dist[nx.first] = dnx, prev[nx.first] = v;
                    if (nx.second) {
                        q[qr++] = nx.first;
                    } else {
                        q[--ql] = nx.first;
                    }
                }
            }
        }
    }

    // Dial's algorithm
    // - Requirement: wmin >= 0
    // - Complexity: O(wmax * V + E)
    void dial(int s, int t = INVALID) {
        assert(0 <= s and s < V);
        build_();
        dist.assign(V, INFV), prev.assign(V, INVALID);
        dist[s] = 0;
        std::vector<std::vector<std::pair<int, T>>> q(wmax + 1);
        q[0].emplace_back(s, dist[s]);
        int ninq = 1;

        int cur = 0;
        T dcur = 0;
        for (; ninq; ++cur, ++dcur) {
            if (cur == wmax + 1) cur = 0;
            while (!q[cur].empty()) {
                int v = q[cur].back().first;
                T dnow = q[cur].back().second;
                q[cur].pop_back(), --ninq;
                if (v == t) return;
                if (dist[v] < dnow) continue;

                for (int e = head[v]; e < head[v + 1]; ++e) {
                    const auto &nx = tos[e];
                    T dnx = dist[v] + nx.second;
                    if (dist[nx.first] > dnx) {
                        dist[nx.first] = dnx, prev[nx.first] = v;
                        int nxtcur = cur + int(nx.second);
                        if (nxtcur >= int(q.size())) nxtcur -= q.size();
                        q[nxtcur].emplace_back(nx.first, dnx), ++ninq;
                    }
                }
            }
        }
    }

    // Solver for DAG
    // - Requirement: graph is DAG
    // - Complexity: O(V + E)
    bool dag_solver(int s) {
        assert(0 <= s and s < V);
        build_();
        dist.assign(V, INFV), prev.assign(V, INVALID);
        dist[s] = 0;
        std::vector<int> indeg(V, 0);
        std::vector<int> q(V * 2);
        int ql = 0, qr = 0;
        q[qr++] = s;
        while (ql < qr) {
            int now = q[ql++];
            for (int e = head[now]; e < head[now + 1]; ++e) {
                const auto &nx = tos[e];
                ++indeg[nx.first];
                if (indeg[nx.first] == 1) q[qr++] = nx.first;
            }
        }
        ql = qr = 0;
        q[qr++] = s;
        while (ql < qr) {
            int now = q[ql++];
            for (int e = head[now]; e < head[now + 1]; ++e) {
                const auto &nx = tos[e];
                --indeg[nx.first];
                if (dist[nx.first] > dist[now] + nx.second)
                    dist[nx.first] = dist[now] + nx.second, prev[nx.first] = now;
                if (indeg[nx.first] == 0) q[qr++] = nx.first;
            }
        }
        return *max_element(indeg.begin(), indeg.end()) == 0;
    }

    // Retrieve a sequence of vertex ids that represents shortest path [s, ..., goal]
    // If not reachable to goal, return {}
    std::vector<int> retrieve_path(int goal) const {
        assert(int(prev.size()) == V);
        assert(0 <= goal and goal < V);
        if (dist[goal] == INFV) return {};
        std::vector<int> ret{goal};
        while (prev[goal] != INVALID) {
            goal = prev[goal];
            ret.push_back(goal);
        }
        std::reverse(ret.begin(), ret.end());
        return ret;
    }

    void solve(int s, int t = INVALID) {
        if (wmin >= 0) {
            if (single_positive_weight) {
                zero_one_bfs(s, t);
            } else if (wmax <= 10) {
                dial(s, t);
            } else {
                if ((long long)V * V < (E << 4)) {
                    dijkstra_vquad(s, t);
                } else {
                    dijkstra(s, t);
                }
            }
        } else {
            bellman_ford(s, V);
        }
    }

    // Warshall-Floyd algorithm
    // - Requirement: no negative loop
    // - Complexity: O(E + V^3)
    std::vector<std::vector<T>> floyd_warshall() {
        build_();
        std::vector<std::vector<T>> dist2d(V, std::vector<T>(V, INFV));
        for (int i = 0; i < V; i++) {
            dist2d[i][i] = 0;
            for (const auto &e : edges) {
                int s = std::get<0>(e), t = std::get<1>(e);
                dist2d[s][t] = std::min(dist2d[s][t], std::get<2>(e));
            }
        }
        for (int k = 0; k < V; k++) {
            for (int i = 0; i < V; i++) {
                if (dist2d[i][k] == INFV) continue;
                for (int j = 0; j < V; j++) {
                    if (dist2d[k][j] == INFV) continue;
                    dist2d[i][j] = std::min(dist2d[i][j], dist2d[i][k] + dist2d[k][j]);
                }
            }
        }
        return dist2d;
    }

    void to_dot(std::string filename = "shortest_path") const {
        std::ofstream ss(filename + ".DOT");
        ss << "digraph{\n";
        build_();
        for (int i = 0; i < V; i++) {
            for (int e = head[i]; e < head[i + 1]; ++e) {
                ss << i << "->" << tos[e].first << "[label=" << tos[e].second << "];\n";
            }
        }
        ss << "}\n";
        ss.close();
        return;
    }
};

template <class Matroid1, class Matroid2, class T = int>
bool matroid_intersection_augment(Matroid1 &m1, Matroid2 &m2, std::vector<bool> &I,
                                  const std::vector<T> &weights = {}) {
    const int n = m1.size();
    assert(m2.size() == n);
    assert((int)I.size() == n);

    auto weight = [&](int e) { return weights.empty() ? T() : weights.at(e) * (n + 1); };

    const int gs = n, gt = n + 1;
    shortest_path<T> sssp(n + 2);
    m1.set(I);
    m2.set(I);
    for (int e = 0; e < n; ++e) {
        if (I.at(e)) continue;
        auto c1 = m1.circuit(e), c2 = m2.circuit(e);
        if (c1.empty()) sssp.add_edge(e, gt, T());
        for (int f : c1) {
            if (f != e) sssp.add_edge(e, f, -weight(f) + T(1));
        }
        if (c2.empty()) sssp.add_edge(gs, e, weight(e) + T(1));
        for (int f : c2) {
            if (f != e) sssp.add_edge(f, e, weight(e) + T(1));
        }
    }
    sssp.solve(gs, gt);

    if (auto aug_path = sssp.retrieve_path(gt); aug_path.empty()) {
        return false;
    } else {
        for (auto e : aug_path) {
            if (e != gs and e != gt) I.at(e) = !I.at(e);
        }
        return true;
    }
}

// Minimum weight matroid intersection solver
// Algorithm based on http://dopal.cs.uec.ac.jp/okamotoy/lect/2015/matroid/
// Complexity: O(Cn^2 + n^3) (C : circuit query, non-weighted)
template <class Matroid1, class Matroid2, class T = int>
std::vector<bool>
MatroidIntersection(Matroid1 matroid1, Matroid2 matroid2, std::vector<T> weights = {}) {
    const int n = matroid1.size();
    assert(matroid2.size() == n);
    assert(weights.empty() or (int) weights.size() == n);

    std::vector<bool> I(n);

    if (weights.empty()) {
        matroid1.set(I);
        matroid2.set(I);
        for (int e = 0; e < n; ++e) {
            if (matroid1.circuit(e).empty() and matroid2.circuit(e).empty()) {
                I.at(e) = true;
                matroid1.set(I);
                matroid2.set(I);
            }
        }
    }

    while (matroid_intersection_augment(matroid1, matroid2, I, weights)) {}
    return I;
}