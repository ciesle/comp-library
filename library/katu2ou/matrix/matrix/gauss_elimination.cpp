/*
<Gauss Elimination / ガウス消去法>
    行列 a に対してガウス消去法を行い，階数 rank を求める．
    行列 a は破壊的に変形される．

    T は四則演算，特に除算 / が定義された型である必要がある．
    典型的には double, long double, modint などで使う．
    整数型 int, long long では一般には正しく使えないので注意する．

[実装/関数]
    - pair<int, T> GaussElimination(vector<vector<T>> &a,
                                    int pivot_end = -1,
                                    bool diagonalize = false)

        : 行列 a にガウス消去法を行う．
          返り値は {rank, det}．

        : rank は，pivot_end 未満の列を係数部分とみなしたときの階数．

        : pivot_end は，ピボットを探す列の終端を表す．
          ピボット候補として使う列は [0, pivot_end)．
          pivot_end == -1 のときは，すべての列をピボット候補にする．

        : pivot_end より右側の列も，行基本変形に合わせて同時に変形される．
          したがって，拡大係数行列 [A | b] に対して
              pivot_end = A の列数
          として使える．

        : diagonalize == false の場合，
          ピボットより下の行だけを消去する．
          すなわち，行列 a は階段形に近い形になる．

        : diagonalize == true の場合，
          ピボット行をピボットが 1 になるように正規化し，
          ピボット列の他の行もすべて 0 にする．
          すなわち，行列 a は簡約行階段形に近い形になる．

        : det は，正方行列に対して通常の行列式を表す．
          具体的には，
              H == W かつ pivot_end == W
          のように，正方行列全体に対して消去した場合に det(a) が得られる．
          途中でピボットが見つからなければ det = 0 になる．

          ただし，非正方行列や拡大係数行列に対して呼んだ場合，
          det は通常の意味での行列式ではないので，基本的には rank だけを見る．

[計算時間]
    - O(H W pivot_end)
      ただし H = a.size(), W = a[0].size()．

    - pivot_end == W のとき O(H W^2) 程度．
      H = W = N の正方行列なら O(N^3)．

[備考]
    - a は破壊的に変形される．
      元の行列が必要な場合はコピーを渡す．

    - T(0), T(1), +, -, *, /, != が使える型である必要がある．

    - modint 上で連立一次方程式を解く場合や，
      行列の rank / determinant を求める場合に使える．

    - double などの浮動小数点数で使う場合，
      a[i][j] != T(0) という厳密比較は数値誤差に弱い．
      必要なら eps を用いた判定に変更する．

    - pivot_end を W - 1 にすれば，
      最後の列を右辺とする拡大係数行列に対して掃き出しができる．

[使用例]
    vector<vector<mint>> A(n, vector<mint>(n));
    auto [rank, det] = GaussElimination(A);

    // rank : A の階数
    // det  : A の行列式
*/

template <typename T>
std::pair<int, T> GaussElimination(vector<vector<T>> &a, int pivot_end = -1,
                                   bool diagonalize = false) {
  if (a.empty()) return {0, 1};
  int H = a.size(), W = a[0].size(), rank = 0;
  if (pivot_end == -1) pivot_end = W;
  T det = 1;
  for (int j = 0; j < pivot_end; j++) {
    int idx = -1;
    for (int i = rank; i < H; i++) {
      if (a[i][j] != T(0)) {
        idx = i;
        break;
      }
    }
    if (idx == -1) {
      det = 0;
      continue;
    }
    if (rank != idx) det = -det, swap(a[rank], a[idx]);
    det *= a[rank][j];
    if (diagonalize && a[rank][j] != T(1)) {
      T coeff = T(1) / a[rank][j];
      for (int k = j; k < W; k++) a[rank][k] *= coeff;
    }
    int is = diagonalize ? 0 : rank + 1;
    for (int i = is; i < H; i++) {
      if (i == rank) continue;
      if (a[i][j] != T(0)) {
        T coeff = a[i][j] / a[rank][j];
        for (int k = j; k < W; k++) a[i][k] -= a[rank][k] * coeff;
      }
    }
    rank++;
  }
  return make_pair(rank, det);
}
