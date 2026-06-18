/*
  <分割マトロイド>
    - Eを互いに素な部分集合E_1,...,E_kに分割し，非負整数r_1,...,r_kを定める．
        各成分iに対して|I\cap E_i|<=r_iを満たすI全体の集合を独立集合族とする．
        (ex) グラフGの独立集合Sをとる．Sの各点に対して，その点から出る辺数が高々r_iである
        ような辺の選び方はマトロイドをなす

    [実装/関数]
        - PartitionMatroid(int n, int k, vector<int> r, vector<int> part) :
            n : 台集合のサイズ  k : 分割の個数
            r : 各分割に対するr_i  part : 各i(1<=i<=n)がどの分割に属するか
        - Matroid : struct(class)として実装する．以下のような要素が必要
            - int size() : 全体の要素数を返す
            - void set(vector<bool> I) : 現在の独立集合I(vector<bool>で与えられる)に対して内部状態を構築
            - vector<int> circuit(int e) : Iに要素eを加えた集合に含まれるサーキットの点集合をvector<int>で得る
    
    [計算時間]
        - 重みなし : O(nr(n+c)) (rはランクのmax, cはサーキットクエリにかかる時間)
        - 重みあり : O(nr(n^2+c)) (O(nr(r+c+logn))に改善可能)
    
    [備考]
        - 
    
    [depends on]
        - shortest-path(このコードに記載)

    [参照]
        - https://hitonanode.github.io/cplib-cpp/combinatorial_opt/matroid_intersection.hpp.html

    [verified at]
        - 
        
*/

class PartitionMatroid {
    public:
        int n;                     // 地集合のサイズ（全要素数）
        int k;                     // 分割数（部分の個数）
        vector<int> r;           // 各部分の容量（非負整数 r_i, size = k）
        vector<int> part;        // 各要素が属する部分の番号 (0-indexed, size = n)
        
        // 内部状態：set() 呼び出し時に現在の独立集合から計算する
        vector<int> cnt;         // 各部分ごとに現在選ばれている要素数（size = k）
        vector<bool> currentI;   // 現在の独立集合 I (size = n)
    
        // コンストラクタ: 地集合のサイズ n，部分数 k，各部分の容量 r，および
        // 各要素の属する部分を表す part を与える．
        PartitionMatroid(int n, int k, const vector<int>& r, const vector<int>& part)
            : n(n), k(k), r(r), part(part) {}
    
        // 地集合のサイズを返す
        int size() const {
            return n;
        }
    
        // 現在の独立集合 I に基づいて内部状態を更新する
        // I[i] == true ならば、要素 i が選ばれているものとする
        void set(const vector<bool>& I) {
            cnt.assign(k, 0);
            currentI = I;
            for (int i = 0; i < n; i++) {
                if (I[i])
                    cnt[ part[i] ]++;
            }
        }
    
        // 要素 e を現在の独立集合 I に追加した場合の回路（circuit）を求める
        // 分割マトロイドでは、もし e が属する部分 p で cnt[p] < r[p] ならば
        // e を追加しても独立性が保たれるので空のベクトルを返す。
        // 逆に、cnt[p] == r[p] ならば、回路は I∩E_p と e の全体となる。
        std::vector<int> circuit(int e) {
            int p = part[e];
            if (cnt[p] < r[p]) {
                // 追加しても容量制限を超えないので、依存は起きず回路は存在しない
                return std::vector<int>();
            }
            // 部分 p は既に満杯なので、I におけるその部分の要素全体と e の和が回路となる
            std::vector<int> cyc;
            for (int i = 0; i < n; i++) {
                if (currentI[i] && part[i] == p) {
                    cyc.push_back(i);
                }
            }
            cyc.push_back(e);
            return cyc;
        }
};