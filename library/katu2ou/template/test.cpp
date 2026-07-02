#include <bits/stdc++.h>
using namespace std;

#define rep2(i, m, n) for (int i = (m); i < (n); ++i)
#define rep(i, n) rep2(i, 0, n)
#define drep2(i, m, n) for (int i = (m)-1; i >= (n); --i)
#define drep(i, n) drep2(i, n, 0)
#define all(...) std::begin(__VA_ARGS__), std::end(__VA_ARGS__)
#define INF (long long)1001001001001001001
#define inf 1001001000
#define fi first
#define se second
#define PB push_back

using ll = long long;
using ld = long double;
using vi = vector<int>;
using vvi = vector<vector<int>>;
using vl = vector<long long>;
using vvl = vector<vector<long long>>;
using pii = pair<int, int>;
using pll = pair<long long, long long>;
using vpii = vector<pair<int,int>>;
using vpll = vector<pair<long long, long long>>;
using LL = __int128_t;

ll gcd(ll x, ll y) {if (x == 0) return y;	return gcd(y%x, x);} 
ll lcm(ll x, ll y) { __int128_t xx,yy; xx=x; yy=y; __int128_t ans=xx * yy / gcd(x, y); ll ans2=ans; return ans2; }
template<typename T>
T POW(T x, ll n){T ret=1;	while(n>0){		if(n&1) ret=ret*x;		x=x*x;		n>>=1;	}	return ret;}
template<typename T>
T modpow(T a, ll n, T p) {	if(n==0) return (T)1;  if (n == 1) return a % p;  if (n % 2 == 1) return (a * modpow(a, n - 1, p)) % p;  T t = modpow(a, n / 2, p);  return (t * t) % p;}
template<typename T>
T modinv(T a, T m) {	if(m==0)return (T)1;	T b = m, u = 1, v = 0;	while (b) {		T t = a / b;		a -= t * b; swap(a, b);		u -= t * v; swap(u, v);	}	u %= m;	if (u < 0) u += m;	return u;}
ll REM(ll a, ll b){ return (a % b + b) % b;}
ll QUO(ll a, ll b){ return (a - REM(a, b)) / b;}
struct RNG {
    std::mt19937_64 eng;      
    RNG() : eng(std::random_device{}()) {}   
    /// [a, b] の一様乱数を返す
    long long random_uniform(long long a, long long b) {
        std::uniform_int_distribution<long long> dist(a, b);
        return dist(eng);
    }
};
/*
auto start = chrono::steady_clock::now(); //時間計測の開始
auto now = std::chrono::steady_clock::now(); //現在時刻と開始時刻の差を測定
double elapsed = std::chrono::duration<double>(now - start).count(); //時間をdouble型で取得
*/
/*
const int MAXCOMB=510000;
std::vector<mint> FAC(MAXCOMB), FINV(MAXCOMB), INV(MAXCOMB);
void COMinit() {FAC[0] = FAC[1] = 1;FINV[0] = FINV[1] = 1;INV[1] = 1;for (int i = 2; i < MAXCOMB; i++) {FAC[i] = FAC[i - 1] * i;INV[i] = mint(0) - INV[mint::mod() % i] * (mint::mod() / i);FINV[i] = FINV[i - 1] * INV[i];}}
mint COM(int n, int k) {if (n < k) return 0;if (n < 0 || k < 0) return 0;return FAC[n] * FINV[k] * FINV[n - k];}
*/

template <typename T> inline bool chmax(T &a, T b) { return ((a < b) ? (a = b, true) : (false));}
template <typename T> inline bool chmin(T &a, T b) { return ((a > b) ? (a = b, true) : (false));}


struct DoubleFPS : vector<double> {
    using vector<double>::vector;
    using FPS = DoubleFPS;

    static constexpr double EPS = 1e-12;

    FPS& operator+=(const FPS& r) {
        if (r.size() > this->size()) this->resize(r.size());
        for (int i = 0; i < (int)r.size(); i++) (*this)[i] += r[i];
        return *this;
    }

    FPS& operator+=(double r) {
        if (this->empty()) this->resize(1);
        (*this)[0] += r;
        return *this;
    }

    FPS& operator-=(const FPS& r) {
        if (r.size() > this->size()) this->resize(r.size());
        for (int i = 0; i < (int)r.size(); i++) (*this)[i] -= r[i];
        return *this;
    }

    FPS& operator-=(double r) {
        if (this->empty()) this->resize(1);
        (*this)[0] -= r;
        return *this;
    }

    FPS& operator*=(double v) {
        for (double& x : *this) x *= v;
        return *this;
    }

    FPS& operator/=(double v) {
        assert(fabs(v) > EPS);
        for (double& x : *this) x /= v;
        return *this;
    }

    FPS operator+(const FPS& r) const { return FPS(*this) += r; }
    FPS operator+(double v) const { return FPS(*this) += v; }
    FPS operator-(const FPS& r) const { return FPS(*this) -= r; }
    FPS operator-(double v) const { return FPS(*this) -= v; }
    FPS operator*(const FPS& r) const { return FPS(*this) *= r; }
    FPS operator*(double v) const { return FPS(*this) *= v; }
    FPS operator/(const FPS& r) const { return FPS(*this) /= r; }
    FPS operator/(double v) const { return FPS(*this) /= v; }
    FPS operator%(const FPS& r) const { return FPS(*this) %= r; }

