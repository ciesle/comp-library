//Randomized Binary Search Tree
//平衡二分探索木の一種
/*
値の挿入・値の削除・
*/
//出典: https://qiita.com/drken/items/1b7e6e459c24a83bb7fd

/*
<Randomized Binary Search Tree / RBST>
    平衡二分探索木の一種．
    merge/split を基本操作として，乱択により木の高さを期待 O(log N) に保つ．

    この実装では，値の昇順に要素を管理する multiset 的な用途を想定している．
    重複要素も保持できる．

[実装/関数]
    - RBST<VAL>()
        : 空の RBST を作る．

    - int size()
        : 現在の要素数を返す．

    - VAL sum()
        : 木全体に含まれる値の総和を返す．
          VAL には + が定義されている必要がある．

    - int lowerBound(VAL val)
        : val 以上である最初の要素の位置を返す．
          すなわち，val 未満の要素数を返す．

    - int upperBound(VAL val)
        : val より大きい最初の要素の位置を返す．
          すなわち，val 以下の要素数を返す．

    - int count(VAL val)
        : val と等しい要素の個数を返す．

    - VAL get(int k)
        : 0-indexed で k 番目に小さい値を返す．
          k が範囲外の場合の挙動には注意する．

    - pair<NODE*, NODE*> split(NODE* node, int k)
        : node を根とする木を，
              [0, k), [k, n)
          の2つに分割する．
          返り値は {左側の根, 右側の根}．

    - RBST split(int k)
        : 現在の木を [0, k), [k, n) に分割する．
          自身には [0, k) 側が残り，返り値として [k, n) 側の RBST を返す．

    - NODE* merge(NODE* left, NODE* right)
        : 2つの木 left, right を併合する．
          二分探索木として使う場合，
              left の全要素 <= right の全要素
          が成り立っている必要がある．

    - void merge(RBST add)
        : 現在の木と add を併合する．
          同様に，現在の木の全要素 <= add の全要素である必要がある．

    - void insert(VAL val)
        : val を1つ挿入する．
          lowerBound(val) の位置で split し，その間に新しいノードを挿入する．
          重複要素も挿入できる．

    - void erase(VAL val)
        : val を1つ削除する．
          val が存在しない場合は何もしない．
          同じ値が複数ある場合，そのうち1つだけを削除する．

    - void print()
        : 現在の要素を昇順に出力する．
          デバッグ用．

[計算量]
    - insert(val)
        : 期待 O(log N)

    - erase(val)
        : 期待 O(log N)

    - lowerBound(val), upperBound(val), count(val)
        : 期待 O(log N)

    - get(k)
        : 期待 O(log N)

    - split(k), merge(...)
        : 期待 O(log N)

    - size(), sum()
        : O(1)

[備考]
    - 各ノードは
          val  : そのノードの値
          size : 部分木サイズ
          sum  : 部分木の値の総和
      を持つ．

    - update(node) により，子の情報から size, sum を再計算する．

    - push(node) は遅延伝播用の関数である．
      現在は空実装だが，区間反転などを入れたい場合は NODE::push() に処理を書く．

    - VAL には少なくとも比較演算子 <, <=, >= と，加算 + が必要である．
      sum を使わない場合でも，この実装では update 内で + を使っているため注意する．

    - get(k) の範囲外アクセス時に -1 を返しているため，
      VAL が int 以外の場合はそのままだと不自然な場合がある．
      ライブラリ化するなら assert(0 <= k && k < size()) などに変えるとよい．

    - merge(left, right) は単に2つの木を結合する操作であり，
      一般の2つの二分探索木を自動で順序付きにマージするわけではない．
      必ず left 側の全要素が right 側の全要素以下である状況で使う．

    - new NODE(val) でノードを確保しているが，delete はしていない．
      競技プログラミング用途では問題になりにくいが，
      長時間動かすプログラムではメモリ管理に注意する．

    
    [使用例]
    RBST<long long> st;
    st.insert(5);
    st.insert(2);
    st.insert(5);

    st.count(5);       // 2
    st.get(0);         // 2
    st.get(1);         // 5
    st.lowerBound(5);  // 1
    st.upperBound(5);  // 3

    st.erase(5);       // 5 を1つ削除する

*/

