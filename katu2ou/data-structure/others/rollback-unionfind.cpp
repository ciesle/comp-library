//https://nyaannyaan.github.io/library/data-structure/rollback-union-find.hpp.html
/*
 - RollbackつきUnion Find
    - 経路圧縮を行わないことで，mergeの際の変更箇所が高々2箇所になるので，マージ前の配列を保存しておくことで
    復元できる mergeやfindがO(log n)
        - RollbackUnionFind
        - unite
        - find
        - same
        - undo
        - snapshot
        - rollback(int state s) : s=-1ならsnapshotが取られた時に戻り，s>=0ならuniteがs回呼ばれた
            直後に戻る
*/

struct RollbackUnionFind {
    vector<int> data;
    stack<pair<int, int> > history;
    int inner_snap;
  
    RollbackUnionFind(int sz) : inner_snap(0) { data.assign(sz, -1); }
  
    bool unite(int x, int y) {
      x = find(x), y = find(y);
      history.emplace(x, data[x]);
      history.emplace(y, data[y]);
      if (x == y) return false;
      if (data[x] > data[y]) swap(x, y);
      data[x] += data[y];
      data[y] = x;
      return true;
    }
  
    int find(int k) {
      if (data[k] < 0) return k;
      return find(data[k]);
    }
  
    int same(int x, int y) { return find(x) == find(y); }
  
    int size(int k) { return (-data[find(k)]); }
  
    void undo() {
      data[history.top().first] = history.top().second;
      history.pop();
      data[history.top().first] = history.top().second;
      history.pop();
    }
  
    void snapshot() { inner_snap = int(history.size() >> 1); }
  
    int get_state() { return int(history.size() >> 1); }
  
    void rollback(int state = -1) {
      if (state == -1) state = inner_snap;
      state <<= 1;
      assert(state <= (int)history.size());
      while (state < (int)history.size()) undo();
    }
};


// モノイドを持てるもの(chatgpt)
template<typename Monoid, Monoid (*Op)(Monoid, Monoid), Monoid (*E)()>
struct RollbackUnionFind {
    vector<int> parent;            // parent[k] < 0 のとき k は根で，その絶対値が集合のサイズ
    vector<Monoid> value;          // 各集合のモノイド値（根にのみ意味がある）
    // 履歴スタックには (ノード番号, 変更前の parent 値, 変更前の value) を記録
    stack<tuple<int, int, Monoid>> history;
    int inner_snap;                // snapshot 時の履歴のサイズ（ペア数）

    // コンストラクタ: 各ノードの初期モノイド値は init から与える
    RollbackUnionFind(int sz, const vector<Monoid>& init) : inner_snap(0) {
        parent.assign(sz, -1);
        value = init;
    }

    // 経路圧縮は行わず，rollback に対応
    int find(int k) {
        if (parent[k] < 0) return k;
        return find(parent[k]);
    }

    // 2 つの集合を併合する
    bool unite(int x, int y) {
        x = find(x), y = find(y);
        history.emplace(x, parent[x], value[x]);
        history.emplace(y, parent[y], value[y]);
        if (x == y) return false;
        if (parent[x] > parent[y]) swap(x, y);
        parent[x] += parent[y];
        parent[y] = x;
        // 併合時に，2 つのモノイドの値を結合関数 Op で合成
        value[x] = Op(value[x], value[y]);
        return true;
    }

    bool same(int x, int y) { return find(x) == find(y); }
    int size(int k) { return -parent[find(k)]; }
    // ノード k が属する集合全体のモノイド値を返す
    Monoid get_value(int k) { return value[find(k)]; }

    // 最新の union 操作で行った変更（2 回分）を undo
    void undo() {
        auto [y, p_y, v_y] = history.top(); history.pop();
        auto [x, p_x, v_x] = history.top(); history.pop();
        parent[y] = p_y; value[y] = v_y;
        parent[x] = p_x; value[x] = v_x;
    }

    // 現在の状態をスナップショットとして記録
    void snapshot() { inner_snap = history.size() / 2; }
    int get_state() { return history.size() / 2; }

    // rollback: 指定した状態まで変更を取り消す（state を省略した場合は snapshot まで戻す）
    void rollback(int state = -1) {
        if(state == -1) state = inner_snap;
        while(history.size() > (size_t)(state * 2)) {
            undo();
        }
    }
};