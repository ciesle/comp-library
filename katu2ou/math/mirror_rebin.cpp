// 準備 /////////////////////////////////////////////////////////////////

//乱数生成([0,n]から一様)
long long random_uniform(long long n) {
    random_device rnd; 
    mt19937_64 mt(rnd());
    uniform_int_distribution<long long> dist(0, n); //[0,n]の一様分布から生成
    return dist(mt);
}


//modの値としてll全体(正)をとれるmodpow
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


//高速素数判定(2^64以下の整数に対しての確定的アルゴリズム) (nはint型の範囲, POW_MODを使うと10^18までOK)
bool Mirror_Rebin(ll n){
    if(n == 2) return true;
    if(n % 2 == 0 || n == 1) return false;
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

///////////////////////////////////////////////////////////////////////////////////////////

//高速素数判定(通常版) 判定をroop回繰り返す。誤判定の確率は1/4^roop以下 (nはint型の範囲, POW_MODを使うと10^18までOK)
bool Mirror_Rebin_normal(ll n, int roop){
    if(n == 2) return true;
    if(n % 2 == 0 || n == 1) return false;
    bool comp = false;
    ll m = n;
    m--;
    ll rad = 0;
    while(m%2==0){
        rad++;
        m /= 2;
    }
    for(int i = 0; i < roop; i++){
        ll a = 0; 
        while(a == 0){
          a = random_uniform(n-1);
        }   
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