template<class VAL> struct RBST {
    VAL SUM_UNITY = 0;                              // to be set

    unsigned int randInt() {
        static unsigned int tx = 123456789, ty = 362436069, tz = 521288629, tw = 88675123;
        unsigned int tt = (tx ^ (tx << 11));
        tx = ty; ty = tz; tz = tw;
        return (tw = (tw ^ (tw >> 19)) ^ (tt ^ (tt >> 8)));
    }

    struct NODE {
        NODE *left, *right;
        VAL val;                        // the value of the node
        int size;                       // the size of the subtree 
        VAL sum;                        // the value-sum of the subtree

        NODE() : val(SUM_UNITY), size(1), sum(SUM_UNITY) {
            left = right = NULL;
        }

        NODE(VAL v) : val(v), size(1), sum(v) {
            left = right = NULL;
        }

        /* additional update */
        inline void update() {

        }

        /* additional lazy-propagation */
        inline void push() {

            /* ex: reverse */
            /*
            if (this->rev) {
            swap(this->left, this->right);
            if (this->left) this->left->rev ^= true;
            if (this->right) this->right->rev ^= true;
            this->rev = false;
            }
            */
        }
    };


    ///////////////////////
    // root
    ///////////////////////

    NODE* root;
    RBST() : root(NULL) { }
    RBST(NODE* node) : root(node) { }


    ///////////////////////
    // basic operations
    ///////////////////////

    /* size */
    inline int size(NODE *node) {
        return !node ? 0 : node->size;
    }
    inline int size() {
        return this->size(this->root);
    }

    /* sum */
    inline VAL sum(NODE *node) {
        return !node ? SUM_UNITY : node->sum;
    }
    inline VAL sum() {
        return this->sum(this->root);
    }

    /* update, push */
    inline NODE* update(NODE *node) {
        node->size = size(node->left) + size(node->right) + 1;
        node->sum = sum(node->left) + sum(node->right) + node->val;
        node->update();
        return node;
    }

    inline void push(NODE *node) {
        if (!node) return;
        node->push();
    }

    /* lower_bound */
    inline int lowerBound(NODE *node, VAL val) {
        push(node);
        if (!node) return 0;
        if (val <= node->val) return lowerBound(node->left, val);
        else return size(node->left) + lowerBound(node->right, val) + 1;
    }
    inline int lowerBound(VAL val) {
        return this->lowerBound(this->root, val);
    }

    /* upper_bound */
    inline int upperBound(NODE *node, VAL val) {
        push(node);
        if (!node) return 0;
        if (val >= node->val) return size(node->left) + upperBound(node->right, val) + 1;
        else return upperBound(node->left, val);
    }
    inline int upperBound(VAL val) {
        return this->upperBound(this->root, val);
    }

    /* count */
    inline int count(VAL val) {
        return upperBound(val) - lowerBound(val);
    }

    /* get --- k: 0-index */
    inline VAL get(NODE *node, int k) {
        push(node);
        if (!node) return -1;
        if (k == size(node->left)) return node->val;
        if (k < size(node->left)) return get(node->left, k);
        else return get(node->right, k - size(node->left) - 1);
    }
    inline VAL get(int k) {
        return get(this->root, k);
    }


    ///////////////////////
    // merge-split
    ///////////////////////

    NODE* merge(NODE *left, NODE *right) {
        push(left);
        push(right);
        if (!left || !right) {
            if (left) return left;
            else return right;
        }
        if (randInt() % (left->size + right->size) < left->size) {
            left->right = merge(left->right, right);
            return update(left);
        }
        else {
            right->left = merge(left, right->left);
            return update(right);
        }
    }
    void merge(RBST add) {
        this->root = this->merge(this->root, add.root);
    }
    pair<NODE*, NODE*> split(NODE* node, int k) { // [0, k), [k, n)
        push(node);
        if (!node) return make_pair(node, node);
        if (k <= size(node->left)) {
            pair<NODE*, NODE*> sub = split(node->left, k);
            node->left = sub.second;
            return make_pair(sub.first, update(node));
        }
        else {
            pair<NODE*, NODE*> sub = split(node->right, k - size(node->left) - 1);
            node->right = sub.first;
            return make_pair(update(node), sub.second);
        }
    }
    RBST split(int k) {
        pair<NODE*, NODE*> sub = split(this->root, k);
        this->root = sub.first;
        return RBST(sub.second);
    }


    ///////////////////////
    // insert-erase
    ///////////////////////

    void insert(const VAL val) {
        pair<NODE*, NODE*> sub = this->split(this->root, this->lowerBound(val));
        this->root = this->merge(this->merge(sub.first, new NODE(val)), sub.second);
    }

    void erase(const VAL val) {
        if (!this->count(val)) return;
        pair<NODE*, NODE*> sub = this->split(this->root, this->lowerBound(val));
        this->root = this->merge(sub.first, this->split(sub.second, 1).second);
    }


    ///////////////////////
    // debug
    ///////////////////////

    void print(NODE *node) {
        if (!node) return;
        push(node);
        print(node->left);
        cout << node->val << " ";
        print(node->right);
    }
    void print() {
        cout << "{";
        print(this->root);
        cout << "}" << endl;
    }
};

