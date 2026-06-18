/*
  <Lowlink>
    - 無向グラフの橋と関節点を列挙する

    [実装/関数]
        - Lowlink<int> lowlink(WGraph<T> G)
            : lowlinkをする．lowlink.apsに関節点の集合を，lowlink.bridgesに
                橋の集合を入れる．
        - vector<int> lowlink.aps : 関節点の集合
        - vector<int> lowlink.bridges : 橋である辺のindexの集合

    [計算時間]
        - O(n+m)
    
    [備考]
        - 
    
    [参照]
        - https://bakamono1357.hatenablog.com/entry/2020/04/29/025320
        - https://algo-logic.info/bridge-lowlink/

    [verified at]
        - https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all
    
    [使用例]
        
        
*/


template <typename T>
struct LowLink {
    const WGraph<T> &G;
    vector<int> used, ord, low;
    vector<int> aps;  // 関節点の集合
    vector<int> bridges; //橋である辺のindexの集合
    LowLink(const WGraph<T> &G_) : G(G_) {
        used.assign(G.size(), 0);
        ord.assign(G.size(), 0);
        low.assign(G.size(), 0);
        int k = 0;
        for (int i = 0; i < (int)G.size(); i++) {
            if (!used[i]) k = dfs(i, k, -1);
        }
        sort(aps.begin(), aps.end()); // 必要ならソートする
        sort(bridges.begin(), bridges.end()); // 必要ならソートする
    }
    int dfs(int id, int k, int par) { // id:探索中の頂点, k:dfsで何番目に探索するか, par:idの親
        used[id] = true;
        ord[id] = k++;
        low[id] = ord[id];
        bool is_aps = false;
        int count = 0; // 子の数
        for (auto &e : G[id]) {
            if (!used[e.to]) {
                count++;
                k = dfs(e.to, k, id);
                low[id] = min(low[id], low[e.to]);
                if (par != -1 && ord[id] <= low[e.to]) is_aps = true; 
                if (ord[id] < low[e.to]) bridges.emplace_back(e.index); // 条件を満たすので橋  
            } else if (e.to != par) { // eが後退辺の時
                low[id] = min(low[id], ord[e.to]);
            }
        }
        if (par == -1 && count >= 2) is_aps = true; 
        if (is_aps) aps.push_back(id);
        return k;
    }
};
