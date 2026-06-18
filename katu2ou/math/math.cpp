<目次>
/*
桁数
GCD,LCM(c++17より前向け)
REM,QUO(負の値の時も正の値と同様に動くようにした章と余り)
POW(llでの冪乗)
proot(正の整数nの正のm乗根を求める)
a^m=nなるmを返す
MOD逆元の列挙
約数列挙
素因数分解
エラトステネスの篩と一括素因数分解
二項係数 mod
二項係数 notmod
離散対数問題
任意modでの離散対数問題
*/

//全て関数形で記述

/*memo
int の範囲は -2147483648から2147483647まで
ll の範囲は -9223372036854775808から9223372036854775807まで
doubleの精度は16桁(16桁まで保存できるということ)
*/


//桁数
ll dt(ll n){
	ll cnt=0;
	while(n>=1){ n/=10; cnt++;}
	return cnt;
}

//最大公約数と最小公倍数(c++17より前ではstdで提供されていないので必要)
ll GCD(ll a, ll b){
  if(a%b == 0){
    return b;
  }else{
    return GCD(b, a%b);
  }
}
ll LCM(ll a, ll b){
  return a*b / GCD(a, b);
}

//割り算の商と余りを、負の整数の割り算についても性の場合と同様になるようにする
ll REM(ll a, ll b){
return (a % b + b) % b;
}

ll QUO(ll a, ll b){
return (a - REM(a, b)) / b;
}

//冪乗(intの範囲を超えてllの範囲に収まる)
ll POW(ll n, ll m){
	ll ret=1;
	while(m>0){
		if(m&1) ret=ret*n;
		n=n*n;
		m>>=1;
	}
	return ret;
}

//整数nの正のm乗根が存在すれば返す(存在しなければ-1を返す) (2<=n<=10^18くらい、mは62くらいまで多分ok)
// ll proot(ll n, ll m){
//     if(m == 1)return n;
// 	ll root = (ll)round(pow(n, 1.0/m));
// 	if((ll)POW(root, m) == n) return root;
// 	else return -1;
// }

//より安全なもの(拝借 ull->llに書き替え可 k<=64)
ull kth_root(ull n, ull k) {
    if (n <= 1 || k == 1) return n;
    if (k >= 65) return 1;
    auto is_over = [&](ull x) -> bool {
        __uint128_t res = 1, a = x;
        for (int b = k; b; b >>= 1, a *= a) {
            if (b & 1) res *= a;
        }
        return res > n;
    };
    ll res = pow(n, 1.0 / k);
    while (!is_over(res + 1)) ++res;
    while ( is_over(res)) --res;
    return res;
}

//拡張ユークリッドの互除法
	//入力 ll a, b,x,y(初期値x,yはll x,y=0とする。a>=0,b>=0. a<0 or b<0でも結果は帰ってくるが，Gが所望の-1倍の可能性がある)
	//出力 ax + by = gcd(a,b) をみたす(x,y)にx,yを変化させる、d=gcd(a,b)を返す
	//(注) 返り値Gの値の符号が欲しいものと逆ならば，x,yを-1倍しないと欲しい結果が返ってこない
template< typename T >
T extgcd(T a, T b, T &x, T &y) {
  T d = a;
  if(b != 0) {
    d = extgcd(b, a % b, y, x);
    y -= (a / b) * x;
  } else {
    x = 1;
    y = 0;
  }
  return d;
}

// ax+by=d (dはgcd(a,b)の倍数と仮定する) を満たす(x,y)のうち，xが非負の最小値であるものを求める
	// (a,bは整数)


//MOD逆元の列挙
vector<long long> inv(mx + 1); 
inv[1] = 1;
  for (int i = 2; i <= mx; i++){
    inv[i] = MOD - inv[MOD % i] * (MOD / i) % MOD;
}

//約数列挙(昇順に並べたvectorで返す)
vector<ll> divisor(ll n) {
	vector<ll> ret;
	for (ll i = 1; i * i <= n; i++) {
		if (n % i != 0) continue;
		ret.push_back(i);
		//重複しないならばpush_back(n/i)をする
		if (i * i != n) ret.push_back(n / i);
	}
	sort(ret.begin(), ret.end());
	return ret;
}

//素因数分解 ((素因数,指数)のペアのvectorを返す)
vector<pair<ll, ll>> prime_factor(ll n) {
    vector<pair<ll, ll>> ret;
    for (ll i = 2; i * i <= n; i++) {
        if (n % i != 0) continue;
        ll tmp = 0;
        while (n % i == 0) {
            tmp++;
            n /= i;
        }
        ret.push_back(make_pair(i, tmp));
    }
    if (n != 1) ret.push_back(make_pair(n, 1));
    return ret;
}

//エラトステネスの篩(O(NloglogN)のやつ 添字1--nに対してisprime[i]は素数なら1を返し、
//合成数なら最小の素因数を返すvector)
//これを使った後ならN以下の素因数分解を最悪計算量O(logN)でできる。
vector<int> Eratosthenes(int N) {
    vector<int> isprime(N+1, 1);
    isprime[1] = 0;
    for (ll p = 2; p <= N; ++p) {
        if (isprime[p] != 1) continue;
        for (ll q = p * p; q <= N; q += p) {
            if(isprime[q]==1) isprime[q] = p;
        }
    }
    return isprime;
}

	//2からnまでの素因数分解を一括で行う(O(NlogN))
	vector<vector<pii>> prime_factor_all(int n){
		vector<int> isprime(n+1, 1);
		isprime[1] = 0;
		for (ll p = 2; p <= n; ++p) {
			if (isprime[p] != 1) continue;
			for (ll q = p * 2; q <= n; q += p) {
				if(isprime[q]==1) {isprime[q] = p;}
			}
		}
		vector<vector<pii>> pr(n+1);
		for(int i=2; i<=n; i++){
			if(isprime[i]==1){
				pr[i].pb({i,1});
				continue;
			}
			int tmp=i;
			while(tmp>=2){
				int u=isprime[tmp];
				if(u==1){
					pr[i].pb({tmp,1});
					break;
				}
				int cnt=0;
				while(tmp%u==0){
					cnt++;
					tmp/=u;
				}
				pr[i].pb({u,cnt});
			}
		}
		return pr;
	}

