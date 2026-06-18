/*
  <Rerooting(全方位木DP)>
    - n頂点の木Tの各頂点を値とした時の木DPの結果を得る

    [実装/関数]
        (構造体)
        - struct RerootingDP<class E, class V, merge, e, put_edge, put_vertex> dp(n)
            - E : 辺に持たせる情報の型
            - V : 頂点に持たせる情報の型(≠頂点の番号) 各頂点のdp表にはこの型の値を記録する
            - E merge(E e1, E e2) : 同じ頂点を親側に持つ二つの辺をマージして新たな辺の情報として持つ
            - E e() : mergeの単位元
            - E put_edge(V v, int i) : 頂点iの親側に辺を1つ張った時，その辺が持つ情報を計算
            - V put_vertex(E e, int i) : 頂点iの全ての子側の辺をマージした値から，頂点の情報を計算
                - 木の葉のdpの値は，Eの単位元eによってput_vertex(e,i)により定まる
            (葉 : put_vertex(e,i) により生成
            他 : 葉の値をそれぞれput_edge -> merge -> 得られた値をput_vertex)
            
        (関数)
        - void add_edge(int u, int v, int i, int j) : 
            頂点uからvへの番号iの辺と，頂点vからuへの番号jの辺を追加する(i=jでもよい)

        - V build(int v) : 頂点vを根とした時のdp[v]の値を返す(1回しか呼べない)

        - vector<V> reroot() : 頂点iを根とした時のdp[i]の値を持つ配列を返す
            (事前にbuildをちょうど1回呼んでいる必要あり)
    
    [計算時間]
        - コンストラクタ O(n), build O(n), reroot O(n)
    
    [備考]
        - (codeforcesのG++ 20ではEとしてpiiを用いるとうまく動かなかった G++ 17 を用いること)
        - put_edgeとput_vertexの添字iの型はintで指定すること(llとするとコンパイルエラー)

    [参照]
        - traP

    [verified at]
        - https://atcoder.jp/contests/abc394/submissions/63066845
        
*/

/*
    - n頂点の木Tが与えられている

    - RerootingDP<E,V,merge,e,put_edge,put_vertex> g(n) : コンストラクタ O(n)
        E : 親のdpの計算をする際に用いる値の型
        V : 頂点のdpの値の型
        E put_edge (V v, int i) : 頂点vを根とする部分木のdpの値に辺iを追加した時に得られる値
        E merge(E e1, E e2) :  2つの子について求めたput_edgeの値を結合する
            mergeは可換モノイドによる結合を要求される
        E e() : mergeの単位元を返す関数
        V put_vertex(E e, int u) : 全ての子についてmergeした値から，頂点uのdp[u]を計算する
    
    - void add_edge(int u, int v, int idx, int xdi) : 頂点u->vの辺番号idxの辺と
        頂点v->uの辺番号xdiの辺を追加する (ちょうどn-1回呼ぶ．ならしO(1))

    - V bulid(int v): 頂点vを根とした時の，dp[v]の値を返す (1回までしか呼べない O(n))

    - vector<V> reroot(): 長さnの配列(i番目には，頂点iを根とした時の，dp[i]の値)を返す
        (先にbuildをちょうど1回呼んでいる必要あり O(n))
 

    
    (codeforcesのG++ 20ではEとしてpiiを用いるとうまく動かなかった G++ 17 を用いること)
*/

template <class E, class V, E (*merge)(E, E), E (*e)(), E (*put_edge)(V, int), V (*put_vertex)(E, int)>
struct RerootingDP {
    struct edge {
        int to, idx, xdi;
    };
    RerootingDP(int n_ = 0) : n(n_), inner_edge_id(0) {
        es.resize(2*n-2);
        start.resize(2*n-2);
        if (n == 1) es_build();
    }
    void add_edge(int u, int v, int idx, int xdi){
        start[inner_edge_id] = u;
        es[inner_edge_id] = {v,idx,xdi};
        inner_edge_id++;
        start[inner_edge_id] = v;
        es[inner_edge_id] = {u,xdi,idx};
        inner_edge_id++;
        if (inner_edge_id == 2*n-2){
            es_build();
        }
    }
    vector<V> build(int root_ = 0){
        root = root_;
        vector<V> subdp(n); subdp[0] = put_vertex(e(),0);
        outs.resize(n);
        vector<int> geta(n+1,0);
        for (int i = 0; i < n; i++) geta[i+1] = start[i+1] - start[i] - 1;
        geta[root+1]++;
        for (int i = 0; i < n; i++) geta[i+1] += geta[i];
        auto dfs = [&](auto sfs, int v, int f) -> void {
            E val = e();
            for (int i = start[v]; i < start[v+1]; i++){
                if (es[i].to == f){
                    swap(es[start[v+1]-1],es[i]);
                }
                if (es[i].to == f) continue;
                sfs(sfs,es[i].to,v);
                E nval = put_edge(subdp[es[i].to],es[i].idx);
                outs[geta[v]++] = nval;
                val = merge(val,nval);
            }
            subdp[v] = put_vertex(val, v);
        };
        dfs(dfs,root,-1);
        return subdp;
    }
    vector<V> reroot(){
        vector<E> reverse_edge(n);
        reverse_edge[root] = e();
        vector<V> answers(n);
        auto dfs = [&](auto sfs, int v) -> void {
            int le = outs_start(v);
            int ri = outs_start(v+1);
            int siz = ri - le;
            vector<E> rui(siz+1);
            rui[siz] = e();
            for (int i = siz-1; i >= 0; i--){
                rui[i] = merge(outs[le+i],rui[i+1]);
            }
            answers[v] = put_vertex(merge(rui[0],reverse_edge[v]),v);
            E lui = e();
            for (int i = 0; i < siz; i++){
                V rdp = put_vertex(merge(merge(lui,rui[i+1]),reverse_edge[v]),v);
                reverse_edge[es[start[v]+i].to] = put_edge(rdp,es[start[v]+i].xdi);
                lui = merge(lui,outs[le+i]);
                sfs(sfs,es[start[v]+i].to);
            }
        };
        dfs(dfs,root);
        return answers;
    }
    private:
    int n, root, inner_edge_id;
    vector<E> outs;
    vector<edge> es;
    vector<int> start;
    int outs_start(int v){
        int res = start[v] - v;
        if (root < v) res++;
        return res;
    }
    void es_build(){
        vector<edge> nes(2*n-2);
        vector<int> nstart(n+2,0);
        for (int i = 0; i < 2*n-2; i++) nstart[start[i]+2]++;
        for (int i = 0; i < n; i++) nstart[i+1] += nstart[i];
        for (int i = 0; i < 2*n-2; i++) nes[nstart[start[i]+1]++] = es[i];
        swap(es,nes);
        swap(start,nstart);
    }
};


// typedef array<int,4> E;

// mint merge(mint a, mint b){
//     return a * b;
// }
// mint e(){
//     return mint(1);
// }
// mint put_edge(mint v, int i){
//     return v + 1;
// }
// mint put_vertex(mint e, int v){
//     return e;
// }

// int main(){
//     int n, m; cin >> n >> m;
//     mint::set_mod(m);
//     RerootingDP<mint,mint,merge,e,put_edge,put_vertex> g(n);
//     for (int i = 0; i < n-1; i++){
//         int u, v; cin >> u >> v;
//         g.add_edge(u-1,v-1,i,i);
//     }
//     g.build();
//     for (auto ans : g.reroot()) cout << ans.val() << endl;
// }