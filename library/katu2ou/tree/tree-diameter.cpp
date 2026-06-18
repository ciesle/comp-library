/*
- 木の直径とそれを構成するパスをO(N)で求める
	- 各辺の長さが1のとき 
	- 各辺に正の整数の重みが定まっている時
	// Gにはvviやvector<vector<Edge<T>>>が入る

	-出力: 1要素目に直径の長さ，2要素目以降に直径を構成する点列
- N=1のときは未対応
*/

vector<int> tree_diameter(vector<vector<int>> &g){
	if(g.size()<=1) return {0,0};
	vector<int> diameter_vertex;
	int n = g.size();
	int lv = -1;
	long long lvcost = 0;
	vector<long long> dist(n,-1);
	queue<int> que;
	que.push(0);
	dist[0] = 0;
	while(que.size()){
		int v = que.front();
		que.pop();
		for(int to:g[v]){
			if(dist[to]!=-1) continue;
			dist[to] = dist[v] + 1;
			if(lvcost < dist[to]){
				lv = to, lvcost = dist[to];
			}
			que.push(to);
		}
	}
	int rv = -1;
	long long rvcost = 0;
	for(int i=0; i<n; i++){
		dist[i] = -1;
	}
	que.push(lv);
	dist[lv] = 0;
	while(que.size()){
		int v = que.front();
		que.pop();
		for(int to:g[v]){
			if(dist[to]!=-1) continue;
			dist[to] = dist[v] + 1;
			if(rvcost < dist[to]){
				rv = to, rvcost = dist[to];
			}
			que.push(to);
		}
	}
	diameter_vertex.push_back(rvcost);
	diameter_vertex.push_back(rv);
	while(rvcost){
		for(int to:g[rv]){
			if(dist[rv] - dist[to] == 1){
				diameter_vertex.push_back(to);
				rv = to;
				rvcost = dist[to];
				break;
			}
		}
	}
	return diameter_vertex;
}


template <typename T>
vector<long long> weighted_tree_diameter(vector<vector<Edge<T>>> &g){
	if(g.size()<=1) return {(long long)0, (long long)0};
	vector<long long> diameter_vertex;
	int n = g.size();
	int lv = -1;
	long long lvcost = 0;
	vector<long long> dist(n,-1);
	queue<int> que;
	que.push(0);
	dist[0] = 0;
	while(que.size()){
		int v = que.front();
		que.pop();
		for(auto e:g[v]){
			int to = e.to;
			if(dist[to]!=-1) continue;
			long long cost = e.cost;
			dist[to] = dist[v] + cost;
			if(lvcost < dist[to]){
				lv = to, lvcost = dist[to];
			}
			que.push(to);
		}
	}
	int rv = -1;
	long long rvcost = 0;
	for(int i=0; i<n; i++){
		dist[i] = -1;
	}
	dist[lv] = 0;
	que.push(lv);
	while(que.size()){
		int v = que.front();
		que.pop();
		for(auto e:g[v]){
			int to = e.to;
			if(dist[to] != -1) continue;
			long long cost = e.cost;
			dist[to] = dist[v] + cost;
			if(rvcost < dist[to]){
				rv = to, rvcost = dist[to];
			}
			que.push(to);
		}
	}
	diameter_vertex.push_back(rvcost);
	diameter_vertex.push_back(rv);
	while(rvcost){
		for(auto e:g[rv]){
			int to = e.to;
			long long cost = e.cost;
			if(dist[rv] - dist[to] == cost){
				diameter_vertex.push_back(to);
				rv = to;
				rvcost = dist[to];
				break;
			}
		}
	}
	return diameter_vertex;
}