//n以下の素数を全て取得
vector<ll> get_primes(ll n){
	vector<int> isprime(N+1, 1);
	vector<int> prime;
    isprime[1] = 0;
    for (ll p = 2; p <= N; ++p) {
        if (isprime[p] != 1) continue;
        for (ll q = p * p; q <= N; q += p) {
            if(isprime[q]==1) isprime[q] = p;
        }
    }
	for(int i=2;i<=N;i++){
		if(isprime[i]==1) ret.pb(i);
	}
	return ret;
}

//m以上n以下の整数をk以下の整数で割れるだけ割る(m,n<=10^18,O((n-m)logk))
vector<vector<pair<ll,int>>> prime_factor_range(ll m, ll n, ll k){
	vector<int> isprime(k+1, 1);
	vector<int> prime;
    
    isprime[1] = 0;
    for (ll p = 2; p <= k; ++p) {
        if (isprime[p] != 1) continue;
        for (ll q = p * p; q <= k; q += p) {
            if(isprime[q]==1) isprime[q] = p;
        }
    }
	for(int i=2;i<=k;i++){
		if(isprime[i]==1) prime.pb(i);
	}
   

	vector<vector<pair<ll,int>>> ret(n-m+1);
	vector<ll> nowv(n-m+1);
	for(ll i=m;i<=n;i++){
		nowv[i-m]=i;
	}
    
	for(ll i:prime){
		ll rem=m%i;
		ll start=m;
		if(rem!=0) start=m+i-rem;
		for(ll u=start;u<=n;u+=i){
			int cnt=0;
			while(nowv[u-m]%i==0){
				cnt++;
				nowv[u-m]/=i;
			}
			ret[u-m].pb({i,cnt});
		}
	}
    
	for(ll i=m;i<=n;i++){
		if(nowv[i-m]!=1){
			ret[i-m].pb({i,1});
		}
	}
	return ret;
}


//nCm modpの求め方(n,k<=10^7までいける)
const int MAX = 510000;
const int MOD = 1000000007;

ll fac[MAX], finv[MAX], inv[MAX];
	// テーブルを作る前処理
	void COMinit() {
		fac[0] = fac[1] = 1;
		finv[0] = finv[1] = 1;
		inv[1] = 1;
		for (int i = 2; i < MAX; i++) {
			fac[i] = fac[i - 1] * i % MOD;
			inv[i] = MOD - inv[MOD%i] * (MOD / i) % MOD;
			finv[i] = finv[i - 1] * inv[i] % MOD;
		}
	}
	// 二項係数計算
	ll COM(int n, int k) {
		if (n < k) return 0;
		if (n < 0 || k < 0) return 0;
		return fac[n] * (finv[k] * finv[n - k] % MOD) % MOD;
	}

//atcoder modintに対して
	const int MAX = 510000;
	mint fac[MAX], finv[MAX], inv[MAX];

	// テーブルを作る前処理
	void COMinit() {
		const int MOD = mint::mod();
		fac[0] = fac[1] = 1;
		finv[0] = finv[1] = 1;
		inv[1] = 1;
		for (int i = 2; i < MAX; i++){
			fac[i] = fac[i - 1] * i;
			inv[i] = MOD - inv[MOD%i] * (MOD / i);
			finv[i] = finv[i - 1] * inv[i];
		}
	}
	// 二項係数計算
	mint COM(int n, int k){
		if (n < k) return 0;
		if (n < 0 || k < 0) return 0;
		return fac[n] * finv[k] * finv[n - k];
	}


//modを取らない二項係数
//前計算
ll fact[25];
fact[0]=fact[1]=1;
for(ll i=2;i<=25;i++){
	fact[i]=fact[i-1]*i;
}
ll ncom(ll n, ll m){
	if(n == 0) return 1;
	if(n < m)  return 0;
return fact[n]/(fact[m]*fact[n-m]);
}


//離散対数問題(O(√plogp)) a^x=b(mod p)を満たす非負整数xの最小値を求める(babystep-giantstep法)
//(下はint64_tで定義されているが、うまくいかなかったらllに直す)
int64_t mod_log(int64_t a, int64_t b, int64_t p) {
  int64_t g = 1;

  for(int64_t i = p; i; i /= 2) (g *= a) %= p;
  g = __gcd(g, p);

  int64_t t = 1, c = 0;
  for(; t % g; c++) {
    if(t == b) return c;
    (t *= a) %= p;
  }
  if(b % g) return -1;

  t /= g;
  b /= g;

  int64_t n = p / g, h = 0, gs = 1;

  for(; h * h < n; h++) (gs *= a) %= n;

  unordered_map< int64_t, int64_t > bs;
  for(int64_t s = 0, e = b; s < h; bs[e] = ++s) {
    (e *= a) %= n;
  }

  for(int64_t s = 0, e = t; s < n;) {
    (e *= gs) %= n;
    s += h;
    if(bs.count(e)) return c + s - bs[e];
  }
  return -1;
}

//任意modでの離散対数問題
//各素因数ごとに離散対数問題を解いて中国剰余定理で解を求める(素べきmodの場合は周期がp^{n-1}(p-1)なので
//上と同様にbabystep-giantstepができるはず)



