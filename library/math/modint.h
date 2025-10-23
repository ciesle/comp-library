template<unsigned int mod>
class mymodint {
public:
	unsigned int v;
	mymodint(long long vv = 0) noexcept {
		if (likely(vv >= 0)) v = vv % mod;
		else v = (vv % mod + mod) % mod;
	}
	constexpr static int fn_ = (int)3e5 + 5;
	constexpr static int iv_ = (int)3e5 + 5;
	static inline vector<mymodint>fct, comp, iv;
	mymodint pow(long long x) const {
		mymodint b(v), c(1);
		while (x) {
			if (x & 1) c *= b;
			b *= b;
			x >>= 1;
		}
		return c;
	}
	inline mymodint& s(unsigned int vv) noexcept {
		if (likely(vv < mod)) v = vv;
		else v = vv - mod;
		return *this;
	}
	inline mymodint inv()const noexcept {
		if (unlikely(iv.empty())) invinit();
		if (likely(v < iv_)) return iv[v];
		return pow(mod - 2);
	}
	inline mymodint operator+()const { return *this; }
	inline mymodint operator-()const { return mymodint() - *this; }
	inline mymodint& operator++() { ++v; if (unlikely(v == mod)) v = 0; return *this; }
	inline mymodint& operator--() { if (unlikely(v == 0)) v = mod; --v; return *this; }
	inline mymodint operator++(int) { mymodint res = *this; ++*this; return res; }
	inline mymodint operator--(int) { mymodint res = *this; --*this; return res; }
	inline mymodint& operator+=(const mymodint b) { return s(v + b.v); }
	inline mymodint& operator-=(const mymodint b) { return s(v + mod - b.v); }
	inline mymodint& operator*=(const mymodint b) { unsigned long long z = v; z *= b.v; v = (unsigned int)(z % mod); return *this; }
	inline mymodint& operator/=(const mymodint b) { return *this = *this * b.inv(); }
	inline mymodint operator+(const mymodint b) const { return mymodint(v) += b; }
	inline mymodint operator-(const mymodint b) const { return mymodint(v) -= b; }
	inline mymodint operator*(const mymodint b) const { return mymodint(v) *= b; }
	inline mymodint operator/(const mymodint b) const { return mymodint(v) /= b; }
	template<typename T>inline friend mymodint operator+(const T a, const mymodint& b) { return mymodint(a) += b; }
	template<typename T>inline friend mymodint operator-(const T a, const mymodint& b) { return mymodint(a) -= b; }
	template<typename T>inline friend mymodint operator*(const T a, const mymodint& b) { return mymodint(a) *= b; }
	template<typename T>inline friend mymodint operator/(const T a, const mymodint& b) { return mymodint(a) /= b; }
	friend ostream& operator<<(ostream& os, const mymodint& m) {
		return os << m.v;
	}
	friend istream& operator>>(istream& is, mymodint& m) {
		int x; is >> x; m = mymodint(x);
		return is;
	}
	bool operator==(const mymodint& r)const { return v == r.v; }
	bool operator!=(const mymodint& r)const { return v != r.v; }
	explicit operator bool()const { return v; }
	explicit operator int()const { return v; }
	mymodint comb(long long k) {
		if (unlikely(k > v)) return mymodint();
		if (unlikely(fct.empty())) combinit();
		if (unlikely(v >= fn_)) {
			if (k > v - k) k = v - k;
			mymodint tmp(1);
			for (int i = v - k + 1; i <= v; i++) tmp *= mymodint(i);
			return tmp * comp[k];
		}
		return fct[v] * comp[k] * comp[v - k];
	}//nCk
	mymodint perm(long long k) {
		if (unlikely(k > v)) return mymodint();
		if (unlikely(fct.empty())) combinit();
		if (unlikely(v >= fn_)) {
			mymodint tmp(1);
			for (int i = v - k + 1; i <= v; i++) tmp *= mymodint(i);
			return tmp;
		}
		return fct[v] * comp[v - k];
	}//nPk
	mymodint fact() {
		if (unlikely(fct.empty())) combinit();
		if (unlikely(v >= fn_)) {
			mymodint tmp(1);
			for (int i = v; i >= 1; i--) tmp *= mymodint(i);
			return tmp;
		}
		return fct[v];
	}
	static void combinit() {
		fct.assign(fn_, mymodint());
		fct[0] = 1;
		for (int i = 1; i < fn_; i++) fct[i] = fct[i - 1] * mymodint(i);
		comp.assign(fn_, mymodint());
		comp[fn_ - 1] = fct[fn_ - 1].inv();
		for (int i = fn_ - 2; i >= 0; i--) comp[i] = comp[i + 1] * mymodint(i + 1);
	}
	static void invinit() {
		iv.assign(iv_, mymodint());
		for (int i = 1; i < iv_; i++) iv[i] = mymodint(i).pow(mod - 2);
	}
	template<typename... T>
	static mymodint multicomb(T... args) {
		int len = sizeof...(args);
		mymodint ary[] = { args... };
		mymodint ret = 1, sum = ary[0];
		for (int i = 1;i < len;i++) {
			sum += ary[i];
			ret *= sum.comb(ary[i]);
		}
		return ret;
	}
};
using mint = mymodint<mod>;