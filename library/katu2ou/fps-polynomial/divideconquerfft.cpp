/*
<分割統治FFT>
    - [l,r)に対する遷移を，[l,m)に対する処理 -> [l,m)から[m,r)に対する
    処理(ここでfft) -> [m,r)に対する処理 の順番で行う

    [使用方法]
        - dp[i] = base[i] + \sum dp[j] * g[i-j] // 0<=j<i
        の形のdpに対して使える
        - base と g を渡して，dpを返す

    [注意]
        - 計算量はO(N\log^2 N)
        - (r-l)=1 (単一要素のみをみている)の場合は，デフォルトでは
        何もしないようにしているので，必要ならばここでdp[l]を
        計算するようにする
    
    [使用例]
    int n;
    cin >> n;
    vector<mint> base(n), g(n);
    for (int i = 0; i < n; i++) {
        long long x;
        cin >> x;
        base[i] = x;
    }
    g[0] = 0;
    for (int i = 1; i < n; i++) {
        long long x;
        cin >> x;
        g[i] = x;
    }
    DivideConquerFFT<mint> solver(base, g);
    auto dp = solver.run();


*/



template <typename mint>
struct DivideConquerFFT {
    int n;
    vector<mint> dp;
    vector<mint> base;
    vector<mint> g;

    DivideConquerFFT(
        const vector<mint>& base_,
        const vector<mint>& g_
    ) 
        : n((int)base_.size()), dp(base_), base(base_), g(g_) {}

    void solve(int l, int r) {
        if (r - l == 1) return;

        int m = (l + r) / 2;

        solve(l, m);

        fps A(m - l);
        fps B(r - l);

        for (int i = l; i < m; i++) {
            A[i - l] = dp[i];
        }

        for (int i = 0; i < r - l; i++) {
            B[i] = g[i];
        }

        auto C = A * B;

        for (int i = m; i < r; i++) {
            dp[i] += C[i - l];
        }

        solve(m, r);
    }

    vector<mint> run() {
        solve(0, n);
        return dp;
    }
};