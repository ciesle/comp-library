//(https://drken1215.hatenablog.com/entry/2023/10/19/025800)
/*
Cartesian Tree
    - 数列から誘導されるCartesian Treeとは，次のような二分木
        - 根は数列の最小値に対応する
        - 値の左側頂点は，数列全体の最小要素より左側の区間における最小の要素
        - 値の右側頂点は，数列全体の最小要素より右側の区間における最小の要素
        - 以下，再帰的に構成される頂点数nの二分木

    - stackを次のような状態に保つことでO(n)で構成できる
        - stackの先頭要素v_1は，数列の先頭から今まで見てきた要素の中で最小のもの
        - 2番目の要素v_2は，数列のv_1よりも後ろの要素から今まで見てきた要素の中で最小のもの
        - 3番目の要素v_3は，数列のv_2よりも後ろの...
        (新しい値を見る時，その値より大きいstackの値を後ろから順にpopする．1個以上pop
        した場合は，新しい値をstackに加える)
    
        - stackの末尾の要素sの後ろに値vをpushするとき(s < v)
            - vの親をsにする
            - sの右の子をvにする(sが既に右の子を持つときは入れ替える)
        - stackの末尾の要素に対して，値vとの比較によりpopするとき (*>v)
            - その「最後に」popした要素をsとする
            - vの左の子をsとする
            - sの親をvにする(sが既に親を持つときは入れ替える)

- 応用
    - 数列のRMQ(l,r) : Cartesian Tree上でLCA(l,r)を解くことで最小値indexが求まる
    - LCA(u,v) : ETでu,vが現れる範囲において深さ配列d上でRMQを考えることで
        対応する頂点がLCAとして求まる

    <概要>
        - Cartesian木を作る．頂点には最小値が，その左右の子は分割された区間のうち最小値がある．
        (最大値を根側に持っていきたいなら，配列を-1倍して木を作った後，また-1倍して元配列を元に戻せば良い)

    <使用法>
        - CartesianTree<ll> car(vector<T> v) : vに対応するCartesian Tree を作成
        - car.par[i] : 頂点iの親のindex(根の場合は-1)
        - car.left[i]
        - car.right[i]

    - 参照 :
    - verified at : 

*/

template<class T> struct CartesianTree {
    int root;  // root
    vector<int> par, left, right;

    CartesianTree() {}
    CartesianTree(const vector<T>& v) :
    par(v.size(), -1), left(v.size(), -1), right(v.size(), -1) {
        vector<int> st(v.size(), 0);
        int top = 0;
        for (int i = 1; i < v.size(); ++i) {
            if (v[st[top]] > v[i]) {
                while (top >= 1 && v[st[top - 1]] > v[i]) --top;
                left[i] = st[top];
                par[left[i]] = i;
                if (top == 0) {
                    root = i;
                } else {
                    par[i] = st[top - 1];
                    right[par[i]] = i;
                }
                st[top] = i;
            } else {
                par[i] = st[top];
                right[par[i]] = i;
                st[++top] = i;
            }
        }
    }
};
