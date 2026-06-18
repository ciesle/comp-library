/*
  <DM分解>
    - 頂点を，最大マッチングをなす辺に含まれるかどうかに関するグループ分けをしたような感じ?

    - 最大マッチングから，どの頂点が十分にカバーされているか，

    - 

    [実装/関数]
       - vector<pair<vector<int>,vector<int>>> dulmage_mendelsohn(int L, int R, vector<pair<int,int>> edges) :
        左側頂点集合サイズをL, 右側頂点集合サイズをRとして，edgesは辺 (u,v)　(0 <= u < L, 0 <= v < R)の集合
        - first の第i要素が指す左側頂点とsecondの第i要素が指す右側頂点の間には必ず辺が存在する
        - 返り値は必ず長さ2以上のvectorであり，最初と最後の要素のfirst,secondの各vectorは
            空である可能性がある
        - 左右辺のindexに注意(L,R未満)
        - V_0^{-}, V_K^{+} の任意の点に対して，その点を含まない最大マッチングが存在する
        - V_i に含まれる点は，任意の最大マッチングに対して，V_i に含まれる点と繋がっているかまたは
        どの点とも繋がっていないかのいずれかである
    

    [計算時間]
        - 
        - 
    
    [備考]
    
    [depends on]
        - bipartite matching, SCC (コードに記してある)

    [参照]
        - https://hitonanode.github.io/cplib-cpp/graph/dulmage_mendelsohn_decomposition.hpp.html

    [verified at]
        - https://atcoder.jp/contests/abc223/submissions/64979178
        
*/

struct BipartiteMatching {
    int V;
    std::vector<std::vector<int>> to; // Adjacency list
    std::vector<int> dist;            // dist[i] = (Distance from i'th node)
    std::vector<int> match;           // match[i] = (Partner of i'th node) or -1 (No partner)
    std::vector<int> used, vv;
    std::vector<int> color; // color of each node(checking bipartition): 0/1/-1(not determined)

    BipartiteMatching() = default;
    BipartiteMatching(int V_) : V(V_), to(V_), match(V_, -1), used(V_), color(V_, -1) {}

    void add_edge(int u, int v) {
        assert(u >= 0 and u < V and v >= 0 and v < V and u != v);
        to[u].push_back(v);
        to[v].push_back(u);
    }

    void _bfs() {
        dist.assign(V, -1);
        std::vector<int> q;
        int lq = 0;
        for (int i = 0; i < V; i++) {
            if (!color[i] and !used[i]) q.push_back(i), dist[i] = 0;
        }

        while (lq < int(q.size())) {
            int now = q[lq++];
            for (auto nxt : to[now]) {
                int c = match[nxt];
                if (c >= 0 and dist[c] == -1) q.push_back(c), dist[c] = dist[now] + 1;
            }
        }
    }

    bool _dfs(int now) {
        vv[now] = true;
        for (auto nxt : to[now]) {
            int c = match[nxt];
            if (c < 0 or (!vv[c] and dist[c] == dist[now] + 1 and _dfs(c))) {
                match[nxt] = now, match[now] = nxt;
                used[now] = true;
                return true;
            }
        }
        return false;
    }

    bool _color_bfs(int root) {
        color[root] = 0;
        std::vector<int> q{root};
        int lq = 0;
        while (lq < int(q.size())) {
            int now = q[lq++], c = color[now];
            for (auto nxt : to[now]) {
                if (color[nxt] == -1) {
                    color[nxt] = !c, q.push_back(nxt);
                } else if (color[nxt] == c) {
                    return false;
                }
            }
        }
        return true;
    }

    int solve() {
        for (int i = 0; i < V; i++) {
            if (color[i] == -1 and !_color_bfs(i)) return -1;
        }
        int ret = 0;
        while (true) {
            _bfs();
            vv.assign(V, false);
            int flow = 0;
            for (int i = 0; i < V; i++) {
                if (!color[i] and !used[i] and _dfs(i)) flow++;
            }
            if (!flow) break;
            ret += flow;
        }
        return ret;
    }

