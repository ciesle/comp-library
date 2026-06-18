// 線形方程式 Ax = B を解く
// 解が存在する場合は, 解が v + C_1 w_1 + ... + C_k w_k (C_1,...,C_kは任意定数)と表せるとして
// (v, w_1, ..., w_k) を返す
// 解が存在しない場合は空のベクトルを返す

// gausselimination を使う
// double や Rational でも動くはず？(未検証)

// F_2に適用したい場合は，modint.set(2)として使える(遅くはなる) bitsetをうまく使ってできる?

template <typename T>
std::pair<int, T> GaussElimination(vector<vector<T>> &a, int pivot_end = -1,
                                   bool diagonalize = false) {
  if (a.empty()) return {0, 1};
  int H = a.size(), W = a[0].size(), rank = 0;
  assert((int)b.size() == H);
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

template <typename T>
vector<vector<T>> LinearEquation(vector<vector<T>> a, vector<T> b) {
  int H = a.size(), W = a[0].size();
  for (int i = 0; i < H; i++) a[i].push_back(b[i]);
  auto p = GaussElimination(a, W, true);
  int rank = p.first;
  for (int i = rank; i < H; ++i) {
    if (a[i][W] != 0) return vector<vector<T>>{};
  }
  vector<vector<T>> res(1, vector<T>(W));
  vector<int> pivot(W, -1);
  for (int i = 0, j = 0; i < rank; ++i) {
    while (a[i][j] == 0) ++j;
    res[0][j] = a[i][W], pivot[j] = i;
  }
  for (int j = 0; j < W; ++j) {
    if (pivot[j] == -1) {
      vector<T> x(W);
      x[j] = 1;
      for (int k = 0; k < j; ++k) {
        if (pivot[k] != -1) x[k] = -a[pivot[k]][j];
      }
      res.push_back(x);
    }
  }
  return res;
}