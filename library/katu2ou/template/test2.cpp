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

/////////////////////////////////////////////////////////////////////////////////////////
int SOLVEFIN = 0;

string trueans = "2";
int truedist = inf;

void dfs(int n, int m, string&t, vector<string> &s, string &now){
    if(now.size()>t.size())return;
    if(now.size()==t.size()){
        vi pos1;
        vi pos2;
        rep(i,now.size()){
            if(now[i]=='1')
                pos1.PB(i);
            if(t[i]=='1')
                pos2.PB(i);
        }
        if(pos1.size()!=pos2.size()){
            return;
        }
        int dist = 0;
        rep(i, pos1.size()) {
            dist += abs(pos1[i] - pos2[i]);
        }
        if(truedist>dist){
            truedist = dist;
            trueans = now;
        }
        else if(truedist==dist){
            chmin(trueans, now);
        }
        return;
    }
    rep(i,m){
        now += s[i];
        dfs(n, m, t, s, now);
        rep(j,s[i].size()){
            now.pop_back();
        }
    }
    return;
}

void solve() {
    trueans = "2";
    truedist = inf;
    int m ;
    cin >> m;
    if (m == 0) {
        SOLVEFIN = 1;
        return;
    }
    vector<string> s(m);
    vector<int> cnt1(m);
    vector<string> s1;

    int t1 = 0;
    rep(i, m) {
        cin >> s[i];
        rep(j,s[i].size()){
            if(s[i][j]=='1')
                cnt1[i]++;
        }
    }
    string t;
    cin >> t;
    rep(i,t.size()){
        if(t[i]=='1')
            t1++;
    }
    int n = t.size();
    vi pos1t;
    rep(i,n){
        if(t[i]=='1'){
            pos1t.PB(i);
        }
    }
 

    vector<vvi> cost(n, vvi(m, vi(t1+1)));

    rep(k,m){
        vi poss;
        int cnt = cnt1[k];
        for (int i = 0; i < s[k].size();i++){
            if(s[k][i]=='1')
                poss.PB(i);
        }
        rep(l, t1+1) {
            if (l + cnt - 1 > t1 - 1)
                continue;
            int slope = 0;
            vi change(cnt);
            vector<int> waitque;
            int nowcost = 0;
            
            for (int j = 0; j < cnt; j++) {
                //cout << cnt << " " << k << " "<<poss.size()<<endl;
                change[j] = poss[j] - pos1t[l + j];
                // cout << "A" << endl;
                nowcost += abs(change[j]);
                  //cout << j << " "<<cnt<<endl;
                  //cout << "A" << endl;
                if(change[j]<=0){
                    slope--;
                    waitque.PB(change[j]);
                } else
                    slope++;
            }
            //cout << "B" << endl;
            sort(all(waitque));


            //cout << k << " " << l << endl;
            for (int j = 0; j < n - s[k].size() + 1; j++) {
                cost[j][k][l] = nowcost;

                // slopeの更新(0以上) -> nowcostの更新
                while(waitque.size()>=1 && waitque[waitque.size()-1]+j>=0){
                    waitque.pop_back();
                    slope += 2;
                }
                nowcost += slope;
            }
        }
    }


    //dpを作成
    vvi dp(n + 1, vi(n + 1, inf));
    dp[0][0] = 0;

    for (int i = 0;i<=n;i++){
        for (int j = 0; j <= t1;j++){
            rep(k,m){
                int ni = i-s[k].size();
                int nj = j - cnt1[k];
                if(ni<0 || nj<0)
                    continue;
                chmin(dp[i][j], dp[ni][nj] + cost[ni][k][nj]);
                assert(cost[ni][k][nj] >= 0);
                // cout << i << " " << j << " " << dp[i][j] << " " << k << "
                // "<<cost[ni][k][nj]<<endl;
            }
        }
    }

    vector<vpii> G((n + 1) * (n + 1)); //最適のみからなるグラフ to/辺に対応する文字列
    for (int i = 0;i<=n;i++){
        for (int j = 0; j <= t1;j++){
            rep(k,m){
                int ni = i-s[k].size();
                int nj = j - cnt1[k];
                if(ni<0 || nj<0)
                    continue;
                if(cost[ni][k][nj]+dp[ni][nj]==dp[i][j] && dp[ni][nj]!=inf){
                    G[ni * (n + 1) + nj].PB({i * (n + 1) + j,k});
                }
            }
        }
    }

    vector<string> dp2((n+1)*(n+1),"2");
    dp2[0] = "";
    for (int ind = 0;ind<(n+1)*(n+1);ind++){
        for(pii e: G[ind]){
            chmin(dp2[e.first], dp2[ind] + s[e.second]);
        }
    }
    if(dp2[n*(n+1)+t1] == "2"){
        cout << "IMPOSSIBLE" << endl;
    }
    else
        cout << dp2[n * (n + 1) + t1] << endl;

    // string emp = "";
   
    // dfs(n, m, t, s, emp);
    
    // cout << trueans << endl;

    // if(trueans!=dp2[n*(n+1)+t1]){
    //     cout<<"FALSE"<<endl;
    //     cout << trueans << endl;
    //     cout << dp2[(n * (n + 1)) + t1] << endl;
    // }
}

signed main(){
	cin.tie(0);
	ios::sync_with_stdio(0);
	cout<<fixed<<setprecision(20);
        int term = 0;
        while (SOLVEFIN == 0){
            term++;
            //cout << term << endl;
            solve();
        }
           
}