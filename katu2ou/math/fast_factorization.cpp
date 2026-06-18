// 準備 /////////////////////////////////////////////////////////////////
/*
ll GCD(ll a, ll b){
  if(a%b == 0){
    return b;
  }else{
    return GCD(b, a%b);
  }
}
*/

//乱数生成([0,n]から一様)
long long random_uniform(long long n) {
    random_device rnd; 
    mt19937_64 mt(rnd());
    uniform_int_distribution<long long> dist(0, n); //[0,n]の一様分布から生成
    return dist(mt);
}

//　構造体として持っておくと楽

struct RNG {
    std::mt19937_64 eng;            // 64bit Mersenne Twister

    RNG() : eng(std::random_device{}()) {}   // ここでだけ OS 乱数を消費

    /// [0, n] の一様乱数を返す
    long long random_uniform(long long n) {
        std::uniform_int_distribution<long long> dist(0, n);
        return dist(eng);
    }
};

/*
ll rand_int(ll l, ll r) { //[l, r]
	//#ifdef LOCAL
	static mt19937_64 gen;
	/*#else
	static mt19937_64 gen(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	return uniform_int_distribution<ll>(l, r)(gen);
*/

ll Modpow(__int128_t a, ll n, ll mo) {
	__int128_t r=1;
	a%=mo;
	while(n) r=r*((n%2)?a:1)%mo,a=a*a%mo,n>>=1;
	return r;
}

//modの値としてll全体(正)をとれる掛け算
ll Multiple(__int128_t a, __int128_t b, ll mo) {
    __int128_t r=0;
    a%=mo;
    b%=mo;
    r=a*b;
    r%=mo;
    return r;
}

//高速素数判定(2^64以下の整数に対しての確定的アルゴリズム) 
bool Mirror_Rebin(ll n){
    if(n == 2) return true;
    if(n % 2 == 0) return false;
    bool comp = false;
    ll m = n;
    m--;
    ll rad = 0;
    while(m % 2 == 0){
        rad++;
        m /= 2;
    }
    //vector<ll> vec = {2, 13, 23, 1662803}; //nがintの範囲ならこれで十分
    vector<ll> vec = {2,3,5,7,11,13,17,19,23,29,31,37}; //nが64bit用
    for(auto g: vec){
        if(n == g) return true;
    }
    for(int i = 0; i < vec.size(); i++){
        ll a = vec[i];
        ll val = Modpow(a,m,n);
        if(val == 1) continue;
        bool fin = false;
        for(int j = 0; j < rad; j++){
            if(val == n-1){
                fin = true;
                continue;
            }
            val = Modpow(val,(ll)2,n);
        }
        if(!fin) return false;
    }
    return true;
} 

//高速素因数分解(ポラードのρ法と併せてO(N^1/4)で素因数分解する)
vector<pair<ll, ll>> Fast_Factrization(ll n){
    vector<pair<ll, ll>> prime;
    vector<ll> small_prime = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
    for(auto g: small_prime){
        if(n % g == 0){
            ll cnt = 0;
            while(n % g == 0){
                n /= g;
                cnt++;
            }
            prime.push_back(make_pair(g,cnt));
        }
    }
    while(true){
        if(n == 1) return prime;
        if(Mirror_Rebin(n)){
            prime.push_back(make_pair(n,1));
            return prime;
        }
        
        for(ll c = 1; c < n; c++){ //ρ法の部分
            ll y = random_uniform(n-1);
            ll r = 1;
            ll k = 0;
            ll x = y;
            int fin = 0;
            while(true){
                while(k < r){
                    k = k + 1;
                    y = (Modpow(y, (ll)2, n) + c) % n;
                    ll g = gcd(abs(x-y), n);
                    if(g != 1){
                        if(Mirror_Rebin(g)){
                            fin = 1;
                            ll cnt = 0;
                            while(n % g == 0){
                                cnt++;
                                n /= g;
                            }
                            prime.push_back(make_pair(g,cnt));
                        }
                        else fin = 2;
                        break;
                    }
                }
                if(fin != 0) break;
                r = 2*r;
                x = y;
            }
            if(fin == 2) continue;
            if(fin == 1) break;
        }
    }
}
