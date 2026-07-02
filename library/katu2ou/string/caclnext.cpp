
//配列nex: nex[i][c]は、文字列Sのi文字目以降で、文字cが出現する最小の添字(存在しない場合は N)
//Sを受け取って vvi型のnexを返す関数
//英小文字用である。大文字なら直さないとSEとなる
vector<vector<int> > calcNext(const string &S) {
    int n = (int)S.size();
    vector<vector<int>> res(n+1, vector<int>(26, n));
    for (int i = n-1; i >= 0; --i) {
        for (int j = 0; j < 26; ++j) res[i][j] = res[i+1][j];
        res[i][S[i]-'a'] = i;
    }
    return res;
}