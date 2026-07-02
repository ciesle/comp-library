/*
  <Bostan-Mori / 線形漸化式の第 N 項計算>
    - 有理型母関数 P(x) / Q(x) の x^k の係数 [x^k] P(x)/Q(x) を高速に求める
    - 線形漸化式を満たす数列の第 N 項計算に使える
    - formal-power-series.cpp が必要

    [概要]
        - 数列 a_0, a_1, a_2, ... が k 階の線形漸化式を満たすとする．

        - 例えば，Q_0 = 1 として

              Q_0 a_n + Q_1 a_{n-1} + Q_2 a_{n-2} + ... + Q_k a_{n-k} = 0

          がすべての n >= k で成り立つとする．

        - このとき，数列の母関数 A(x) = Σ a_n x^n は

              A(x) = P(x) / Q(x)

          の形に書ける．

        - Bostan-Mori は，この有理型母関数 P(x)/Q(x) に対して，
          第 N 項，すなわち [x^N] P(x)/Q(x) を高速に求めるアルゴリズムである．

    [実装/関数]
        - mint LinearRecurrence(long long k, FormalPowerSeries<mint> Q,
                                FormalPowerSeries<mint> P)

            有理型母関数 P(x) / Q(x) の x^k の係数を返す．

            つまり，

                [x^k] P(x) / Q(x)

            を求める．

            P, Q は FormalPowerSeries<mint> 型で与える．

        - mint kitamasa(long long N, FormalPowerSeries<mint> Q,
                        FormalPowerSeries<mint> a)

            線形漸化式の第 N 項 a_N を返す．

            Q は漸化式を表す多項式で，

                Q_0 a_n + Q_1 a_{n-1} + ... + Q_k a_{n-k} = 0
                Q_0 = 1

            を満たすように与える．

            a は数列の初期項であり，少なくとも k 項必要．
            つまり，Q.size() = k + 1 のとき，a.size() >= k が必要．

    [計算時間]
        - LinearRecurrence
            O(M(k) log N)

          ここで k は Q の次数程度，
          M(k) は k 次程度の多項式積の計算時間．

        - NTT が使える場合
            おおよそ O(k log k log N)

        - NTT が使えない場合
            多項式積の実装に依存する

    [要件]
        - mint は modint 型を想定
        - FormalPowerSeries<mint> が定義されていること
        - FormalPowerSeries<mint> が以下の操作を持つこと
            - +, -, *, /
            - inv()
            - pre()
            - resize()
            - shrink()
            - ntt()
            - intt()
            - ntt_doubling()
            - set_fft()
            - ntt_ptr
            - ntt_pr()

        - Q[0] != 0 であること
        - kitamasa では Q[0] = 1 を想定している

    [備考]
        - N は 0-indexed
          つまり，kitamasa(0, Q, a) は a_0 を返す．

        - kitamasa では，初期項 a と Q から分子 P を

              P = A(x) Q(x) mod x^k

          として作り，LinearRecurrence に渡している．

        - 関数名 kitamasa となっているが，内部では Bostan-Mori により第 N 項を求めている．

        - Berlekamp-Massey と組み合わせると，
          数列の先頭項から最小線形漸化式を推定し，
          その第 N 項を高速に求められる．

        - P.size() >= Q.size() の場合にも対応しており，
          最初に多項式除算で整式部分を取り出してから処理している．

        - 線形漸化式の典型例:
            - Fibonacci 数列
            - 線形 DP
            - 行列累乗で表される数列
            - 定数係数線形漸化式

    [使用例]
        // Fibonacci 数列
        // a_0 = 0, a_1 = 1
        // a_n = a_{n-1} + a_{n-2}

        // 漸化式:
        // a_n - a_{n-1} - a_{n-2} = 0

        // Q(x) = 1 - x - x^2
        // a = {0, 1}

        FormalPowerSeries<mint> Q = {1, -1, -1};
        FormalPowerSeries<mint> a = {0, 1};

        cout << kitamasa(N, Q, a) << endl;

    [参照]
        - Bostan-Mori algorithm
        - formal-power-series.cpp

    [verified at]
        -
*/
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