    template <class OStream> friend OStream &operator<<(OStream &os, const BipartiteMatching &bm) {
        os << "{N=" << bm.V << ':';
        for (int i = 0; i < bm.V; i++) {
            if (bm.match[i] > i) os << '(' << i << '-' << bm.match[i] << "),";
        }
        return os << '}';
    }
};


struct DirectedGraphSCC {
    int V; // # of Vertices
    std::vector<std::vector<int>> to, from;
    std::vector<int> used; // Only true/false
    std::vector<int> vs;
    std::vector<int> cmp;
    int scc_num = -1;

    DirectedGraphSCC(int V = 0) : V(V), to(V), from(V), cmp(V) {}

    void _dfs(int v) {
        used[v] = true;
        for (auto t : to[v])
            if (!used[t]) _dfs(t);
        vs.push_back(v);
    }
    void _rdfs(int v, int k) {
        used[v] = true;
        cmp[v] = k;
        for (auto t : from[v])
            if (!used[t]) _rdfs(t, k);
    }

    void add_edge(int from_, int to_) {
        assert(from_ >= 0 and from_ < V and to_ >= 0 and to_ < V);
        to[from_].push_back(to_);
        from[to_].push_back(from_);
    }

    // Detect strongly connected components and return # of them.
    // Also, assign each vertex `v` the scc id `cmp[v]` (0-indexed)
    int FindStronglyConnectedComponents() {
        used.assign(V, false);
        vs.clear();
        for (int v = 0; v < V; v++)
            if (!used[v]) _dfs(v);
        used.assign(V, false);
        scc_num = 0;
        for (int i = (int)vs.size() - 1; i >= 0; i--)
            if (!used[vs[i]]) _rdfs(vs[i], scc_num++);
        return scc_num;
    }

    // Find and output the vertices that form a closed cycle.
    // output: {v_1, ..., v_C}, where C is the length of cycle,
    //         {} if there's NO cycle (graph is DAG)
    int _c, _init;
    std::vector<int> _ret_cycle;
    bool _dfs_detectcycle(int now, bool b0) {
        if (now == _init and b0) return true;
        for (auto nxt : to[now])
            if (cmp[nxt] == _c and !used[nxt]) {
                _ret_cycle.emplace_back(nxt), used[nxt] = 1;
                if (_dfs_detectcycle(nxt, true)) return true;
                _ret_cycle.pop_back();
            }
        return false;
    }
    std::vector<int> DetectCycle() {
        int ns = FindStronglyConnectedComponents();
        if (ns == V) return {};
        std::vector<int> cnt(ns);
        for (auto x : cmp) cnt[x]++;
        _c = std::find_if(cnt.begin(), cnt.end(), [](int x) { return x > 1; }) - cnt.begin();
        _init = std::find(cmp.begin(), cmp.end(), _c) - cmp.begin();
        used.assign(V, false);
        _ret_cycle.clear();
        _dfs_detectcycle(_init, false);
        return _ret_cycle;
    }

    // After calling `FindStronglyConnectedComponents()`, generate a new graph by uniting all
    // vertices belonging to the same component(The resultant graph is DAG).
    DirectedGraphSCC GenerateTopologicalGraph() {
        DirectedGraphSCC newgraph(scc_num);
        for (int s = 0; s < V; s++)
            for (auto t : to[s]) {
                if (cmp[s] != cmp[t]) newgraph.add_edge(cmp[s], cmp[t]);
            }
        return newgraph;
    }
};

