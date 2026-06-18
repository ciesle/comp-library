/*
  <ゼータ変換 メビウス変換>
    - ゼータ変換とメビウス変換はposet上の関数に対して定義される．畳み込みを定めるには束である
        必要がある(つまり，posetであって任意の二元a,bに対しa以上かつb以上の元の最小元と
        a以下かつb以下の元の最大限が存在してほしい)
        - 参照: https://qiita.com/convexineq/items/afc84dfb9ee4ec4a67d5m

    -

    [実装/関数]
        - void zt_des(vector<T> A, int n) : 
            長さ2^n以上の配列Aに対して，A[i]を(iの部分集合jに対するA[j]の和)に変更する

        - void zt_asc(vector<T> A, int n) : 
            長さ2^n以上の配列Aに対して，A[i]を(iを含む集合jに対するA[j]の和)に変更する

        - void mb_asc(vector<A>, int n) : 
            
    
    [計算時間]
        - ゼータ変換/メビウス変換 : O(2^n*n)
    
    [備考]
        -
    
    [参照]
        - 

    [verified at]
        - 
        
*/

// bの下位集合全体の和
template <typename T>
void zt_des(vector<T> &A, int n) {
    int An = A.size();
    for(int i=0;i<n;i++){ //各次元に対しての更新
        for(int j=0;j<An;j++){ //各要素を見る
            if(j & (1LL<<i)) A[j] += A[j^(1LL<<i)];
        }
    }    
    return;
}

// bの上位集合全体の和
template <typename T>
void zt_asc(vector<T> &A, int n) {
    int An = A.size();
    for(int i=0;i<n;i++){
        for(int j=0;j<An;j++){
            if(j & (1LL<<i)) A[j^(1LL<<i)] += A[j];
        }
    }    
    return;
}

template <typename T>
void mb_asc(vector<T> &A, int n) {
    int An = A.size();
    for(int i=0;i<n;i++){
        for(int j=0;j<An;j++){
            if(j & (1LL<<i)) A[j^(1LL<<i)] -= A[j];
        }
    }    
    return;
}


/*
    2. 約数包除
        - 長さnの配列A(=(A[1],...,A[n]))に対して，bの倍数であるi(bの上位...)に対するA[i]の総和と，
            bの約数であるjに対するA[j]の総和を計算する
*/ 


/*
    3. min,maxでの畳み込み
        - 半順序集合(Poset)上の(ある環への)関数に対して，Pのmin(or max)と積による畳み込みを考える
            つまり， h(k) = \sum_{min(i,j)=k} f(i)*g(j) (0<=i,j<n) を求めたい．
            このとき，各点積として ζh = ζf * ζg が成り立つ．
              -> このような形に落とし込めば，畳み込みをゼータ変換・各点積とメビウス変換
                により行うことができる(nlogn)
            - (高速離散フーリエ変換では，畳み込みを求める上で多項式補完のアイデアを用いており，
                変換する列のサイズ長が2冪の時に小さい2つの計算に帰着されることで高速化される)
        
        - 一般に，束上での^とvによる畳み込みに対しても，ζh = ζf * ζgの関係が成り立つ
*/

/*
    4. gcdの畳み込み
        - p_1,p_2,...を素数を小さい方から並べた列として，正の整数nに対して
            n = (素因数p_1の個数, .... ) (=p(n))
        とN^Nの元に対応させると(実際にはある整数mに対してN^mの元と解釈)，gcd,lcmは
        対応先のN^mの元のmin,maxに対応する(つまり，gcd(x,y)=k <-> min(p(x),p(y))=p(k))
        よって3の手法により高速な畳み込みができる
*/