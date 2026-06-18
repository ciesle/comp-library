//離散対数問題(baby step-giant step 法)

// 入力 ll x,y,p (pは素数でなくても良い)
// 出力 x^a = y (mod p)を満たす最小の非負整数aを返す
// 解が存在しない場合は-1を返すはず

ll modlog(ll x, ll y, ll p) {
    ll H = sqrt(p) + 1;
    static std::pair<ll, ll> baby[100010];
    for (ll b = 0, xby = y; b < H; b++, xby = (xby * x) % p) {
        baby[b] = std::make_pair(xby, b);
    }
    std::sort(baby, baby + H);
    ll xH = 1;
    for (int i = 0; i < H; ++i) {
        xH = (xH * x) % p;
    }
    for (ll a = 1, xaH = xH; a <= H; a++, xaH = (xaH * xH) % p) {
        auto it = std::lower_bound(baby, baby + H, std::make_pair(xaH + 1, 0LL));
        if (it == baby) continue;
        it--;
        if (it->first == xaH) return a * H - it->second;
    }
    return -1;
}