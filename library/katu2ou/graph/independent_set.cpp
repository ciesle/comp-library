/*
  <Maximum Independent Set / 最大安定集合>
    - 一般の無向単純グラフに対して，最大安定集合を求める
    - 安定集合とは，どの 2 頂点間にも辺が存在しないような頂点集合のこと
    - 最大安定集合とは，サイズが最大の安定集合のこと

    [概要]
        - 分枝探索により最大安定集合を求める
        - 連結成分ごとに独立に解けるため，まず現在残っている頂点集合を連結成分に分解する
        - 各連結成分に対して以下を行う

            1. 最大次数が 2 以下の場合
                - 連結成分はパスまたはサイクル
                - パス・サイクル上の最大安定集合は DP で解ける

            2. 次数 1 の頂点がある場合
                - 葉 p は，ある最大安定集合に含めてよい
                - p を採用し，p とその隣接頂点を削除して再帰する

            3. それ以外の場合
                - 最大次数の頂点 p を選ぶ
                - p を使う場合と使わない場合の 2 通りに分枝する

                p を使う場合:
                    p と p の隣接頂点を削除して再帰し，p を答えに加える

                p を使わない場合:
                    p だけを削除して再帰する

        - 2 通りのうち，得られる安定集合のサイズが大きい方を採用する

    [実装/関数]
        - MaximumIndependentSet mis(int n)
            頂点数 n の空グラフを作る

        - MaximumIndependentSet mis(int n, vector<vector<int>> g)
            隣接リスト g からグラフを作る

        - void add_edge(int x, int y)
            無向辺 x-y を追加する

        - vector<int> StableSetVertices()
            最大安定集合を構成する頂点集合を返す

        - int StableSet()
            最大安定集合のサイズを返す

    [内部関数]
        - void dfs(int x, vector<char>& seen, vector<char>& comp,
                   const vector<char>& can)
            can で残っている頂点だけを使って DFS し，
            x を含む連結成分を comp に記録する

        - vector<int> solve_path(vector<int> order, int l, int r)
            パス order[l], order[l+1], ..., order[r-1] 上の最大安定集合を返す

        - vector<int> DegreeLeqTwoCase(vector<char> can)
            最大次数 2 以下の連結成分を解く
            連結成分はパスまたはサイクルなので，それぞれ専用に処理する

        - vector<int> ConnectedCase(vector<char> can)
            can で表された 1 つの連結成分の最大安定集合を返す

        - vector<int> GeneralCase(vector<char> can)
            can で表された残り頂点集合を連結成分に分け，
            各成分の最大安定集合を合わせて返す

    [計算時間]
        - 最悪計算量は指数時間

        - 分枝部分では，最大次数 3 以上の頂点 p に対して
              p を使わない: 頂点 p を削除
              p を使う    : p と隣接頂点を削除
          という分枝を行う

        - 最大次数が 2 以下の成分は線形時間で処理する

        - 実用上は小〜中規模の一般グラフに対する厳密解法として使う

    [要件]
        - 無向単純グラフを想定する
        - self-loop や多重辺は入れないこと
        - 辺 x-y を追加するときは add_edge(x,y) を 1 回だけ呼ぶ
        - 頂点番号は 0-indexed

    [備考]
        - 返り値の頂点集合の順序は保証しない
        - StableSet() は内部で StableSetVertices() を呼ぶため，集合を構成してからサイズを返す
        - 最大安定集合は複数存在することがあり，そのうち 1 つを返す
        - この問題は一般グラフでは NP-hard なので，多項式時間では解けないことが期待される
        - 補グラフで最大クリークを求める問題と対応する

    [正当性の要点]
        - 連結成分が複数ある場合，最大安定集合は各連結成分の最大安定集合の和として得られる

        - 最大次数 2 以下の連結成分は，パスまたはサイクルである
          パスは標準的な DP で解ける
          サイクルは，先頭頂点を使う場合・使わない場合に分けることでパスに帰着できる

        - 次数 1 の頂点 p については，ある最大安定集合に p を含めることができる
          そのため，p とその隣接頂点を削除して再帰してよい

        - それ以外の場合，任意の頂点 p について，
          最大安定集合は
              p を含む
              p を含まない
          のどちらかである
          両方を試して大きい方を選ぶので，最適解を失わない

    [verified at]
        - g++ 14.2.0, -std=gnu++23
        - n <= 6 の全単純無向グラフで愚直全探索と比較
        - n <= 14 のランダム単純無向グラフ 5000 ケースで愚直全探索と比較

    [使用例]
        int n, m;
        cin >> n >> m;

        MaximumIndependentSet mis(n);

        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            mis.add_edge(u, v);
        }

        vector<int> st = mis.StableSetVertices();

        cout << st.size() << endl;

        for (int v : st) {
            cout << v << ' ';
        }
        cout << endl;
*/

struct MaximumIndependentSet {
    int n;
    vector<vector<int>> v;

    MaximumIndependentSet(int n_ = 0) : n(n_), v(n_) {}
    MaximumIndependentSet(int n_, vector<vector<int>> v_) : n(n_), v(v_) {}

