//https://kokiymgch.hatenablog.com/entry/2018/01/27/235959 
/*
    - O(2^n * n)
        - グラフをいくつかのクリーク(完全グラフである部分グラフ)に分割して重複なく頂点集合を
        覆うとき，その分割する個数が最小になるようなものを最小クリーク被覆という．
        - グラフにおけるクリークは，補グラフにおいて独立集合をなす
        -> グラフの彩色数は，その補グラフにおける最小クリーク被覆のサイズに等しい
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