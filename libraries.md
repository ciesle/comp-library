# libraryの一覧

## algorithm
- Mo : 先頭要素が l, 2番目の要素が r である構造体の配列と、lの最大値、1つ伸ばしたとき、1つ縮めた時の動作を指定する
  - process を呼び出すと、適切にlambdaを呼び出したのち次の区間を返す。whileで繰り返し呼び出して使う。終了判定は頑張る

## array
- ntt : nttをする
- fps: fps関連の演算をまとめている。
  - pow : 繰り返し二乗法を実装
  - div : 長さNの多項式を長さMの多項式で割る。計算量はO(NM)

## array
- ntt.h : nttを実行する。残りの高速化（バタフライを基数3で展開、モンゴメリ剰余乗算をかける場所を減らす、reduce演算の最適化、simdの範囲の拡張、本当にuintである必要ある？）

## data_structure
- binary_indexed_tree : 一点更新、区間クエリ、二分探索に対応
- trie_tree : 文字列の集合を与えると、TRIE木を構築して返す（Treeを返すようにしたい）
- Binary_trie_tree : バイナリートライ（Trie_treeと同じ）
- SquareDecomposition : 一点更新、区間クエリに対応

## graph
- Tree : 重み無し木をラップ。入力、深さ有線探索、深さ計算を自動化
- namori_circuit : なもりグラフ中の閉路を見つけてリストで出力

## math
- divisor : nの約数を返す
- divisors : n以下の整数について約数を返す
- eratos_sieve : []でその値が素数かどうか、countである範囲内の素数の数、listで素数のリストを返す
- extended_euclid : ax + by = gcd(a,b) となる x,y を求める
- modint : multicombで多項定理が使える
- modpow
- matrix : 基本演算＋吐き出し法を実装
- bitmatrix : 基本演算＋吐き出し法を実装
- kernel_basis : 与えられた行列のkernelの基底を出力する

## segtree
- seg_max, seg_min, seg_sum : まとめる
- seg_none : 何もしない（更新系）
- seg_update, seg_updsum, seg_add : 更新


## strings

## tools
- measure : Measure.start(0)とMeasure.end(0)の間の時間を計測。add_pointsで足した点の間の間隔を計測。最後にoutputを呼んですべて出力。
- debug : 大体全部いい感じに出力してくれる

## utils
- sorted_index : 配列を与えると、ソートした配列と元の配列での番号をpairにして返す
- ctz : 末尾の0の個数を返す
- popcount
- cycvec : 円環配列。indexが2Nを超えるとオーバーフロー
- fill_array : 多次元配列の初期化をできる