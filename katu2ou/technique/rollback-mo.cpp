/*
<Rollback Mo's Algorithm / Rollback-Mo>
    区間クエリ [l, r) をオフラインで処理するアルゴリズム．
    通常の Mo と異なり，削除操作 remove を使わず，
    add と rollback によって状態を戻す．

    削除が難しいが，変更履歴を用いて snapshot 時点まで巻き戻せる
    データ構造に対して有効．

[使う関数]
    - add(i)
        : 添字 i の要素を現在のデータ構造に追加する．
          変更が rollback できるように，必要なら変更前の値を履歴に保存する．

    - query(q)
        : q 番目のクエリに対する答えを，現在のデータ構造から計算して保存する．

    - reset()
        : データ構造を空の状態に戻す．
          変更履歴や snapshot 情報も初期化する．

    - snapshot()
        : 現在の状態を記録する．
          以降の rollback() はこの状態まで戻す．

    - rollback()
        : 直前の snapshot() の状態まで巻き戻す．

[通常の Mo との違い]
    - 通常の Mo は add/remove で左右端を動かす．
    - Rollback Mo は remove を使わず，add と rollback だけで処理する．

[計算量]
    - おおよそ O((N + Q) sqrt(N) * add の計算量)
*/

struct MoRollBack {
    using ADD = function<void(int)>;
    using QUERY = function<void(int)>;
    using RESET = function<void()>;
    using SNAPSHOT = function<void()>;
    using ROLLBACK = function<void()>;

    int N, Q;
    int width;
    vector<int> left, right, order;

    MoRollBack(int N_, int Q_)
        : N(N_), Q(Q_), width(max(1, (int)sqrt(N_))), order(Q_) {
        iota(order.begin(), order.end(), 0);
    }

    // 区間クエリ [l, r) を追加する
    void add_query(int l, int r) {
        left.emplace_back(l);
        right.emplace_back(r);
    }

    // 互換用: 以前の add(l,r) という名前でも使えるようにする
    void add(int l, int r) {
        add_query(l, r);
    }

    void run(const ADD& add,
             const QUERY& query,
             const RESET& reset,
             const SNAPSHOT& snapshot,
             const ROLLBACK& rollback) {
        assert((int)left.size() == Q);
        assert((int)right.size() == Q);

        sort(order.begin(), order.end(), [&](int a, int b) {
            int ablock = left[a] / width;
            int bblock = left[b] / width;
            if (ablock != bblock) return ablock < bblock;
            return right[a] < right[b];
        });

        // 短い区間は，空状態からそのまま追加して答える
        reset();

        for (int idx : order) {
            if (right[idx] - left[idx] < width) {
                snapshot();

                for (int i = left[idx]; i < right[idx]; i++) {
                    add(i);
                }

                query(idx);
                rollback();
            }
        }

        // 長い区間を左端のブロックごとに処理する
        int nr = 0;
        int last_block = -1;

        for (int idx : order) {
            if (right[idx] - left[idx] < width) continue;

            int block = left[idx] / width;
            int block_right = min(N, (block + 1) * width);

            if (last_block != block) {
                reset();
                last_block = block;
                nr = block_right;
            }

            // 右端は単調に伸ばす
            while (nr < right[idx]) {
                add(nr++);
            }

            // 現在の状態を保存してから，左側を一時的に追加する
            snapshot();

            for (int j = block_right - 1; j >= left[idx]; j--) {
                add(j);
            }

            query(idx);

            // 左側を一時的に追加する前の状態に戻す
            rollback();
        }
    }
};