//回文検出
/*
入力: 長さnの文字列s
出力: 長さ2n+1のvector<int> radで、各要素はsのi文字目(or文字と文字の間)を中心とする
     回文の長さの最大値 (L文字目からR文字目までの部分列が回文かどうかの判定は
     rad[(L+R)/2-1]>=R-L+1であるかどうかで判定できる)
*/

vector<int> manacher(string &u){
    int n = u.size();
    string s(2 * n + 1, '#');
    vector<int> rad(2 * n + 1);

    for (int i = 0; i < n; i++) s[2 * i + 1] = u[i];
    n = 2 * n + 1;

    vector<int> rad(n);
    int c = 0, r = 0;

    while (c < n) {
        // cを中心に同じ文字がどこまで連続するか
        while (0 <= c - r && c + r < n && s[c - r] == s[c + r]) r++;
        rad[c] = r;

        //回文の長さに応じて利用可能な範囲を確認しつつメモ
        int k = 1;
        while (0 <= c - k && k + rad[c - k] < r) {
        rad[c + k] = rad[c - k];
        k++;
        }
        //すでに計算が終わった分だけ中心と探索半径をずらす
        c += k;
        r -= k;
    }
    //'＃'分の補正
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0)
        rad[i] = (rad[i] - 1) / 2;
        else
        rad[i] /= 2;
    }
    
    return rad;
}

