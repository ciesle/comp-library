/*
-   線形回帰数列(u_{n+p}=a_0u_n + ... + a_{p-1}u_{n+p-1} : p階の線形回帰数列)
    この前k項が与えられた時に第n項をO(k^2+klogklogn)で計算できる
    
    - (a_i)の母関数は，\sum_{i=0}^{\infty} a_i x^i = P(x)/(1-c_1x-c_2x^2-...-c_kx^k)
    と表せる．ここで，P(x)とc_1,...,c_kを求めるのがBerulekamp-massey アルゴリズム
    - Bostan-mori アルゴリズムは，分子分母が高々k次の多項式で表される分数P(x)/Q(x)に対し，[x^n]P(x)/Q(x)
    を求める．

    - Bostan-Mori が必要

        - nth_term(N,a) : vector<mint>型数列(a)の第N項を計算する
*/

template <typename mint>
vector<mint> BerlekampMassey(const vector<mint> &s) {
  const int N = (int)s.size();
  vector<mint> b, c;
  b.reserve(N + 1);
  c.reserve(N + 1);
  b.push_back(mint(1));
  c.push_back(mint(1));
  mint y = mint(1);
  for (int ed = 1; ed <= N; ed++) {
    int l = int(c.size()), m = int(b.size());
    mint x = 0;
    for (int i = 0; i < l; i++) x += c[i] * s[ed - l + i];
    b.emplace_back(mint(0));
    m++;
    if (x == mint(0)) continue;
    mint freq = x / y;
    if (l < m) {
      auto tmp = c;
      c.insert(begin(c), m - l, mint(0));
      for (int i = 0; i < m; i++) c[m - 1 - i] -= freq * b[m - 1 - i];
      b = tmp;
      y = x;
    } else {
      for (int i = 0; i < m; i++) c[l - 1 - i] -= freq * b[m - 1 - i];
    }
  }
  reverse(begin(c), end(c));
  return c;
}

template <typename mint>
mint LinearRecurrence(long long k, FormalPowerSeries<mint> Q,
                      FormalPowerSeries<mint> P) {
  Q.shrink();
  mint ret = 0;
  if (P.size() >= Q.size()) {
    auto R = P / Q;
    P -= R * Q;
    P.shrink();
    if (k < (int)R.size()) ret += R[k];
  }
  if ((int)P.size() == 0) return ret;

  FormalPowerSeries<mint>::set_fft();
  if (FormalPowerSeries<mint>::ntt_ptr == nullptr) {
    P.resize((int)Q.size() - 1);
    while (k) {
      auto Q2 = Q;
      for (int i = 1; i < (int)Q2.size(); i += 2) Q2[i] = -Q2[i];
      auto S = P * Q2;
      auto T = Q * Q2;
      if (k & 1) {
        for (int i = 1; i < (int)S.size(); i += 2) P[i >> 1] = S[i];
        for (int i = 0; i < (int)T.size(); i += 2) Q[i >> 1] = T[i];
      } else {
        for (int i = 0; i < (int)S.size(); i += 2) P[i >> 1] = S[i];
        for (int i = 0; i < (int)T.size(); i += 2) Q[i >> 1] = T[i];
      }
      k >>= 1;
    }
    return ret + P[0];
  } else {
    int N = 1;
    while (N < (int)Q.size()) N <<= 1;

    P.resize(2 * N);
    Q.resize(2 * N);
    P.ntt();
    Q.ntt();
    vector<mint> S(2 * N), T(2 * N);

    vector<int> btr(N);
    for (int i = 0, logn = __builtin_ctz(N); i < (1 << logn); i++) {
      btr[i] = (btr[i >> 1] >> 1) + ((i & 1) << (logn - 1));
    }
    mint dw = mint(FormalPowerSeries<mint>::ntt_pr())
                  .inverse()
                  .pow((mint::get_mod() - 1) / (2 * N));

    while (k) {
      mint inv2 = mint(2).inverse();

      // even degree of Q(x)Q(-x)
      T.resize(N);
      for (int i = 0; i < N; i++) T[i] = Q[(i << 1) | 0] * Q[(i << 1) | 1];

      S.resize(N);
      if (k & 1) {
        // odd degree of P(x)Q(-x)
        for (auto &i : btr) {
          S[i] = (P[(i << 1) | 0] * Q[(i << 1) | 1] -
                  P[(i << 1) | 1] * Q[(i << 1) | 0]) *
                 inv2;
          inv2 *= dw;
        }
      } else {
        // even degree of P(x)Q(-x)
        for (int i = 0; i < N; i++) {
          S[i] = (P[(i << 1) | 0] * Q[(i << 1) | 1] +
                  P[(i << 1) | 1] * Q[(i << 1) | 0]) *
                 inv2;
        }
      }
      swap(P, S);
      swap(Q, T);
      k >>= 1;
      if (k < N) break;
      P.ntt_doubling();
      Q.ntt_doubling();
    }
    P.intt();
    Q.intt();
    return ret + (P * (Q.inv()))[k];
  }
}

template <typename mint>
mint kitamasa(long long N, FormalPowerSeries<mint> Q,
              FormalPowerSeries<mint> a) {
  assert(!Q.empty() && Q[0] != 0);
  if (N < (int)a.size()) return a[N];
  assert((int)a.size() >= int(Q.size()) - 1);
  auto P = a.pre((int)Q.size() - 1) * Q;
  P.resize(Q.size() - 1);
  return LinearRecurrence<mint>(N, Q, P);
}


template <typename mint>
mint nth_term(long long n, const vector<mint> &s) {
  using fps = FormalPowerSeries<mint>;
  auto bm = BerlekampMassey<mint>(s);
  return kitamasa(n, fps{begin(bm), end(bm)}, fps{begin(s), end(s)});
}
