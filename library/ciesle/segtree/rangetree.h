template<typename T, typename V, typename segtree>
class rangetree {
	int n, rr;
	using P = pair<T, T>;
	vector<int>idx;
	vector<P>xy;
	vector<vector<P>>yx;
	vector<V>v;
	vector<segtree>seg;
	V zero;
	function<T(T, T)>eval;
	void update_internal(int i, V v) {
		i += rr;
		while (i > 0) {
			int l = lower_bound(yx.begin(), yx.end(), P{ xy[i].second,i }) - yx.begin();
			seg.update(l, l + 1, v);
			i >>= 1;
		}
	}
public:
	rangetree(vector<P>& pos, vector<V> val, V zero, function<T(T, T)>eval) :n(pos.size()), idx(n), zero(zero), eval(eval) {
		rr = 1;
		while (rr < n) rr <<= 1;
		iota(idx.begin(), idx.end(), 0);
		sort(idx.begin(), idx.end(), [&](int i, int j) {
			return pos[i] < pos[j];
			});
		xy.resize(n);v.resize(n);
		for (int i = 0;i < n;i++) xy[i] = pos[idx[i]], v[i] = val[idx[i]];

		yx.resize(2 * rr);
		seg.resize(2 * rr);
		for (int i = 0;i < n;i++) {
			yx[rr + i].emplace_back(P{ xy[i].second, i });
			vector<V>v2{ v[i] };
			seg[rr + i] = segtree(v2);
		}

		for (int i = rr - 1;i > 0;i--) {
			yx[i].resize(yx[i * 2].size() + yx[i * 2 + 1].size());
			merge(yx[i * 2].begin(), yx[i * 2].end(), yx[i * 2 + 1].begin(), yx[i * 2 + 1].end(), yx[i].begin());
			vector<V>v2(yx[i].size());
			for (int j = 0;j < yx[i].size();j++) v2[j] = v[yx[i][j].second];
			seg[i] = segtree(v2);
		}
	}
	void update(int i, V v) {
		update_internal(i, v);
	}
	void update(T x, T y, V v) {
		update_internal(lower_bound(xy.begin(), xy.end(), P{ x,y }) - xy.begin());
	}
	//(x,y) -> (u, v) 
	V querya(T x, T y, T u, T v) {
		return queryb(x, y, u - x, v - y);
	}
	//(x,y) -> (x+a, y+b)
	V queryb(T x, T y, T a, T b) {
		int l = lower_bound(xy.begin(), xy.end(), P{ x,numeric_limits<T>::min() }) - xy.begin();
		int r = lower_bound(xy.begin(), xy.end(), P{ x + a,numeric_limits<T>::min() }) - xy.begin();
		l += rr, r += rr;
		V ret = zero;
		while (l < r) {
			if (l & 1) {
				int s = lower_bound(yx[l].begin(), yx[l].end(), P{ y,numeric_limits<T>::min() }) - yx[l].begin();
				int t = lower_bound(yx[l].begin(), yx[l].end(), P{ y + b,numeric_limits<T>::min() }) - yx[l].begin();
				ret = eval(ret, seg[l].query(s, t));
				l++;
			}
			if (r & 1) {
				r--;
				int s = lower_bound(yx[r].begin(), yx[r].end(), P{ y,numeric_limits<T>::min() }) - yx[r].begin();
				int t = lower_bound(yx[r].begin(), yx[r].end(), P{ y + b,numeric_limits<T>::min() }) - yx[r].begin();
				ret = eval(ret, seg[r].query(s, t));
			}
			l >>= 1;r >>= 1;
		}
		return ret;
	}
};