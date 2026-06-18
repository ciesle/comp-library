/*
  <Rolling Hash>
    - 文字列sに対して，sの部分文字列s[a,b]に対するhashを計算

    [実装/関数]
        - RollingHash<Str> rh(Str S)
            : Sに対するRollingHashの構築 (Strはstringやvector<int>など)
        - void build(Str S) : (コンストラクタにおいてSを指定していなかった場合) Sに対して構築をする
        - Hash get(int l, int r) : 区間[l,r) のハッシュを返す
        - Hash get_hash(Str T) : 文字列Tのハッシュ値を返す
        - int find(Str T, int lower = 0) : Sのlower文字以降で初めてTが出てくる位置(Tの最初の文字のindex)を返す(存在しないなら-1を返す)
        - int LCP(int a, int b, int al, int bl)
            : aのal文字目から始まるsuffixとbのbl文字目から始まるsuffixのLCPを返す
        - int strcmp(int a, int b, int al, int bl, int ar = -1, int br = -1)
            : a[al,ar) と b[bl,br) の代償を比較する(返り値はstd::strcmpの仕様に準ずる。)
        - int size() : Sのサイズを返す

        - Hash : array<u64, BASE_NUM> である．BASE_NUMの個数だけhash値を保持している．

    [計算時間]
        - O(n+m)
    
    [備考]
        - BASE_NUM は同時に持つ独立ハッシュの個数(デフォルトは2)
    
    [参照]
        - https://bakamono1357.hatenablog.com/entry/2020/04/29/025320
        - https://algo-logic.info/bridge-lowlink/

    [verified at]
        - https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all
    
    [使用例]
        
        
*/


namespace internal1 {
using i64 = long long;
using u64 = unsigned long long;
using u128 = __uint128_t;

template <int BASE_NUM = 2>
struct Hash : array<u64, BASE_NUM> {
  using array<u64, BASE_NUM>::operator[];
  static constexpr int n = BASE_NUM;

  Hash() : array<u64, BASE_NUM>() {}

  static constexpr u64 md = (1ull << 61) - 1;

  constexpr static Hash set(const i64 &a) {
    Hash res;
    fill(begin(res), end(res), cast(a));
    return res;
  }
  Hash &operator+=(const Hash &r) {
    for (int i = 0; i < n; i++)
      if (((*this)[i] += r[i]) >= md) (*this)[i] -= md;
    return *this;
  }
  Hash &operator+=(const i64 &r) {
    u64 s = cast(r);
    for (int i = 0; i < n; i++)
      if (((*this)[i] += s) >= md) (*this)[i] -= md;
    return *this;
  }
  Hash &operator-=(const Hash &r) {
    for (int i = 0; i < n; i++)
      if (((*this)[i] += md - r[i]) >= md) (*this)[i] -= md;
    return *this;
  }
  Hash &operator-=(const i64 &r) {
    u64 s = cast(r);
    for (int i = 0; i < n; i++)
      if (((*this)[i] += md - s) >= md) (*this)[i] -= md;
    return *this;
  }
  Hash &operator*=(const Hash &r) {
    for (int i = 0; i < n; i++) (*this)[i] = modmul((*this)[i], r[i]);
    return *this;
  }
  Hash &operator*=(const i64 &r) {
    u64 s = cast(r);
    for (int i = 0; i < n; i++) (*this)[i] = modmul((*this)[i], s);
    return *this;
  }

  Hash operator+(const Hash &r) { return Hash(*this) += r; }
  Hash operator+(const i64 &r) { return Hash(*this) += r; }
  Hash operator-(const Hash &r) { return Hash(*this) -= r; }
  Hash operator-(const i64 &r) { return Hash(*this) -= r; }
  Hash operator*(const Hash &r) { return Hash(*this) *= r; }
  Hash operator*(const i64 &r) { return Hash(*this) *= r; }
  Hash operator-() const {
    Hash res;
    for (int i = 0; i < n; i++) res[i] = (*this)[i] == 0 ? 0 : md - (*this)[i];
    return res;
  }
  friend Hash pfma(const Hash &a, const Hash &b, const Hash &c) {
    Hash res;
    for (int i = 0; i < n; i++) res[i] = modfma(a[i], b[i], c[i]);
    return res;
  }
  friend Hash pfma(const Hash &a, const Hash &b, const i64 &c) {
    Hash res;
    u64 s = cast(c);
    for (int i = 0; i < n; i++) res[i] = modfma(a[i], b[i], s);
    return res;
  }

