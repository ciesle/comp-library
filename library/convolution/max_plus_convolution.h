//C[i]=max(A[x], B[y]) (x+y=i)
//Bは上凸(max), 下凸(min)
template<typename T, bool max_or_min = true>
vector<T> max_plus_convolution(vector<T>& a, vector<T>& b) {
	int n = a.size(), m = b.size();
	vector<T>c(n + m - 1);
	T zero;
	if constexpr (max_or_min) zero = numeric_limits<T>::min();
	else zero = numeric_limits<T>::max();
	auto get = [&](int x, int y) {
		if (x < 0 || x >= n + m || y < 0 || y >= n || x - y < 0 || x - y >= m) return zero;
		return a[y] + b[x - y];
		};
	auto dfs = [&](int il, int ir, int al, int ar, auto& dfs)->void {
		if (il >= ir || al >= ar) return;
		int im = (il + ir) >> 1, idx = al;
		T now = zero;
		for (int i = al;i < ar;i++) {
			T v = get(im, i);
			if constexpr (max_or_min) {
				if (now < v) now = v, idx = i;
			}
			else {
				if (now > v) now = v, idx = i;
			}
		}
		c[im] = now;
		dfs(il, im, al, idx + 1, dfs);
		dfs(im + 1, ir, idx, ar, dfs);
		};
	dfs(0, n + m - 1, 0, n, dfs);
	return c;
}