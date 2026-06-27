
//実装例(x^2 = a mod pとなるようなxがあれば返す(存在しないなら-1を返す)))
ll Tonelli_shanks(ll a, ll p){
    if(a % p == 0) return 0;
    if(p == 2) return a;
    if(pow_mod(a, (p-1)/2, p) != 1) return -1; 
    ll q = p - 1;
    ll Q = 0;
    while(q % 2 == 0){
        q /= 2;
        Q++;
    }
    ll b = 1;
    while(pow_mod(b, (p-1)/2, p) == 1){
        b++; //randomにしてもよい。
    }
    ll x = pow_mod(a, (q+1)/2, p);
    ll b = pow_mod(b, q, p);

    ll shift = 2;
    while(pow_mod(x, 2, p) != a){
        ll error = inv_mod(a, p) * pow_mod(x, 2, p) % p;
        if(pow_mod(error, 1 << (Q - shift), p) != 1){
            x = x * b % p;
        }
        b = pow_mod(b, 2, p);
        shift +=1;
    }
return x;   
}