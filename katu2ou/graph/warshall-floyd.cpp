// 隣接行列dist(辺があるところは辺のcost, ないところは-1)を受け取り，全頂点最短路の長さとする

void warshall_floyd(vector<vector<ll>> &dist) {
    int V = dist.size();
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if(dist[i][k] == -1 || dist[k][j] == -1) continue;
                if(dist[i][j] == -1) dist[i][j] = dist[i][k] + dist[k][j];
                else dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }
}