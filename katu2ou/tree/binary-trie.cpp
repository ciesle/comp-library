/*
<binary trie> (https://kazuma8128.hatenablog.com/entry/2018/05/06/022654)
    - 整数をbit列と見做してtrie木の要領でsetみたいなことができるデータ構造

    [できること] ビット長(固定)をBとすると，次の操作がO(B)でできる
        - binary_trie : コンストラクタ
        - void insert(U x) : 値xを集合から一つ挿入
        - void erase(U x) : 値xを集合から消去
        - U max_element/min_element() : 集合内の最大/最小値を取得
        - U max_element/min_element(U x) : 集合内の値の中で，値xとXORしたときに最大・最小となる値の取得
                                        (XORした値ではなく，集合の元を返す)
        - int lower_bound/upper_bound(U x) : 集合内でx以上(より大きい)の最小の要素の番号(小さい方から何番目か)を
                                        0-indexedで取得(xが何番目に小さいか得られる)
        - bt[k] : k番目に小さい要素(0-indexed)が何個あるかを返す
        - int count(U x, int D) : 深さDのノードにおいて，値xが何個入っているかを返す   
            (1<=D<=B)

    (作る予定)
        - dfs() : 要素をdfs的に条件付き探索
        - xor_all(U x) : 全要素を値xでxorした値に変更 (遅延評価をするとO(1))
        - kth_element(U x, int k) : 全要素を値xでxorした際に小さい方からk(0-indexed)番目の要素の値を，
                                    xorする前の値として取得

        木構造なので永続化もできるらしい

    [構造]
        - 木の根元にダミーノードが入っており，その後ろが二分の構造になっている
        - B : 格納する値のビット長 (32で固定)
        - value : 1->1->0としたら110が入っている 根のvalueは-1
        - depth : 1->1としたら深さは2
        - 
*/

template<typename U = unsigned, int B = 32>
class binary_trie {
    struct node {
        int cnt;
        node *ch[2];
        node() : cnt(0), ch{ nullptr, nullptr } {}
    };
    node* add(node* t, U val, int b = B - 1) {
        if (!t) t = new node;
        t->cnt += 1;
        if (b < 0) return t;
        bool f = (val >> (U)b) & (U)1;
        t->ch[f] = add(t->ch[f], val, b - 1);
        return t;
    }
    node* sub(node* t, U val, int b = B - 1) {
        assert(t);
        t->cnt -= 1;
        if (t->cnt == 0) return nullptr;
        if (b < 0) return t;
        bool f = (val >> (U)b) & (U)1;
        t->ch[f] = sub(t->ch[f], val, b - 1);
        return t;
    }
    U get_min(node* t, U val, int b = B - 1) const {
        assert(t);
        if (b < 0) return 0;
        bool f = (val >> (U)b) & (U)1; f ^= !t->ch[f];
        return get_min(t->ch[f], val, b - 1) | ((U)f << (U)b);
    }
    U get(node* t, int k, int b = B - 1) const {
        if (b < 0) return 0;
        int m = t->ch[0] ? t->ch[0]->cnt : 0;
        return k < m ? get(t->ch[0], k, b - 1) : get(t->ch[1], k - m, b - 1) | ((U)1 << (U)b);
    }
    int count_lower(node* t, U val, int b = B - 1) {
        if (!t || b < 0) return 0;
        bool f = (val >> (U)b) & (U)1;
        return (f && t->ch[0] ? t->ch[0]->cnt : 0) + count_lower(t->ch[f], val, b - 1);
    }
    node *root;
public:
    binary_trie() : root(nullptr) {}
    int size() const {
        return root ? root->cnt : 0;
    }
    bool empty() const {
        return !root;
    }
    void insert(U val) {
        root = add(root, val);
    }
    void erase(U val) {
        root = sub(root, val);
    }
    U max_element(U bias = 0) const {
        return get_min(root, ~bias);
    }
    U min_element(U bias = 0) const {
        return get_min(root, bias);
    }
    int lower_bound(U val) { // return id
        return count_lower(root, val);
    }
    int upper_bound(U val) { // return id
        return count_lower(root, val + 1);
    }
    U operator[](int k) const {
        assert(0 <= k && k < size());
        return get(root, k);
    }
    int count(U val, int D) const {
        if (!root) return 0;
        node *t = root;
        for (int i = D - 1; i >= 0; i--) {
            t = t->ch[(val >> (U)i) & (U)1];
            if (!t) return 0;
        }
        return t->cnt;
    }
};