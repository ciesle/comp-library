/*
  <extgcd>
    - ax + by = gcd(a,b) なる(x,y)を計算する

    [実装/関数]
        - T extgcd(T a, T b, T &x, T &y)
            : x, y は型Tの変数．x = 0, y = 0 として渡す．
            : ax + by = gcd(a,b) なる (x,y) にx, yを変化させる．また，関数の返り値としてgcd(a,b) を返す．
                (a,bの符号によらずgcdは非負の値として得られる)
            : 得られるx,yは正であるとは限らない．
            : |x| <= |b/gcd(a,b)| を満たすはず

    [計算時間]
        - O(log max(|a|,|b|))
    
    [備考]
        - 
    
    [参照]
        - 

    [verified at]
        - 
    
    [使用例] 
        
*/

template<typename T>
T extgcd(T a, T b, T &x, T &y) {
    if (b == 0) {
        if (a >= 0) {
            x = 1;
            y = 0;
            return a;
        } else {
            x = -1;
            y = 0;
            return -a;
        }
    }

    T d = extgcd(b, a % b, y, x);
    y -= (a / b) * x;
    return d;
}