    void add_edge(int x, int y) {
        v[x].push_back(y);
        v[y].push_back(x);
    }

    void dfs(int x, vector<char>& seen, vector<char>& comp, const vector<char>& can) {
        seen[x] = true;
        comp[x] = true;
        for (auto to : v[x]) {
            if (!seen[to] && can[to]) dfs(to, seen, comp, can);
        }
    }

    // パス order[l], order[l+1], ..., order[r-1] 上の最大安定集合を返す
    vector<int> solve_path(const vector<int>& order, int l, int r) {
        int m = r - l;
        vector<int> dp(m + 1, 0);

        for (int i = 1; i <= m; ++i) {
            dp[i] = dp[i - 1];
            if (i >= 2) dp[i] = max(dp[i], dp[i - 2] + 1);
            else dp[i] = max(dp[i], 1);
        }

        vector<int> res;
        int i = m;
        while (i > 0) {
            int take = 1;
            if (i >= 2) take += dp[i - 2];

            if (dp[i] == dp[i - 1]) {
                --i;
            } else {
                res.push_back(order[l + i - 1]);
                i -= 2;
            }
        }

        reverse(res.begin(), res.end());
        return res;
    }

    // 最大次数2以下の連結成分，すなわちパスまたはサイクルを解く
    vector<int> DegreeLeqTwoCase(const vector<char>& can) {
        vector<int> verts;
        for (int i = 0; i < n; ++i) {
            if (can[i]) verts.push_back(i);
        }

        int num = (int)verts.size();
        if (num == 0) return {};
        if (num == 1) return {verts[0]};

        int start = -1;

        // 次数1以下の頂点があればパス
        for (int x : verts) {
            int deg = 0;
            for (int to : v[x]) {
                if (can[to]) ++deg;
            }
            if (deg <= 1) {
                start = x;
                break;
            }
        }

        bool is_path = (start != -1);

        // 次数1以下の頂点がなければサイクル
        if (!is_path) start = verts[0];

        vector<int> order;
        int pre = -1;
        int cur = start;

        for (int step = 0; step < num; ++step) {
            order.push_back(cur);

            if (step + 1 == num) break;

            int nxt = -1;
            for (int to : v[cur]) {
                if (!can[to]) continue;
                if (to == pre) continue;
                nxt = to;
                break;
            }

            pre = cur;
            cur = nxt;
        }

        if (is_path) {
            return solve_path(order, 0, num);
        } else {
            // サイクルの場合
            // 1. order[0] を使わない場合: order[1..num-1] のパス
            vector<int> res1 = solve_path(order, 1, num);

            // 2. order[0] を使う場合:
            //    order[1] と order[num-1] は使えないので，
            //    order[2..num-2] のパスを解く
            vector<int> res2 = solve_path(order, 2, num - 1);
            res2.push_back(order[0]);

            if (res1.size() >= res2.size()) return res1;
            else return res2;
        }
    }

    vector<int> ConnectedCase(const vector<char>& can) {
        int pMax = -1, pMin = -1;
        int Max = -1, Min = n + 1, num = 0;

        for (int i = 0; i < n; ++i) {
            if (!can[i]) continue;

            ++num;
            int deg = 0;
            for (auto to : v[i]) {
                if (can[to]) ++deg;
            }

            if (Max < deg) {
                Max = deg;
                pMax = i;
            }
            if (Min > deg) {
                Min = deg;
                pMin = i;
            }
        }

        if (num == 0) return {};
        if (num == 1) return {pMin};

        // 連結かつ最大次数2以下なら，パスまたはサイクル
        if (Max <= 2) {
            return DegreeLeqTwoCase(can);
        }

        // 次数1の頂点 pMin がある場合，pMin はある最大安定集合に含めてよい
        if (Min < 2) {
            vector<char> ncan = can;
            ncan[pMin] = false;
            for (auto to : v[pMin]) ncan[to] = false;

            vector<int> res = GeneralCase(ncan);
            res.push_back(pMin);
            return res;
        }

        // pMax を使う場合
        vector<char> ncan = can;
        ncan[pMax] = false;
        for (auto to : v[pMax]) ncan[to] = false;

        vector<int> use = GeneralCase(ncan);
        use.push_back(pMax);

        // pMax を使わない場合
        ncan = can;
        ncan[pMax] = false;

        vector<int> not_use = GeneralCase(ncan);

        if (use.size() >= not_use.size()) return use;
        else return not_use;
    }

    vector<int> GeneralCase(const vector<char>& can) {
        vector<char> seen(n, false);
        vector<int> res;

        for (int i = 0; i < n; ++i) {
            if (!seen[i] && can[i]) {
                vector<char> gcan(n, false);
                dfs(i, seen, gcan, can);

                vector<int> sub = ConnectedCase(gcan);
                for (int x : sub) res.push_back(x);
            }
        }

        return res;
    }

    // 最大安定集合そのものを返す
    vector<int> StableSetVertices() {
        vector<char> can(n, true);
        return GeneralCase(can);
    }

    // 最大安定集合のサイズだけを返す
    int StableSet() {
        return (int)StableSetVertices().size();
    }
};