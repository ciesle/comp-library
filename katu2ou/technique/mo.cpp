//Mo's algorithm
/*
長さNの配列に対して、Q個の「1次元区間」クエリを合計でO((N+Q)sqrt(N))で処理する
<条件>
・処理する全クエリを先読みできる(オフラインクエリ)．
・配列の各要素が不変．
・区間[l,r)の結果から区間[l+1,r)，[l−1,r)，[l,r−1)，[l,r+1)の結果をある程度高速に計算できる．(最終的な計算量に影響する．)
・区間[l,m)と区間[m,r)の結果をマージして[l,r)の結果を高速に計算することができない．(これができるならセグメント木などを使えば良い．)

つまり、l,rを1変えるだけなら高速にできるが、マージにO(N)かかってしまうようなものに対して
うまく作用する。
*/

// 以下のstructの部分はadd,erase,outのみ変更する
/*
<使用法>
- 最初のクエリの区間[l,r)から順にadd(l, r) を呼び出す.
- そのあと build(add, erase, out) を呼び出すと, すべてのクエリを処理する. 
 問題に応じて実装すべきなのは add(i), erase(i), out(q) で, それぞれ 
 i番目の要素が加わる時,
 i番目の要素が消える時, 
 q番目のクエリを処理するとき
 に呼び出される.
- 要素の追加や削除が非可換な場合があるが, その時は 
 build(add_left, add_right, erase_left, erase_right, out) を呼び出すこと.

*/
struct Mo {
  int n;
  vector< pair< int, int > > lr;

  explicit Mo(int n) : n(n) {}

  void add(int l, int r) { /* [l, r) */
    lr.emplace_back(l, r);
  }

  template< typename AL, typename AR, typename EL, typename ER, typename O >
  void build(const AL &add_left, const AR &add_right, const EL &erase_left, const ER &erase_right, const O &out) {
    int q = (int) lr.size();
    int bs = n / min< int >(n, sqrt(q));
    vector< int > ord(q);
    iota(begin(ord), end(ord), 0);
    sort(begin(ord), end(ord), [&](int a, int b) {
      int ablock = lr[a].first / bs, bblock = lr[b].first / bs;
      if(ablock != bblock) return ablock < bblock;
      return (ablock & 1) ? lr[a].second > lr[b].second : lr[a].second < lr[b].second;
    });
    int l = 0, r = 0;
    for(auto idx : ord) {
      while(l > lr[idx].first) add_left(--l);
      while(r < lr[idx].second) add_right(r++);
      while(l < lr[idx].first) erase_left(l++);
      while(r > lr[idx].second) erase_right(--r);
      out(idx);
    }
  }

  template< typename A, typename E, typename O >
  void build(const A &add, const E &erase, const O &out) {
    build(add, add, erase, erase, out);
  }
};

// ex) 長さNの数列{ai}(ai<=10^6)が与えられた時、[lj,rj]に含まれる値の種類数を答える(1<=j<=Q)
// int main() {
//   int N;
//   cin >> N;
//   vector< int > A(N);
//   for(auto &a: A) cin >> a;
//   int Q;
//   cin >> Q;
//   Mo mo(N); //呼び出し
//   for(int i = 0; i < Q; i++) {
//     int a, b;
//     cin >> a >> b;
//     mo.add(a - 1, b); //クエリに対応する区間[k,l)(半閉区間!)をaddする
//   }
//   vector< int > cnt(1000001), ans(Q); //操作に関与する変数はmainで定義 moにも渡さなくて良い
//   int sum = 0;

//   auto add = [&](int i) { // i番目の要素が加わる時に、どのような操作をするか(cnt,sumは現在の状態として持っておく)
//     if(cnt[A[i]]++ == 0) ++sum;
//   };
//   auto erase = [&](int i) { //i番目の要素が外される時に...
//     if(--cnt[A[i]] == 0) --sum;
//   };
//   auto out = [&](int q) { //これがq番目のクエリの答えになる(ansとして出力)
//     ans[q] = sum;
//   };

//   mo.build(add, erase, out); // buildで解く
//   for(auto &p: ans) cout << p << "\n";
// }