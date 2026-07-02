/*
  <Dijkstra / ダイクストラ法>
    - 非負重み付きグラフにおける単一始点最短路を求める
    - priority_queue を用いて O(E log V) で動作する
    - 有向グラフ・無向グラフのどちらにも使える
      無向グラフの場合は両方向に辺を追加する

    [前提]
        - すべての辺重みは 0 以上であること
        - 負辺があるグラフには使えない
        - 距離型 T は以下を満たすこと
            - T(0), T(-1) が使える
            - +, <, == が使える
            - priority_queue の比較に使える

    [グラフの型]
        - vector<vector<Edge<T>>> g

        - Edge<T> は例えば次のような構造体を想定する

            template <typename T>
            struct Edge {
                int to;
                T cost;
            };

        - g[v] は頂点 v から出る辺の一覧
        - 辺 e について
            e.to   : 行き先
            e.cost : 辺重み

    [実装/関数]
        - vector<T> dijkstra(vector<vector<Edge<T>>> &g, int start = 0)

            始点 start から各頂点への最短距離を返す．

            返り値 dist について，
                dist[v] = start から v への最短距離
                dist[v] = -1 なら v は到達不能

        - vector<pair<T, int>> dijkstra_restore(vector<vector<Edge<T>>> &g, int start = 0)

            始点 start から各頂点への最短距離と，最短路復元用の親を返す．

            返り値 res について，
                res[v].first  = start から v への最短距離
                res[v].second = 最短路上で v の直前に来る頂点

            start については
                res[start].first = 0
                res[start].second = -1

            到達不能な頂点 v については
                res[v].first = -1
                res[v].second = -1

    [経路復元]
        - dijkstra_restore の返り値 res を用いると，
          start から goal への最短路を次のように復元できる．

            vector<int> path;
            for (int v = goal; v != -1; v = res[v].second) {
                path.push_back(v);
            }
            reverse(path.begin(), path.end());

        - ただし，res[goal].first == -1 の場合，goal は到達不能である．

    [計算時間]
        - O(E log V)

    [メモリ]
        - O(V + E)

    [備考]
        - 未到達を T(-1) で表しているため，T は signed integer 型が自然
          例: int, long long
        - unsigned 型や modint 型には適さない
        - 距離のオーバーフローには注意する
        - 負辺がある場合は Bellman-Ford などを使う
        - dijkstra_restore は「最短路そのもの」を返すのではなく，
          各頂点に対する直前の頂点を返す

    [verified at]
        - g++ 14.2.0, -std=gnu++23
        - ランダム有向グラフ 5000 ケースで Floyd-Warshall と比較
*/

template <typename T>
vector<T> dijkstra(vector<vector<Edge<T>>> &g, int start = 0){
    using P = pair<T, int>;
    int N = (int)g.size();
    vector<T> d(N, T(-1));
    priority_queue<P, vector<P>, greater<P>> pq;
    d[start] = 0;
    pq.emplace(0, start);
    while(!pq.empty()){
        P p = pq.top();
        pq.pop();
        int cur = p.second;
        if(d[cur] < p.first) continue;
        for(auto dst : g[cur]){
            if(d[dst.to] == T(-1) || d[cur] + dst.cost < d[dst.to]){
                d[dst.to] = d[cur] + dst.cost;
                pq.emplace(d[dst.to],dst.to);
            }
        }
    }
    return d;
}


template <typename T>
vector<pair<T, int>> dijkstra_restore(vector<vector<Edge<T>>> &g, int start = 0) {
  int N = (int)g.size();
  using P = pair<T, int>;
  vector<P> d(N, P{-1, -1});
  priority_queue<P, vector<P>, greater<P>> Q;
  d[start].first = 0;
  Q.push({0, start});
  while (!Q.empty()) {
    auto p = Q.top();
    Q.pop();
    int cur = p.second;
    T dc = d[cur].first;
    if (dc < T(p.first)) continue;
    for (auto dst : g[cur]) {
      if (d[dst.to].first == T(-1) || dc + dst.cost < d[dst.to].first) {
        d[dst.to] = P{dc + dst.cost, cur};
        Q.push({dc + dst.cost, dst.to});
      }
    }
  }
  return d;
}