/*
  <Segment Set / 区間集合>
    - 半開区間 [l, r) を set で管理する
    - 区間を追加すると，既存の区間と重なる部分を自動でマージする
    - この実装では，隣接する区間もマージする

        例:
            [3,5), [4,7), [8,9)
            -> [3,7), [8,9)

            [3,4), [4,5)
            -> [3,5)

    [概要]
        - segset の各要素 pair<long long,long long> は区間 [first, second) を表す
        - 常に，segset 内の通常区間は互いに交わらず，かつ隣接もしないように保つ
        - 番兵として [-LARGE,-LARGE), [LARGE,LARGE) を入れておくことで，
          lower_bound 後の iterator 操作を簡単にしている

    [実装/関数]
        - segment_set st
            空の区間集合を作る

        - void add(long long l, long long r)
            区間 [l,r) を追加する
            既存区間と重なる，または隣接する場合はマージする
            l >= r の場合は空区間として何もしない

        - vector<pair<long long,long long>> intervals()
            現在保持している区間の一覧を昇順で返す
            番兵は含めない

    [計算時間]
        - add(l,r)
            O((k+1) log N)

            ここで，
                N = 現在保持している区間数
                k = add によって削除・マージされる区間数

        - intervals()
            O(N)

    [備考]
        - この実装では隣接区間もマージする
          つまり [a,b), [b,c) は [a,c) になる

        - 隣接区間をマージしたくない場合は，
          条件中の

              x <= it->second
              it->first <= y

          をそれぞれ

              x < it->second
              it->first < y

          に変更する

        - 座標は (-LARGE, LARGE) の範囲で使うことを想定する
        - 番兵を直接操作しないこと

    [verified at]
        - g++ 14.2.0, -std=gnu++23
        - 固定ケース，空区間，ランダムテスト 10000 ケースで確認

    [使用例]
        segment_set st;

        st.add(3, 5);
        st.add(4, 7);
        st.add(8, 9);

        // intervals: [3,7), [8,9)
        for (auto [l, r] : st.intervals()) {
            cout << "[" << l << "," << r << ") ";
        }
        cout << endl;
*/

struct segment_set {
    static constexpr long long LARGE = (1LL << 60);

    set<pair<long long, long long>> segset;

    segment_set() {
        segset.insert({-LARGE, -LARGE});
        segset.insert({ LARGE,  LARGE});
    }

    void add(long long x, long long y) {
        // [x,y) が空なら何もしない
        if (x >= y) return;

        auto it = segset.lower_bound({x, y});
        --it;

        // 左側の区間と重なる，または隣接するならマージする
        if (it->first <= x && x <= it->second) {
            x = min(x, it->first);
            y = max(y, it->second);
            it = segset.erase(it);
        }

        it = segset.lower_bound({x, y});

        // 右側にある区間と重なる，または隣接する限りマージする
        while (it != segset.end()) {
            if (x <= it->first && it->first <= y) {
                y = max(y, it->second);
                it = segset.erase(it);
            } else {
                break;
            }
        }

        segset.insert({x, y});
    }

    vector<pair<long long, long long>> intervals() const {
        vector<pair<long long, long long>> res;

        for (auto [l, r] : segset) {
            if (l == -LARGE && r == -LARGE) continue;
            if (l ==  LARGE && r ==  LARGE) continue;
            res.push_back({l, r});
        }

        return res;
    }
};