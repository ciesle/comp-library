#include <algorithm>
#include <array>
#include <atomic>
#include <bitset>
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfenv>
#include <cfloat>
#include <chrono>
#include <climits>
#include <clocale>
#include <cmath>
#include <complex>
#include <condition_variable>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <exception>
#include <filesystem>
#include <forward_list>
#include <fstream>
#include <functional>
#include <future>
#include <initializer_list>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <mutex>
#include <new>
#include <numeric>
#include <optional>
#include <ostream>
#include <queue>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <set>
#include <shared_mutex>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <string_view>
#include <system_error>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <valarray>
#include <variant>
#include <vector>
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

using ll = long long;
using vi = vector<int>;
using vl = vector<long long>;
using pii = pair<int, int>;
using pll = pair<long long, long long>;
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
/*
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> dist1(1, 100); [1,100]
int random_value = dist1(gen);
*/
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

/////////////////////////////////////////////////////////////////////////////////////////
int SOLVEFIN = 0;

void solve(){
    
}

signed main(){
	cin.tie(0);
	ios::sync_with_stdio(0);
	cout<<fixed<<setprecision(20);
	while(SOLVEFIN == 0) solve();
}