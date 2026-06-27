/*
  <Berlekamp-Massey / Bostan-Mori による線形漸化式の第 N 項計算>

    - 線形漸化式で表される数列の第 N 項を高速に求めるためのライブラリ

    - 数列 a_0, a_1, a_2, ... が，ある k に対して

          a_{n+k} = c_1 a_{n+k-1} + c_2 a_{n+k-2} + ... + c_k a_n

      の形の線形漸化式を満たすとき，先頭の十分な項から最小線形漸化式を推定し，
      その漸化式を用いて第 N 項を計算する．

    - 主に次の 2 段階からなる．

        1. Berlekamp-Massey
            - 与えられた数列 s から，それを生成する最小線形漸化式を求める
            - 返り値は特性多項式 Q(x) に対応する

        2. Bostan-Mori
            - 有理型母関数 P(x) / Q(x) に対して，[x^N] P(x) / Q(x) を高速に求める
            - 線形漸化式の第 N 項計算に対応する

    [母関数との関係]
        - 線形漸化式を満たす数列 a_n の母関数は

              A(x) = Σ_{n>=0} a_n x^n = P(x) / Q(x)

          の形に書ける．

        - ここで Q(x) は漸化式から定まる多項式で，

              Q(x) = 1 - c_1 x - c_2 x^2 - ... - c_k x^k

          の形になる．

        - Berlekamp-Massey は，与えられた数列からこの Q(x) を求める．
        - Bostan-Mori は，P(x), Q(x) から [x^N] P(x)/Q(x) を求める．

    [実装/関数]
        - vector<mint> BerlekampMassey(vector<mint> s)
            数列 s から最小線形漸化式の特性多項式 Q(x) を求める．

            返り値 bm は，FormalPowerSeries として使うことを想定しており，
            bm[0] = 1 となる形の多項式である．

        - mint LinearRecurrence(long long k, FormalPowerSeries<mint> Q,
                                FormalPowerSeries<mint> P)
            有理型母関数 P(x) / Q(x) の x^k の係数を求める．

            つまり，

                [x^k] P(x) / Q(x)

            を返す．

            内部では Bostan-Mori を用いる．
            NTT が使える場合は高速な分岐を用い，そうでない場合も通常の多項式積で動作する．

        - mint kitamasa(long long N, FormalPowerSeries<mint> Q,
                        FormalPowerSeries<mint> a)
            特性多項式 Q(x) と初期項 a から，数列の第 N 項を求める．

            名前は kitamasa だが，内部では母関数 P(x)/Q(x) を作り，
            LinearRecurrence に渡している．

        - mint nth_term(long long n, vector<mint> s)
            数列 s の先頭項から Berlekamp-Massey で線形漸化式を推定し，
            第 n 項を返す．

            使う側は基本的にこの関数を呼べばよい．

    [計算時間]
        - BerlekampMassey
            O(L^2)
            ここで L = s.size()

        - LinearRecurrence / Bostan-Mori
            多項式の次数を k として，おおよそ

                O(M(k) log N)

            ここで M(k) は k 次程度の多項式積の計算時間．
            NTT が使える場合は高速．

        - nth_term
            s.size() = L，最小漸化式の次数を k とすると，

                O(L^2 + M(k) log N)

    [要件]
        - mint は四則演算，逆元，pow などを持つ modint 型であること
        - FormalPowerSeries<mint> が定義されていること
        - FormalPowerSeries が以下の操作に対応していること
            - +, -, *, /, inv()
            - resize()
            - shrink()
            - pre()
            - ntt(), intt()
            - ntt_doubling()
            - set_fft()
            - ntt_ptr
            - ntt_pr()

    [備考]
        - nth_term(n, s) に渡す s は，線形漸化式を推定するために十分な長さが必要．
          一般には，最小漸化式の次数を k とすると 2k 項程度あるとよい．

        - s が短すぎる場合，Berlekamp-Massey が本来の漸化式より低い次数の式を推定することがある．

        - n は 0-indexed．
          つまり nth_term(0, s) は a_0 を返す．

        - 典型的な用途:
            - フィボナッチ数列の第 N 項
            - 線形 DP の第 N 項
            - 遷移行列の累乗で表される数列
            - 畳み込みや Kitamasa 法で扱う線形漸化式

        - Berlekamp-Massey は，有限体上での使用を想定している．
          mod が素数の modint で使うのが自然．

        - Bostan Moriが必要

    [verified at]
        -

    [使用例]
        // Fibonacci:
        // a_0 = 0, a_1 = 1, a_n = a_{n-1} + a_{n-2}

        vector<mint> s = {0, 1, 1, 2, 3, 5, 8, 13};
        cout << nth_term<mint>(N, s) << endl;
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
