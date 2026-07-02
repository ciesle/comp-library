/*
  <ゼータ変換・メビウス変換>

    - bitmask で表された集合族上の関数に対するゼータ変換・メビウス変換を行う
    - 長さ 2^n の配列 A を，集合 S ⊆ {0,1,...,n-1} 上の関数 A[S] とみなす
        - 整数 mask の i bit 目が 1 ⇔ 集合に i が含まれる

    [概要]
        - 下位集合ゼータ変換
            F[S] = Σ_{T ⊆ S} A[T]

        - 上位集合ゼータ変換
            F[S] = Σ_{S ⊆ T} A[T]

        - メビウス変換は，対応するゼータ変換の逆変換である

    [実装/関数]
        - void zt_des(vector<T>& A, int n)
            下位集合ゼータ変換を行う
            A[S] <- Σ_{T ⊆ S} A[T]

        - void mb_des(vector<T>& A, int n)
            下位集合メビウス変換を行う
            zt_des の逆変換
            zt_des 後の配列に適用すると元に戻る

        - void zt_asc(vector<T>& A, int n)
            上位集合ゼータ変換を行う
            A[S] <- Σ_{S ⊆ T} A[T]

        - void mb_asc(vector<T>& A, int n)
            上位集合メビウス変換を行う
            zt_asc の逆変換
            zt_asc 後の配列に適用すると元に戻る

    [計算時間]
        - 各変換 O(n 2^n)

    [要件]
        - A.size() >= 2^n
        - T は +=, -= を持つ型
            例: int, long long, modint など

    [備考]
        - 配列 A は破壊的に変更される
        - 先頭 2^n 要素のみを処理する
        - des は descending / down-set, asc は ascending / up-set の意味
        - OR 畳み込み，AND 畳み込み，高速包除原理などでよく使う

    [verified at]
        -
*/

template <typename T>
void zt_des(vector<T> &A, int n) {
    // A[S] <- Σ_{T ⊆ S} A[T]
    int N = 1 << n;
    assert((int)A.size() >= N);

    for (int i = 0; i < n; i++) {
        for (int S = 0; S < N; S++) {
            if (S & (1 << i)) {
                A[S] += A[S ^ (1 << i)];
            }
        }
    }
}

template <typename T>
void mb_des(vector<T> &A, int n) {
    // A[S] <- Σ_{T ⊆ S} μ(T,S) A[T]
    // zt_des の逆変換
    int N = 1 << n;
    assert((int)A.size() >= N);

    for (int i = 0; i < n; i++) {
        for (int S = 0; S < N; S++) {
            if (S & (1 << i)) {
                A[S] -= A[S ^ (1 << i)];
            }
        }
    }
}

template <typename T>
void zt_asc(vector<T> &A, int n) {
    // A[S] <- Σ_{S ⊆ T} A[T]
    int N = 1 << n;
    assert((int)A.size() >= N);

    for (int i = 0; i < n; i++) {
        for (int S = 0; S < N; S++) {
            if (S & (1 << i)) {
                A[S ^ (1 << i)] += A[S];
            }
        }
    }
}

template <typename T>
void mb_asc(vector<T> &A, int n) {
    // A[S] <- Σ_{S ⊆ T} μ(S,T) A[T]
    // zt_asc の逆変換
    int N = 1 << n;
    assert((int)A.size() >= N);

    for (int i = 0; i < n; i++) {
        for (int S = 0; S < N; S++) {
            if (S & (1 << i)) {
                A[S ^ (1 << i)] -= A[S];
            }
        }
    }
}