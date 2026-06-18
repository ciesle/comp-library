//suffix array
/*
接尾辞配列のこと(←→ prefix array) n文字の文字列Sに対して、Sの接尾辞s_i (=[i,n), 0<=i<n)
を辞書順に並び替えた時、それぞれの接尾辞が何番目にあるかを記録したもの。

時間計算量O(nlogn) 空間計算量O(n)

<方針>
1. S(の末尾に$をつけたもの)の長さ2^iの連続部分文字列(ただしSの末尾-先頭が繋がっているものとみなす = 巡回部分文字列)の
辞書順ソートをi=0,1,...に対して行う
2. ここで、i=k+1の列を構成するには、i=kの列の結果から(s_j,s_j+2^k)という組をつくり、そのソートを
考えると良い
3. 最後に、Sの末尾に付けた$を除いておく (ここで$は任意のアルファベットより辞書順で小さい)
*/

vector<int> sort_cyclic_shifts(string const& s) {
    int n = s.size();
    const int alphabet = 256;
    vector<int> p(n), c(n), cnt(max(alphabet, n), 0);

    //k=0のソート

    for (int i = 0; i < n; i++){
        cnt[s[i]]++;
    }
    for (int i = 1; i < alphabet; i++){
        cnt[i] += cnt[i-1];
    }
    for (int i = 0; i < n; i++){
        p[--cnt[s[i]]] = i;
    }
    c[p[0]] = 0;
    int classes = 1;
    for (int i = 1; i < n; i++) {
        if (s[p[i]] != s[p[i-1]]){
            classes++;
        }
        c[p[i]] = classes - 1;
    }


    //以下k=1…logn+1の反復をする
    vector<int> pn(n), cn(n);
    for (int k = 0; (1 << k) < n; ++k) {
        for (int i = 0; i < n; i++) {
            pn[i] = p[i] - (1 << k);
            if (pn[i] < 0){
                pn[i] += n;
            }
        }
        fill(cnt.begin(), cnt.begin() + classes, 0);
        for (int i = 0; i < n; i++){
            cnt[c[pn[i]]]++;
        }
        for (int i = 1; i < classes; i++){
            cnt[i] += cnt[i-1];
        }
        for (int i = n-1; i >= 0; i--){
            p[--cnt[c[pn[i]]]] = pn[i];
        }
        cn[p[0]] = 0;
        classes = 1;
        for (int i = 1; i < n; i++) {
            pair<int, int> cur = {c[p[i]], c[(p[i] + (1 << k)) % n]};
            pair<int, int> prev = {c[p[i-1]], c[(p[i-1] + (1 << k)) % n]};
            if (cur != prev){
                ++classes;
            }
            cn[p[i]] = classes - 1;
        }
        c.swap(cn);
    }
    return p;
}

vector<int> suffix_array_construction(string s) {
    s += "$";
    vector<int> sorted_shifts = sort_cyclic_shifts(s);
    sorted_shifts.erase(sorted_shifts.begin());
    return sorted_shifts;
}
