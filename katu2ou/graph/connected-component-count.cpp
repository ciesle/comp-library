/*
- 連結とは限らない無向グラフに対して，その連結成分ごとの頂点リストを返す O(N)
*/

template<typename GT>
vector<vector<int>> partitioned_graph(const GT& g){
    int n = g.size();
    vector<int> memo(n,-1);
    vector<vector<int>> res;
    int ind = 0;
    for(int i = 0; i < n; i++){
        if(memo[i] == -1)continue;
        res.push_back(vector<int>());
        queue<int> que;
        memo[i] = ind;
        que.push(i);
        memo[i] = ind;
        while(que.size()){
            int s = que.front();
            que.pop();
            res[ind].push_back(s);
            for(auto u : g[s]){
                if(memo[u] == -1){
                    que.push(u);
                    memo[u] = ind;
                }
            }
        }
        ind++;
    }
    return res;
}

