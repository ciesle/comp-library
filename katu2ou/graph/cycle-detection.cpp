/*
  <閉路検出>
    - グラフに閉路が存在すれば，その閉路を 1 つ返す
    - 閉路が存在しなければ空配列を返す
    - 有向グラフ・無向グラフの両方に対応する
    - 単純グラフに対応(多重辺における長さ2のループは検出しない)

    [実装/関数]
        - vector<int> find_cycle(vector<vector<int>> g, bool directed = true)

            g に含まれる閉路を 1 つ返す

            directed == true の場合:
                有向グラフとして閉路を検出する

            directed == false の場合:
                無向グラフとして閉路を検出する

    [返り値]
        - 閉路が存在する場合:
            閉路に含まれる頂点列を順番に返す

            例えば返り値が

                {v0, v1, ..., vk-1}

            なら，有向グラフでは

                v0 -> v1 -> ... -> vk-1 -> v0

            が存在する

            無向グラフでは，各隣り合う頂点間，および最後と最初の間に辺が存在する

        - 閉路が存在しない場合:
            空配列を返す

    [原理]
        - DFS 中の各頂点を次の 3 状態で管理する

            state[v] = 0 : 未訪問
            state[v] = 1 : 探索中
            state[v] = 2 : 探索完了

        - 有向グラフでは，探索中の頂点へ戻る辺を見つけたとき，閉路が存在する

        - 無向グラフでは，親へ戻る辺を除外したうえで，
          探索中の頂点へ戻る辺を見つけたとき，閉路が存在する

        - 閉路を見つけたら parent をたどって頂点列を復元する

    [計算時間]
        - O(N + M)

        ここで，
            N = 頂点数
            M = 辺数

    [メモリ]
        - O(N)

    [要件]
        - g[v] は頂点 v から出る辺の行き先リスト
        - 無向グラフの場合は，各辺 u-v について両方向に入れること

            g[u].push_back(v);
            g[v].push_back(u);

        - 無向グラフでは単純グラフを想定する
          多重辺による長さ 2 の閉路は検出しない

    [備考]
        - self-loop は閉路として検出される
        - 返り値では始点を末尾に重複させない
          必要なら cycle.push_back(cycle[0]) すればよい
        - 再帰 DFS を使うため，頂点数が非常に大きい場合はスタックオーバーフローに注意
        - 有向グラフで state[to] == 1 の辺は，DFS 木における後退辺に対応する

    [verified at]
        - g++ 14.2.0, -std=gnu++23
        - 有向グラフ n <= 5 の全ケースで検証
        - 無向単純グラフ n <= 6 の全ケースで検証
        - ランダムグラフ 10000 ケースで検証

    [使用例]
        int n, m;
        cin >> n >> m;

        vector<vector<int>> g(n);

        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;

            // 有向グラフの場合
            g[u].push_back(v);

            // 無向グラフの場合は両方向に入れる
            // g[u].push_back(v);
            // g[v].push_back(u);
        }

        auto cycle = find_cycle(g, true);

        if (cycle.empty()) {
            cout << "No cycle\n";
        } else {
            for (int v : cycle) cout << v << ' ';
            cout << '\n';
        }
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
    