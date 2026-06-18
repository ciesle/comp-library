/*
  <エラトステネスの篩>
    - 整数nに対し，2以上n以下の整数に対するエラトステネスの篩を実行する
    - 篩を実行した際に，各整数が素数であるかのbool表/整数を割り切る最小の素数/n以下の値に対するメビウス関数値/
    
    - F(n) = \sum_{n|i} f(i) による f->Fへの変換は(約数系の)ゼータ変換
    -> f(n) = \sum_{n|i} \mobius(i/n) F(i) (これはメビウスの反転公式)

    ex) f(i) : 値がiである場合のスコア  <->  F(i) : 値がiの倍数である場合のスコアの総和
    である．Fの値が求めやすい場合に，Fからfを復元することができる．全てのFの値はO(nlogn)で
    計算できる(もっと早くできる?)
        特に，f(1) = \sum_{i=1}^n \mobius(i) F(i)
        (約数系包除原理: ARC185E など)

        - 特に，整数mに対してmの約数kに対してのみF(k)が非零の場合は，各約数kに対する
        f(k)の値をO((logk)^2)で計算できる(実装)

    [実装/関数]
        - Eratosthenes er(int n) : 1以上n以下の整数に対して篩を適用する．
            er.isprime : 素数ならtrueを，そうでないならfalseを持つ配列
            er.minfactor : 素数なら-1を(?)，そうでないならその値を割り切る2以上の最小の整数を持つ配列
        - vector<pair<int,int>> er.factorize(int n) : nの素因数分解を{素数,指数}の配列で返す
        - vector<int> er.divisor(int n) : nの約数を配列で返す
        - void fast_zeta(vector<T> f, vector<bool> isprime)
            : 配列fを(0-indexedとして)倍数ゼータ変換する．ここでisprimeはfの配列長のものを渡す．
        - void fast_mobius(vector<T> F, vector<bool> isprime)
            : 配列Fを(0-indexedとして)倍数メビウス変換する．
        - void fast_zeta_M(vector<T> f, int m, vector<pair<int,int>> fact, vector<int> divisor)
            : 配列fを(0-indexedとして)mの約数の範囲で倍数ゼータ変換する．ここでfact,divisorはmに対応するもの．
        - void fast_mobius_M(vector<T> F, int m, vector<pair<int,int>> fact, vector<int> divisor)
            : 配列Fを(0-indexedとして)mの約数の範囲で倍数ゼータ変換する．ここでfact,divisorはmに対応するもの．
                
    [計算時間]
        - isprime : O(nloglogn)
        - factorize, divisor : O(log n)
        - 
    
    [備考]
        - //https://qiita.com/drken/items/3beb679e54266f20ab63#7-%E6%B4%BB%E7%94%A8%E4%BE%8B-4-%E9%AB%98%E9%80%9F%E3%83%A1%E3%83%93%E3%82%A6%E3%82%B9%E5%A4%89%E6%8F%9B%E3%81%A8%E7%B4%84%E6%95%B0%E7%B3%BB%E5%8C%85%E9%99%A4%E5%8E%9F%E7%90%86
    
    [参照]
        - 

    [verified at]
        - 
    
    [使用例] 
        
*/

struct Eratosthenes {
    vector<bool> isprime;
    vector<int> minfactor;

    Eratosthenes(int N) : isprime(N+1, true),
                          minfactor(N+1, -1) {
        isprime[1] = false;
        minfactor[1] = 1;
        for (int p = 2; p <= N; ++p) {
            if (!isprime[p]) continue;
            minfactor[p] = p;
            for (int q = p * 2; q <= N; q += p) {
                isprime[q] = false;
                if (minfactor[q] == -1) minfactor[q] = p;
            }
        }
    }

    vector<pair<int,int>> factorize(int n) {
        vector<pair<int,int>> res;
        while (n > 1) {
            int p = minfactor[n];
            int exp = 0;

            // n で割り切れる限り割る
            while (minfactor[n] == p) {
                n /= p;
                ++exp;
            }
            res.emplace_back(p, exp);
        }
        return res;
    }  

    //高速約数列挙
    vector<int> divisors(int n) {
        vector<int> res({1});
        auto pf = factorize(n);
        for (auto p : pf) {
            int s = (int)res.size();
            for (int i = 0; i < s; ++i) {
                int v = 1;
                for (int j = 0; j < p.second; ++j) {
                    v *= p.first;
                    res.push_back(res[i] * v);
                }
            }
        }
        return res;
    }
};

// f -> F (0-indexed)
template<class T> void fast_zeta(vector<T> &f, vector<bool> &isprime) {
    int N = f.size();
    // 各素数 p 軸に対して
    // 大きい座標 (k * p) から小さい座標 (k) へと足し込む
    for (int p = 2; p < N; ++p) {
        if (!isprime[p]) continue;
        // 座標が大きい方を起点として累積和をとる
        for (int k = (N - 1) / p; k >= 1; --k) {
            f[k] += f[k * p];
        }
    }
}

// F -> f (0-indexed)
template<class T> void fast_mobius(vector<T> &F, vector<bool> &isprime) {
    int N = F.size();
    // 各素数 p 軸に対して
    // 小さい座標 (k) から大きい座標 (k * p) を引いていく
    for (int p = 2; p < N; ++p) {
        if (!isprime[p]) continue;
        // 座標が小さい方を起点として差分をとる
        for (int k = 1; k * p < N; ++k) {
            F[k] -= F[k * p];
        }
    }
}

// f -> F (ある正の整数Mの約数に対して)
// Mを素因数分解したものと，Mの約数列を与える
template<class T> void fast_zeta_M(vector<T> &f, int M, vector<pair<int,int>> &fact, vector<int> &divisor){
    sort(divisor.begin(), divisor.end());
    int N = f.size();
    if(N-1 < M) return;
    int sz = fact.size();
    for(int i=0; i<sz; i++){
        for(int j=divisor.size()-1; j>=0; j--){
            if(M%(divisor[j]*fact[i].first)==0){
                f[divisor[j]] += f[divisor[j]*fact[i].first];
            }
        }
    }
} 


// F -> f (ある正の整数Mの約数に対して)
// Mを素因数分解したものと，Mの約数列を与える
template<class T> void fast_mobius_M(vector<T> &F, int M, vector<pair<int,int>> &fact, vector<int> &divisor){
    sort(divisor.begin(), divisor.end());
    int N = F.size();
    if(N-1 < M) return;
    int sz = fact.size();
    for(int i=0; i<sz; i++){
        for(int j=0; j<=divisor.size()-1; j++){
            if(M%(divisor[j]*fact[i].first)==0){
                F[divisor[j]] -= F[divisor[j]*fact[i].first];
            }
        }
    }
} 

//Fからf(1)を求める