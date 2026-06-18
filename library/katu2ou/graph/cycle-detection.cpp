/*
- 有向グラフに対して，閉路があればそれを一つ取って返す．閉路がなければ空の配列を返す．
    - 配列は，閉路に含まれる頂点を順番に持つ
*/
vector<int> find_cycle(const vector<vector<int>>& g, bool directed = true) {
    const int n = (int)g.size();
    vector<int> parent(n, -1);
    vector<int> state(n, 0);               // 0=未訪問,1=探索中,2=完了
    vector<int> cycle;

    /* ---- 再帰 DFS ---- */
    function<bool(int,int)> dfs = [&](int v,int p)->bool{
        state[v] = 1;
        for(int to : g[v]){
            if(!directed && to == p) continue;      // 無向グラフでの親戻りを除外

            if(state[to] == 0){
                parent[to] = v;
                if(dfs(to, v)) return true;
            }
            else if(state[to] == 1){                // 後退辺 / 交差辺 発見
                /* --- 閉路復元 --- */
                cycle.push_back(to);
                for(int cur = v; cur != to; cur = parent[cur])
                    cycle.push_back(cur);
                cycle.push_back(to);
                reverse(cycle.begin(), cycle.end());
                return true;
            }
        }
        state[v] = 2;
        return false;
    };

    for(int v=0; v<n; ++v)
        if(state[v]==0 && dfs(v,-1))
            break;
    if(cycle.size())
        cycle.pop_back();
    return cycle; // 空なら閉路なし
}
    