//有理数クラスを作る

struct Rat{
    ll m, n; //r = m/n;
    Rat() : m(0), n(1) {};
    Rat(ll m, ll n) : m(m), n(n) {;
    if (n == 0) {
        throw std::invalid_argument("Denominator cannot be zero.");
    }
    }

    bool operator<(const Rat& r) const{
        return m*r.n < n*r.m;
    }

    Rat operator+(const Rat& r){
        Rat ret;
        ret.m = m*r.n + r.m*n;
        ret.n = n*r.n;
        if(ret.m==0) return ret;
        ll g=gcd(ret.m,ret.n);
        ret.m/=g;
        ret.n/=g;
        return ret;
    }

    Rat operator-(const Rat& r){
        Rat ret;
        ret.m = m*r.n - r.m*n;
        ret.n = n*r.n;
        if(ret.m==0) return ret;
        ll g=gcd(ret.m,ret.n);
        ret.m/=g;
        ret.n/=g;
        return ret;
    }

    Rat operator*(const Rat& r){
        Rat ret;
        ret.m = m*r.m;
        ret.n = n*r.n;
        if(ret.m==0) return ret;
        ll g=gcd(ret.m,ret.n);
        ret.m/=g;
        ret.n/=g;
        return ret;
    }

    Rat operator/(const Rat& r){
        if (r.m == 0) {
            throw std::invalid_argument("Division by zero is undefined.");
        }
        Rat ret;
        ret.m = m*r.n;
        ret.n = n*r.m;
        if(ret.m==0) return ret;
        ll g=gcd(ret.m,ret.n);
        ret.m/=g;
        ret.n/=g;
        return ret;
    }
};