    FPS operator-() const {
        FPS ret(this->size());
        for (int i = 0; i < (int)this->size(); i++) ret[i] = -(*this)[i];
        return ret;
    }

    friend FPS operator+(double v, const FPS& f) { return f + v; }
    friend FPS operator-(double v, const FPS& f) { return (-f) + v; }
    friend FPS operator*(double v, const FPS& f) { return f * v; }

    void shrink() {
        while (!this->empty() && fabs(this->back()) <= EPS) this->pop_back();
    }

    FPS rev() const {
        FPS ret(*this);
        reverse(ret.begin(), ret.end());
        return ret;
    }

    FPS pre(int sz) const {
        FPS ret(this->begin(), this->begin() + min((int)this->size(), sz));
        if ((int)ret.size() < sz) ret.resize(sz, 0.0);
        return ret;
    }

    FPS operator>>(int sz) const {
        if ((int)this->size() <= sz) return {};
        FPS ret(*this);
        ret.erase(ret.begin(), ret.begin() + sz);
        return ret;
    }

    FPS operator<<(int sz) const {
        FPS ret(*this);
        ret.insert(ret.begin(), sz, 0.0);
        return ret;
    }

    FPS dot(const FPS& r) const {
        FPS ret(min(this->size(), r.size()));
        for (int i = 0; i < (int)ret.size(); i++) ret[i] = (*this)[i] * r[i];
        return ret;
    }

    // 畳み込み O(nm)
    using cd = complex<double>;

    static void fft(vector<cd>& a, bool inverse) {
        int n = (int)a.size();

        for (int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1) j ^= bit;
            j ^= bit;
            if (i < j) std::swap(a[i], a[j]);
        }

        for (int len = 2; len <= n; len <<= 1) {
            double ang = 2.0 * acos(-1.0) / len;
            if (inverse) ang = -ang;

            cd wlen(cos(ang), sin(ang));

            for (int i = 0; i < n; i += len) {
                cd w(1.0, 0.0);

                for (int j = 0; j < len / 2; j++) {
                    cd u = a[i + j];
                    cd v = a[i + j + len / 2] * w;

                    a[i + j] = u + v;
                    a[i + j + len / 2] = u - v;

                    w *= wlen;
                }
            }
        }

