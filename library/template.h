#pragma once
#include <bits/stdc++.h>
#include <immintrin.h>
#pragma GCC target("avx2")
#ifdef ONLINE_JUDGE
//#pragma GCC target("O3")
#endif
//#include<atcoder/all> 
//using atcoder::two_sat, atcoder::mcf_graph, atcoder::mf_graph;
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
#define multi(n) int __testcase_num = (n);while(__testcase_num--)
#define ssp(i, n) ((i)==(n)-1 ? "\n" : " ")
constexpr int mod = 998244353;
constexpr int Mod = 1e9 + 7;
constexpr ld EPS = 1e-10;
constexpr ll inf = (ll)3 * 1e18;
constexpr int Inf = 1e9 + 10;
constexpr pair<int, int> dxy[] = { {-1,0}, {0,1}, {1,0}, {0,-1} };
template<class T>bool chmax(T& a, const T& b) { if (a < b) { a = b; return 1; } return 0; }
template<class T>bool chmin(T& a, const T& b) { if (b < a) { a = b; return 1; } return 0; }
template<class T = ll>T read() { T u, k = scanf("%lld", &u); return u; }
template<typename T>bool ina(pair<T, T> t, T h, T w) { return 0 <= t.fs && t.fs < h && 0 <= t.sc && t.sc < w; }
ll gcd(ll i, ll j) { return j ? gcd(j, i % j) : i; }
template<typename T>void fin(T x) { cout << x << endl; exit(0); }
# define likely(x)	__builtin_expect(!!(x), 1)
# define unlikely(x)	__builtin_expect(!!(x), 0)
template<typename itr> void printvec(itr begin, itr end) {
	for (itr p = begin; p < end; p++){
		if(p!=begin)cout<<" "; cout << *p;
	}
	cout << endl;
}
template <class T> T makevec(T value) {return value;}
template <class T, class... Args,
		std::enable_if_t<(sizeof...(Args) > 0), int> = 0>
auto makevec(std::size_t n, Args... args) {
	auto inner = makevec<T>(args...);
	return std::vector<decltype(inner)>(n, inner);
}