template<typename T>
class csum {
	vec<T> v;
public:
	csum(vec<T>& a) { init(a); }
	csum() {}
	csum(int sz) { init(sz); }
	void init(int sz) { v = vector<T>(sz + 1, 0); }
	void init(vec<T>& a) { v = a;v.push_back(0); build(); }
	void build() {
		for (int i = 1; i < v.size(); i++) v[i] += v[i - 1];
	}
	void add(int l, int r, T x) {
		v[l] += x;
		v[r] -= x;
	}//[l,r)
	//[l,r]
	T a(int l, int r) {
		if (r < l) return 0;
		return v[r] - (l == 0 ? 0 : v[l - 1]);
	}
	//[l,r)
	T b(int l, int r) {
		return a(l, r - 1);
	}
	T a(pair<int, int>t) {
		return a(t.first, t.second);
	}
	T b(pair<int, int>t) {
		return b(t.first, t.second);
	}
	T operator[](int x)const {
		return v[x];
	}
};