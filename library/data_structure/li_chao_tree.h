template<const bool minimum>
class LiChaoTree {
	using ll = long long;
	int n, rr;
	const ll inf = minimum ? 4e18 : -4e18;
	const ll mn = -2e9 - 1, mx = 2e9 + 1;
	static inline ll wmin(ll x, ll y) { return min(x, y); }
	static inline ll wmax(ll x, ll y) { return max(x, y); }
	static inline bool bmin(ll x, ll y) { return x < y; }
	static inline bool bmax(ll x, ll y) { return x > y; }
	ll(* const func)(ll, ll) = minimum ? &wmin : &wmax;
	bool (* const bfunc)(ll, ll) = minimum ? &bmin : &bmax;
	struct node {
		ll l, r;
		ll a, b;
		int lch, rch;
		node() :l(0), r(0), a(0), b(inf), lch(-1), rch(-1) {}
		node(ll l, ll r, ll a, ll b, int lch = -1, int rch = -1) :
			l(l), r(r), a(a), b(b), lch(lch), rch(rch) {}
	};
	vector<node>dat;
public:
	LiChaoTree() {
		dat.assign(1, node{ mn,mx,0,inf });
	}
	//ax+b(全域)
	void add_line(ll a, ll b) {
		add_line_internal(0, mn, mx, a, b);
	}
	//座標がl<=x<rにおいてax+bを追加
	void add_line(ll a, ll b, ll l, ll r) {
		add_line_internal(0, l, r, a, b);
	}
	//点tにおける最小値を求める
	ll query(ll x) {
		return query_internal(0, x);
	}
private:
	inline void split(int i) {
		dat[i].lch = dat.size();
		dat[i].rch = dat.size() + 1;
		dat.push_back(node(dat[i].l, (dat[i].l + dat[i].r) >> 1, 0, inf));
		dat.push_back(node((dat[i].l + dat[i].r) >> 1, dat[i].r, 0, inf));
	}
	void add_line_internal(int i, const ll& u, const ll& v, ll a, ll b) {
		if (i < 0 || dat[i].r <= u || v <= dat[i].l || dat[i].l == dat[i].r) return;
		node* nd = &dat[i];
		if (u <= nd->l && nd->r <= v) {
			auto& c = nd->a, d = nd->b;
			if (!bfunc(a * nd->l + b, c * nd->l + d) &&
				!bfunc(a * (nd->r - 1) + b, c * (nd->r - 1) + d))
				return;
			if (bfunc(a * nd->l + b, c * nd->l + d) &&
				bfunc(a * (nd->r - 1) + b, c * (nd->r - 1) + d)) {
				nd->a = a, nd->b = b;
				return;
			}
			if (dat[i].lch < 0) split(i);
			nd = &dat[i];
			ll t = dat[nd->lch].r;
			if (bfunc(a * t + b, dat[i].a * t + dat[i].b)) swap(a, dat[i].a), swap(b, dat[i].b);
			if (bfunc(a * nd->l + b, dat[i].a * nd->l + dat[i].b)) add_line_internal(nd->lch, u, v, a, b);
			else add_line_internal(nd->rch, u, v, a, b);
			return;
		}
		if (dat[i].lch < 0) split(i);
		add_line_internal(dat[i].lch, u, v, a, b);
		add_line_internal(dat[i].rch, u, v, a, b);
	}
	ll query_internal(int i, ll& x) {
		auto& nd = dat[i];
		if (nd.lch < 0) return nd.a * x + nd.b;
		return func(nd.a * x + nd.b, (x < dat[nd.lch].r) ? query_internal(nd.lch, x) : query_internal(nd.rch, x));
	}
};