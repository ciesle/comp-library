using ll = long long;
using ld = long double;
using pi = pair<int, int>;
using pl = pair<ll, ll>;
using vi = vector<int>;
using vl = vector<ll>;
#define all(a) begin(a), end(a)
#define fs first
#define sc second
#define rng(i, s, n) for(int i = (s) ; i < (n) ; i++)
#define rep(i, n) rng(i, 0, (n))
#define mkp make_pair
#define vec vector
#define pb emplace_back
#define siz(a) int((a).size())
#define crdcomp(b) sort(all((b))); (b).erase(unique(all((b))), (b).end())
#define getidx(b, i) (lower_bound(all((b)), (i))-(b).begin())
#define found(a, x) (a.find(x)!=a.end())
#define multi() int __testcase_num = read();while(__testcase_num--)
#define ssp(i, n) ((i)==(n)-1 ? "\n" : " ")
constexpr int mod = 998244353;
constexpr int Mod = 1e9 + 7;
constexpr ld EPS = 1e-10;
constexpr ll inf = (ll)3 * 1e18;
constexpr int Inf = 1e9 + 10;
constexpr int dx[] = { -1, 1, 0, 0 }, dy[] = { 0, 0, -1, 1 };
template<class T>bool chmax(T& a, const T& b) { if (a < b) { a = b; return 1; } return 0; }
template<class T>bool chmin(T& a, const T& b) { if (b < a) { a = b; return 1; } return 0; }
template<class T = ll>T read() { T u, k = scanf("%lld", &u); return u; }
template<typename T>bool ina(pair<T, T> t, T h, T w) { return 0 <= t.fs && t.fs < h && 0 <= t.sc && t.sc < w; }
ll gcd(ll i, ll j) { return j ? gcd(j, i % j) : i; }
template<typename T>void fin(T x) { cout << x << endl; exit(0); }
template<typename... T>
void debug(const T... args) { ((std::cerr << args << ' '), ...); std::cerr << '\n'; }
struct InitCppIo { InitCppIo() noexcept { std::cin.tie(nullptr);std::ios::sync_with_stdio(false); } } initCppIo;
# define likely(x)	__builtin_expect(!!(x), 1)
# define unlikely(x)	__builtin_expect(!!(x), 0)