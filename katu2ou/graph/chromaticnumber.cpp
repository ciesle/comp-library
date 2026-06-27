/*
  <Chromatic Number / 彩色数>
    - 無向グラフの彩色数を O(n 2^n) で求める
    - 頂点数 n のグラフを bitmask で表す
    - g[v] は頂点 v に隣接する頂点集合を表す bitmask

    [問題]
        - グラフ G の彩色数 χ(G) を求める
        - χ(G) は，隣接する頂点が同じ色にならないように全頂点を塗るために必要な最小色数

    [原理]
        - ある色で塗られる頂点集合は，元グラフ G における独立集合でなければならない

        - したがって，k 色で彩色できることは，
          頂点集合 V を k 個の独立集合で被覆できることと同値である

        - 各部分集合 S ⊆ V に対して，
              ind[S] = G[S] に含まれる独立集合の個数
          を求める

        - k 個の独立集合の組で V 全体を被覆する方法の数は，
          包除原理により

              Σ_{S ⊆ V} (-1)^{n-|S|} ind[S]^k

          で求まる

        - この値が 0 でなければ，G は k 色で彩色可能である
        - k = 1,2,... と試し，最初に非 0 になった k が彩色数になる

    [実装/関数]
        - int ChromaticNumber(vector<int> g)
            グラフ G の彩色数を返す

            g[v] は頂点 v の隣接頂点集合を表す bitmask
            つまり，辺 (v,u) があるなら g[v] の u bit 目が 1

    [計算時間]
        - O(n 2^n)

    [メモリ]
        - O(2^n)

    [要件]
        - 無向単純グラフを想定
        - g[v] は 0 <= v < n の範囲の bit のみを使う
        - 辺は対称に入れること
            つまり，辺 (u,v) に対して
                g[u] |= 1 << v
                g[v] |= 1 << u
          とする

    [備考]
        - この実装は modulo = 1077563119 で計算する
        - 包除原理の値を整数としてではなく mod prime 上で判定しているため，
          理論上は確率的な判定になる

        - k < χ(G) のとき，対応する被覆数は整数として 0 なので，
          mod を取っても必ず 0 になる
          したがって，真の彩色数より小さい値を返すことはない

        - 一方，k = χ(G) のとき，被覆数が modulo で割り切れると，
          0 と判定されてしまい，真の彩色数より大きい値を返す可能性がある

        - 実用上は大きな素数を使えばほとんど問題にならないが，
          厳密性を上げたい場合は複数の mod で判定するとよい

        - int all = 1 << n を使っているため，n が大きすぎる場合は使えない
          vector<int> の bitmask 実装なので，基本的には n <= 30 程度まで
          ただし O(n 2^n) なので，実用的には n は 20〜25 程度が目安

        - 元グラフ G の彩色数は，補グラフの最小クリーク被覆数とも等しい
          ただし，この関数に渡す g は補グラフではなく，元グラフの隣接情報である

    [使用例]
        int n, m;
        cin >> n >> m;

        vector<int> g(n, 0);

        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            g[u] |= 1 << v;
            g[v] |= 1 << u;
        }

        cout << ChromaticNumber(g) << endl;

    [verified at]
        - g++ 14.2.0, -std=gnu++23
        - n <= 6 の全グラフで愚直彩色と比較
        - n <= 12 のランダムグラフ 2000 ケースで愚直彩色と比較

    [参照]
        - https://kokiymgch.hatenablog.com/entry/2018/01/27/235959
*/

int ChromaticNumber(const vector<int> &g) {
        int n = g.size();
        if (n == 0) return 0;
        //randomly choose a large prime
        const int modulo = 1077563119;
        int all = 1 << n;
        vector<int> ind(all), s(all);
        for (int i = 0; i < all; i ++) s[i] = ((n - __builtin_popcount(i)) & 1 ? -1 : 1);
        ind[0] = 1;
        for (int i = 1; i < all; i ++) {
                int ctz = __builtin_ctz(i);
                ind[i] = ind[i - (1 << ctz)] + ind[(i - (1 << ctz)) & ~g[ctz]];
                if (ind[i] >= modulo) ind[i] -= modulo;
        }
        //compute the chromatic number (= \sum (-1)^{n - |i|} * ind(i))^k)
        for (int k = 1; k < n; k ++) {
                long long sum = 0;
                for (int i = 0; i < all; i ++) {
                        long long cur = ((s[i] * (long long) ind[i]) % modulo);
                        s[i] = cur;
                        sum += cur;
                }
                if (sum % modulo != 0) return k;
        }
        return n;
}