        if (inverse) {
            for (cd& x : a) x /= n;
        }
    }

    static FPS multiply_fft(const FPS& a, const FPS& b) {
        if (a.empty() || b.empty()) return {};

        int need = (int)a.size() + (int)b.size() - 1;

        // 小さい場合は naive の方が速いことが多い
        if (min(a.size(), b.size()) <= 60) {
            FPS ret(need, 0.0);
            for (int i = 0; i < (int)a.size(); i++) {
                for (int j = 0; j < (int)b.size(); j++) {
                    ret[i + j] += a[i] * b[j];
                }
            }
            return ret;
        }

        int n = 1;
        while (n < need) n <<= 1;

        vector<cd> fa(n), fb(n);

        for (int i = 0; i < (int)a.size(); i++) fa[i] = cd(a[i], 0.0);
        for (int i = 0; i < (int)b.size(); i++) fb[i] = cd(b[i], 0.0);

        fft(fa, false);
        fft(fb, false);

        for (int i = 0; i < n; i++) fa[i] *= fb[i];

        fft(fa, true);

        FPS ret(need);
        for (int i = 0; i < need; i++) {
            ret[i] = fa[i].real();
        }

        return ret;
    }

    // 畳み込み O(n log n)
    FPS& operator*=(const FPS& r) {
        if (this->empty() || r.empty()) {
            this->clear();
            return *this;
        }

        FPS ret = multiply_fft(*this, r);
        return *this = ret;
    }

    // 多項式除算
    FPS& operator/=(const FPS& r) {
        FPS g(r);
        g.shrink();
        assert(!g.empty());

        if (this->size() < g.size()) {
            this->clear();
            return *this;
        }

        int n = (int)this->size() - (int)g.size() + 1;

        if ((int)g.size() <= 64) {
            FPS f(*this);

            double coeff = 1.0 / g.back();
            for (double& x : g) x *= coeff;

            int deg = (int)f.size() - (int)g.size() + 1;
            int gs = (int)g.size();

            FPS quo(deg, 0.0);

            for (int i = deg - 1; i >= 0; i--) {
                quo[i] = f[i + gs - 1];
                for (int j = 0; j < gs; j++) {
                    f[i + j] -= quo[i] * g[j];
                }
            }

            *this = quo * coeff;
            this->resize(n, 0.0);
            return *this;
        }

        return *this = ((*this).rev().pre(n) * g.rev().inv(n)).pre(n).rev();
    }

    FPS& operator%=(const FPS& r) {
        *this -= (*this / r) * r;
        shrink();
        return *this;
    }

    FPS diff() const {
        int n = (int)this->size();
        FPS ret(max(0, n - 1), 0.0);
        for (int i = 1; i < n; i++) {
            ret[i - 1] = (*this)[i] * i;
        }
        return ret;
    }

    FPS integral() const {
        int n = (int)this->size();
        FPS ret(n + 1, 0.0);
        for (int i = 0; i < n; i++) {
            ret[i + 1] = (*this)[i] / (i + 1);
        }
        return ret;
    }

    double eval(double x) const {
        double r = 0.0;
        double w = 1.0;
        for (double v : *this) {
            r += w * v;
            w *= x;
        }
        return r;
    }

    // 1 / f mod x^deg
    FPS inv(int deg = -1) const {
        assert(!this->empty());
        assert(fabs((*this)[0]) > EPS);

        if (deg == -1) deg = (int)this->size();

        FPS ret({1.0 / (*this)[0]});

        for (int i = 1; i < deg; i <<= 1) {
            ret = (ret + ret - ret * ret * this->pre(i << 1)).pre(i << 1);
        }

        return ret.pre(deg);
    }

    // log f
    // 条件: f[0] = 1
    FPS log(int deg = -1) const {
        assert(!this->empty());
        assert(fabs((*this)[0] - 1.0) <= 1e-9);

        if (deg == -1) deg = (int)this->size();

        return (this->diff() * this->inv(deg)).pre(deg - 1).integral();
    }

    // exp f
    // 条件: f[0] = 0
    FPS exp(int deg = -1) const {
        assert(this->empty() || fabs((*this)[0]) <= 1e-9);

        if (deg == -1) deg = (int)this->size();

        FPS ret({1.0});

        for (int i = 1; i < deg; i <<= 1) {
            ret = (ret * (this->pre(i << 1) + 1.0 - ret.log(i << 1))).pre(i << 1);
        }

        return ret.pre(deg);
    }

    // f^k
    // ここでは k >= 0 の整数のみ
    FPS pow(long long k, int deg = -1) const {
        assert(k >= 0);

        int n = (int)this->size();
        if (deg == -1) deg = n;

        if (k == 0) {
            FPS ret(deg, 0.0);
            if (deg) ret[0] = 1.0;
            return ret;
        }

        for (int i = 0; i < n; i++) {
            if (fabs((*this)[i]) > EPS) {
                if ((long long)i * k >= deg) return FPS(deg, 0.0);

                double c = (*this)[i];

                FPS ret = (((*this / c) >> i).log(deg) * (double)k).exp(deg);
                ret *= pow_const(c, k);
                ret = (ret << (int)(i * k)).pre(deg);

                if ((int)ret.size() < deg) ret.resize(deg, 0.0);
                return ret;
            }
        }

        return FPS(deg, 0.0);
    }

    static double pow_const(double a, long long e) {
        double r = 1.0;
        while (e > 0) {
            if (e & 1) r *= a;
            a *= a;
            e >>= 1;
        }
        return r;
    }
};

using fps = DoubleFPS;
vector<int> b;
int p;

template <typename mint>
struct DivideConquerFFT {
    int n;
    vector<mint> dp;
    vector<mint> dp2;
    vector<mint> base;
    vector<mint> g;

    DivideConquerFFT(
        const vector<mint>& base_,
        const vector<mint>& g_
    ) 
        : n((int)base_.size()), dp(base_), dp2(base_), base(base_), g(g_) {}

    void solve(int l, int r) {
        if (r - l == 1){
            if(b[p-1-l]<0)
                {
                dp2[l] = dp[l];
                dp[l] = dp[l] - b[p-1-l];
                }
            else{
                    dp2[l] = dp[l];
                    if(b[p-1-l]<=l)dp[l] = dp2[l - b[p-1-l]];
                    else
                        dp[l] = 0;
                }
                
            if(l==0){
                dp[0] = 0;
                dp2[0] = 0;
            }
            cout << l<< " "<<dp[l] << " " << dp2[l] << endl;
                return;
        }

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
        return dp2;
    }
};



int SOLVEEND = 0;
void solve() {
    int n;
    cin >> n >> p;
    vl a(n);
    b.resize(p);
    double sum = 0;
    rep(i, n) cin >> a[i];
    rep(i, n) sum += a[i];
    rep(i, p) cin >> b[i];
    vector<double> base(2*p);
    vector<double> g(2*p + 1);
    rep(i, p) base[i] = 1.0;
    rep(i, p) g[i + 1] = a[i]/sum;
    DivideConquerFFT<double> df(base,g);

    // dp[0] = マスmからスタートする際の期待値(=0)
    // dp[i] = マスm-iからスタートする際の期待値
    // dp[i] = base[i] + \sum_{0<=j<i}dp[j] * g[i-j]
    // base[i] = 1, g[i-j]:サイコロで目i-jが出る確率
    // dp[m-1]が答え
    auto res = df.run();
    cout << res[p - 1] << endl;
}

signed main(){
	cin.tie(0);
	ios::sync_with_stdio(0);
	cout<<fixed<<setprecision(20);
	solve();
}