  Hash pow(long long e) {
    Hash a{*this}, res{Hash::set(1)};
    for (; e; a *= a, e >>= 1) {
      if (e & 1) res *= a;
    }
    return res;
  }

  static Hash get_basis() {
    static auto rand_time =
        chrono::duration_cast<chrono::nanoseconds>(
            chrono::high_resolution_clock::now().time_since_epoch())
            .count();
    static mt19937_64 rng(rand_time);
    Hash h;
    for (int i = 0; i < n; i++) {
      while (isPrimitive(h[i] = rng() % (md - 1) + 1) == false)
        ;
    }
    return h;
  }

 private:
  static u64 modpow(u64 a, u64 b) {
    u64 r = 1;
    for (a %= md; b; a = modmul(a, a), b >>= 1) r = modmul(r, a);
    return r;
  }
  static bool isPrimitive(u64 x) {
    for (auto &d : vector<u64>{2, 3, 5, 7, 11, 13, 31, 41, 61, 151, 331, 1321})
      if (modpow(x, (md - 1) / d) <= 1) return false;
    return true;
  }
  static inline constexpr u64 cast(const long long &a) {
    return a < 0 ? a + md : a;
  }
  static inline constexpr u64 modmul(const u64 &a, const u64 &b) { 
    u128 d = u128(a) * b;
    u64 ret = (u64(d) & md) + u64(d >> 61);
    return ret >= md ? ret - md : ret;
  }
  static inline constexpr u64 modfma(const u64 &a, const u64 &b, const u64 &c) {
    u128 d = u128(a) * b + c;
    u64 ret = (d >> 61) + (u64(d) & md);
    return ret >= md ? ret - md : ret;
  }
};

}  // namespace internal


template <typename Str, int BASE_NUM = 2>
struct RollingHash {
  using Hash = internal1::Hash<BASE_NUM>;
  Str data;
  vector<Hash> hs, pw;
  int s;
  inline static Hash basis = Hash::get_basis(); //inlineが必要！！

  RollingHash(const Str &S = Str()) { build(S); }

  void build(const Str &S) {
    data = S;
    s = S.size();
    hs.resize(s + 1);
    pw.resize(s + 1);
    pw[0] = Hash::set(1);
    hs[0] = Hash::set(0);
    for (int i = 1; i <= s; i++) {
      pw[i] = pw[i - 1] * basis;
      hs[i] = pfma(hs[i - 1], basis, S[i - 1]);
    }
  }

  Hash get(int l, int r = -1) const {
    if (r == -1) r = s;
    return pfma(hs[l], -pw[r - l], hs[r]);
  }

  // T の hash を返す
  static Hash get_hash(const Str &T) {
    Hash ret = Hash::set(0);
    for (int i = 0; i < (int)T.size(); i++) ret = pfma(ret, basis, T[i]);
    return ret;
  }

  // a + b の hash を返す
  // 引数 : a, b, b の長さ
  static Hash unite(Hash a, Hash b, long long bsize) {
    return pfma(a, basis.pow(bsize), b);
  }

  int find(Str &T, int lower = 0) const {
    auto ths = get_hash(T);
    for (int i = lower; i <= s - (int)T.size(); i++)
      if (ths == get(i, i + (int)T.size())) return i;
    return -1;
  }

  static int lcp(const RollingHash &a, const RollingHash &b, int al, int bl) {
    int ok = 0, ng = min(a.size() - al, b.size() - bl) + 1;
    while (ok + 1 < ng) {
      int med = (ok + ng) / 2;
      (a.get(al, med + al) == b.get(bl, med + bl) ? ok : ng) = med;
    }
    return ok;
  }

  static int strcmp(const RollingHash &a, const RollingHash &b, int al, int bl,
                    int ar = -1, int br = -1) {
    if (ar == -1) ar = a.size();
    if (br == -1) br = b.size();
    int n = min<int>({lcp(a, b, al, bl), ar - al, br - bl});
    return al + n == ar                      ? bl + n == br ? 0 : -1
           : bl + n == br                    ? 1
           : a.data[al + n] < b.data[bl + n] ? -1
                                             : 1;
  }

  int size() const { return s; }
};

template <typename Str, int BASE_NUM>
typename RollingHash<Str, BASE_NUM>::Hash RollingHash<Str, BASE_NUM>::basis =
    internal1::Hash<BASE_NUM>::get_basis();
using roriha = RollingHash<string, 2>;


