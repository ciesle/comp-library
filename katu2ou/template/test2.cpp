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
    /// [0, n] の一様乱数を返す
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



namespace internal {

template <class T> struct simple_queue {
    std::vector<T> payload;
    int pos = 0;
    void reserve(int n) { payload.reserve(n); }
    int size() const { return int(payload.size()) - pos; }
    bool empty() const { return pos == int(payload.size()); }
    void push(const T& t) { payload.push_back(t); }
    T& front() { return payload[pos]; }
    void clear() {
        payload.clear();
        pos = 0;
    }
    void pop() { pos++; }
};

}  // namespace internal

template <class Cap> struct mf_graph {
  public:
    mf_graph() : _n(0) {}
    mf_graph(int n) : _n(n), g(n) {}

    int add_edge(int from, int to, Cap cap) {
        assert(0 <= from && from < _n);
        assert(0 <= to && to < _n);
        assert(0 <= cap);
        int m = int(pos.size());
        pos.push_back({from, int(g[from].size())});
        int from_id = int(g[from].size());
        int to_id = int(g[to].size());
        if (from == to) to_id++;
        g[from].push_back(_edge{to, to_id, cap});
        g[to].push_back(_edge{from, from_id, 0});
        return m;
    }

    struct edge {
        int from, to;
        Cap cap, flow;
    };

    edge get_edge(int i) {
        int m = int(pos.size());
        assert(0 <= i && i < m);
        auto _e = g[pos[i].first][pos[i].second];
        auto _re = g[_e.to][_e.rev];
        return edge{pos[i].first, _e.to, _e.cap + _re.cap, _re.cap};
    }
    std::vector<edge> edges() {
        int m = int(pos.size());
        std::vector<edge> result;
        for (int i = 0; i < m; i++) {
            result.push_back(get_edge(i));
        }
        return result;
    }
    void change_edge(int i, Cap new_cap, Cap new_flow) {
        int m = int(pos.size());
        assert(0 <= i && i < m);
        assert(0 <= new_flow && new_flow <= new_cap);
        auto& _e = g[pos[i].first][pos[i].second];
        auto& _re = g[_e.to][_e.rev];
        _e.cap = new_cap - new_flow;
        _re.cap = new_flow;
    }

    Cap flow(int s, int t) {
        return flow(s, t, std::numeric_limits<Cap>::max());
    }
    Cap flow(int s, int t, Cap flow_limit) {
        assert(0 <= s && s < _n);
        assert(0 <= t && t < _n);
        assert(s != t);

        std::vector<int> level(_n), iter(_n);
        internal::simple_queue<int> que;

        auto bfs = [&]() {
            std::fill(level.begin(), level.end(), -1);
            level[s] = 0;
            que.clear();
            que.push(s);
            while (!que.empty()) {
                int v = que.front();
                que.pop();
                for (auto e : g[v]) {
                    if (e.cap == 0 || level[e.to] >= 0) continue;
                    level[e.to] = level[v] + 1;
                    if (e.to == t) return;
                    que.push(e.to);
                }
            }
        };
        auto dfs = [&](auto self, int v, Cap up) {
            if (v == s) return up;
            Cap res = 0;
            int level_v = level[v];
            for (int& i = iter[v]; i < int(g[v].size()); i++) {
                _edge& e = g[v][i];
                if (level_v <= level[e.to] || g[e.to][e.rev].cap == 0) continue;
                Cap d =
                    self(self, e.to, std::min(up - res, g[e.to][e.rev].cap));
                if (d <= 0) continue;
                g[v][i].cap += d;
                g[e.to][e.rev].cap -= d;
                res += d;
                if (res == up) break;
            }
            return res;
        };

        Cap flow = 0;
        while (flow < flow_limit) {
            bfs();
            if (level[t] == -1) break;
            std::fill(iter.begin(), iter.end(), 0);
            while (flow < flow_limit) {
                Cap f = dfs(dfs, t, flow_limit - flow);
                if (!f) break;
                flow += f;
            }
        }
        return flow;
    }

    std::vector<bool> min_cut(int s) {
        std::vector<bool> visited(_n);
        internal::simple_queue<int> que;
        que.push(s);
        while (!que.empty()) {
            int p = que.front();
            que.pop();
            visited[p] = true;
            for (auto e : g[p]) {
                if (e.cap && !visited[e.to]) {
                    visited[e.to] = true;
                    que.push(e.to);
                }
            }
        }
        return visited;
    }

    struct residual_edge {
        int to;
        Cap cost; // 残余容量
    };

    std::vector<std::vector<residual_edge>> residual_graph() const {
        std::vector<std::vector<residual_edge>> res(_n);

        for (int v = 0; v < _n; v++) {
            for (const auto& e : g[v]) {
                if (e.cap > 0) {
                    res[v].push_back(residual_edge{e.to, e.cap});
                }
            }
        }

        return res;
    }

  private:
    int _n;
    struct _edge {
        int to, rev;
        Cap cap;
    };
    std::vector<std::pair<int, int>> pos;
    std::vector<std::vector<_edge>> g;
};

void solve(){
    mf_graph<int> g(4);
    g.add_edge(0, 1, 2);
    g.add_edge(0, 2, 3);
    g.add_edge(2, 3, 1);
    g.flow(0, 3);
    auto G = g.residual_graph();
    rep(i,4){
        for(auto pr:G[i]){
            cout << i << " " << pr.to << " " << pr.cost << endl;
        }
    }
}

signed main(){
	cin.tie(0);
	ios::sync_with_stdio(0);
	cout<<fixed<<setprecision(20);
        solve();
}