std::vector<std::pair<std::vector<int>, std::vector<int>>>
dulmage_mendelsohn(int L, int R, const std::vector<std::pair<int, int>> &edges) {
    for (auto p : edges) {
        assert(0 <= p.first and p.first < L);
        assert(0 <= p.second and p.second < R);
    }

    BipartiteMatching bm(L + R);
    for (auto p : edges) bm.add_edge(p.first, L + p.second);
    bm.solve();

    DirectedGraphSCC scc(L + R);
    for (auto p : edges) scc.add_edge(p.first, L + p.second);
    for (int l = 0; l < L; ++l) {
        if (bm.match[l] >= L) scc.add_edge(bm.match[l], l);
    }

    int nscc = scc.FindStronglyConnectedComponents();
    std::vector<int> cmp_map(nscc, -2);

    std::vector<int> vis(L + R);
    std::vector<int> st;
    for (int c = 0; c < 2; ++c) {
        std::vector<std::vector<int>> to(L + R);
        auto color = [&L](int x) { return x >= L; };
        for (auto p : edges) {
            int u = p.first, v = L + p.second;
            if (color(u) != c) std::swap(u, v);
            to[u].push_back(v);
            if (bm.match[u] == v) to[v].push_back(u);
        }
        for (int i = 0; i < L + R; ++i) {
            if (bm.match[i] >= 0 or color(i) != c or vis[i]) continue;
            vis[i] = 1, st = {i};
            while (!st.empty()) {
                int now = st.back();
                cmp_map[scc.cmp[now]] = c - 1;
                st.pop_back();
                for (int nxt : to[now]) {
                    if (!vis[nxt]) vis[nxt] = 1, st.push_back(nxt);
                }
            }
        }
    }

    int nset = 1;
    for (int n = 0; n < nscc; ++n) {
        if (cmp_map[n] == -2) cmp_map[n] = nset++;
    }
    for (auto &x : cmp_map) {
        if (x == -1) x = nset;
    }
    nset++;

    std::vector<std::pair<std::vector<int>, std::vector<int>>> groups(nset);

    for (int l = 0; l < L; ++l) {
        if (bm.match[l] < 0) continue;
        int c = cmp_map[scc.cmp[l]];
        groups[c].first.push_back(l);
        groups[c].second.push_back(bm.match[l] - L);
    }
    for (int l = 0; l < L; ++l) {
        if (bm.match[l] >= 0) continue;
        int c = cmp_map[scc.cmp[l]];
        groups[c].first.push_back(l);
    }
    for (int r = 0; r < R; ++r) {
        if (bm.match[L + r] >= 0) continue;
        int c = cmp_map[scc.cmp[L + r]];
        groups[c].second.push_back(r);
    }

    return groups;
}


// 2-SAT solver: Find a solution for  `(Ai v Aj) ^ (Ak v Al) ^ ... = true`
// - `nb_sat_vars`: Number of variables
// - Considering a graph with `2 * nb_sat_vars` vertices
// - Vertices [0, nb_sat_vars) means `Ai`
// - vertices [nb_sat_vars, 2 * nb_sat_vars) means `not Ai`
struct SATSolver : DirectedGraphSCC {
    int nb_sat_vars;
    std::vector<int> solution;
    SATSolver(int nb_variables = 0)
        : DirectedGraphSCC(nb_variables * 2), nb_sat_vars(nb_variables), solution(nb_sat_vars) {}
    void add_x_or_y_constraint(bool is_x_true, int x, bool is_y_true, int y) {
        assert(x >= 0 and x < nb_sat_vars);
        assert(y >= 0 and y < nb_sat_vars);
        if (!is_x_true) x += nb_sat_vars;
        if (!is_y_true) y += nb_sat_vars;
        add_edge((x + nb_sat_vars) % (nb_sat_vars * 2), y);
        add_edge((y + nb_sat_vars) % (nb_sat_vars * 2), x);
    }
    // Solve the 2-SAT problem. If no solution exists, return `false`.
    // Otherwise, dump one solution to `solution` and return `true`.
    bool run() {
        FindStronglyConnectedComponents();
        for (int i = 0; i < nb_sat_vars; i++) {
            if (cmp[i] == cmp[i + nb_sat_vars]) return false;
            solution[i] = cmp[i] > cmp[i + nb_sat_vars];
        }
        return true;
    }
};

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