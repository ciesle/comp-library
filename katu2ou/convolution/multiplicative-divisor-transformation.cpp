/*
  <倍数変換 / 約数変換>
    - (約数/倍数)(ゼータ/メビウス)変換をO(NloglogN)で行う

    - 約数変換 : g(n)は，nの全ての約数dに対するf(d)の値の総和
    - 倍数変換 : g(n)は，nの全ての倍数mに対するf(m)の値の総和

    [実装/関数]
        - void divisor_transform::zeta_transform(vector<T> f) : fを約数ゼータ変換する(fをgに)
            - void divisor_transform::zeta_transform(map<ll, T> f) : i|nに対してf(i)が与えられた時にg(j)を求める
                (mapには，(i,f(i))が入っている)
        - void divisor_transform::mobius_transform(vector<T> g) : gを約数メビウス変換する(gをfに)
        - void multiple_transform::zeta_transform(vector<T> f) : fを倍数ゼータ変換する(fをgに)
        - void multiple_transform::mobius_transform(vector<T> g) : gを倍数メビウス変換する(gをfに)
        
        - 添字は0-indexedとして扱う

    [計算時間]
        - vector<T> に対して O(NloglogN)
        - map<ll, T> に対して O(\sigma(N)^2)
    
    [備考]
        - Burnsideの補題 (集合Xに群Gが作用している時，(軌道の個数) = \sum(gが固定するXの元の個数)/|G|)
    
    [参照]
        - https://nyaannyaan.github.io/library/multiplicative-function/divisor-multiple-transform.hpp.html

    [verified at]
        - https://atcoder.jp/contests/abc162/tasks/abc162_e
        - https://yukicoder.me/submissions/1045682 (Burnsideの補題の使用例)
        
    
  <gcd畳み込み>
    - 1以上N以下の整数に対し値を持つ配列f,gに対して
        h(n) = \sum_{gcd(i,j)=n} f(i)g(j)
    により配列hを計算するのがgcd畳み込み 

    [実装/関数]
        - vector<T> gcd_convolution(const vector<T>& a, const vector<T>& b) : 
            a,bのgcd畳み込みを求める
        
        - 添字は0-indexedで扱う
    
    [計算時間]

    [参照]

    [verified at]

*/

vector<int> prime_enumerate(int N) {
    vector<bool> sieve(N / 3 + 1, 1);
    for (int p = 5, d = 4, i = 1, sqn = sqrt(N); p <= sqn; p += d = 6 - d, i++) {
      if (!sieve[i]) continue;
      for (int q = p * p / 3, r = d * p / 3 + (d * p % 3 == 2), s = 2 * p,
               qe = sieve.size();
           q < qe; q += r = s - r)
        sieve[q] = 0;
    }
    vector<int> ret{2, 3};
    for (int p = 5, d = 4, i = 1; p <= N; p += d = 6 - d, i++)
      if (sieve[i]) ret.push_back(p);
    while (!ret.empty() && ret.back() > N) ret.pop_back();
    return ret;
}

struct divisor_transform {
    template <typename T>
    static void zeta_transform(vector<T> &a) {
      int N = a.size() - 1;
      auto sieve = prime_enumerate(N);
      for (auto &p : sieve)
        for (int k = 1; k * p <= N; ++k) a[k * p] += a[k];
    }
    template <typename T>
    static void mobius_transform(vector<T> &a) {
      int N = a.size() - 1;
      auto sieve = prime_enumerate(N);
      for (auto &p : sieve)
        for (int k = N / p; k > 0; --k) a[k * p] -= a[k];
    }
  
    template <typename I, typename T>
    static void zeta_transform(map<I, T> &a) {
      for (auto p = rbegin(a); p != rend(a); p++)
        for (auto &x : a) {
          if (p->first == x.first) break;
          if (p->first % x.first == 0) p->second += x.second;
        }
    }
    template <typename I, typename T>
    static void mobius_transform(map<I, T> &a) {
      for (auto &x : a) {
        for (auto p = rbegin(a); p != rend(a); p++) {
          if (x.first == p->first) break;
          if (p->first % x.first == 0) p->second -= x.second;
        }
      }
    }
};
  
  struct multiple_transform {
    template <typename T>
    static void zeta_transform(vector<T> &a) {
      int N = a.size() - 1;
      auto sieve = prime_enumerate(N);
      for (auto &p : sieve)
        for (int k = N / p; k > 0; --k) a[k] += a[k * p];
    }
    template <typename T>
    static void mobius_transform(vector<T> &a) {
      int N = a.size() - 1;
      auto sieve = prime_enumerate(N);
      for (auto &p : sieve)
        for (int k = 1; k * p <= N; ++k) a[k] -= a[k * p];
    }
  
    template <typename I, typename T>
    static void zeta_transform(map<I, T> &a) {
      for (auto &x : a)
        for (auto p = rbegin(a); p->first != x.first; p++)
          if (p->first % x.first == 0) x.second += p->second;
    }
    template <typename I, typename T>
    static void mobius_transform(map<I, T> &a) {
      for (auto p1 = rbegin(a); p1 != rend(a); p1++)
        for (auto p2 = rbegin(a); p2 != p1; p2++)
          if (p2->first % p1->first == 0) p1->second -= p2->second;
    }
};

template <typename mint>
vector<mint> gcd_convolution(const vector<mint>& a, const vector<mint>& b) {
  assert(a.size() == b.size());
  auto s = a, t = b;
  multiple_transform::zeta_transform(s);
  multiple_transform::zeta_transform(t);
  for (int i = 0; i < (int)a.size(); i++) s[i] *= t[i];
  multiple_transform::mobius_transform(s);
  return s;
}

// template <typename mint>
// vector<mint> lcm_convolution(const vector<mint>& a, const vector<mint>& b) {
//   assert(a.size() == b.size());
//   auto s = a, t = b;
//   divisor_transform::zeta_transform(s);
//   divisor_transform::zeta_transform(t);
//   for (int i = 0; i < (int)a.size(); i++) s[i] *= t[i];
//   divisor_transform::mobius_transform(s);
//   return s;
// }
