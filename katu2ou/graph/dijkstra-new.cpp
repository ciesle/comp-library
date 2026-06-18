/*
- 普通のdijkstra法をO(ElogV)で行う．

    - dijkstra(G, int start = 0) : 重み付きグラフGで始点startのダイクストラ法を行い，
    各頂点までの最短距離が入ったvecetor配列を返す．
        
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

/*
- 経路復元もするもの

    - dijkstra_restore(G, int start = 0) : 重み付きグラフGで始点startのダイクストラ法を行い，
    各頂点の最短距離をfirstに，最短距離を実現する一つのパスに対し始点から終点までの
    頂点列をsecondにおいたvectorを返す．
    - ここで，頂点uに対応するsecond成分は，startからuまでの最短路をとったときの最後尾として
    ありうる頂点の一つである．(よって，最短路の逆路が，uからsecond成分への移動を繰り返すことで
    得られる)
*/
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