template<typename T, const int SIZE, const int PRIM_ROOT>
class Numeric_Theory_Transform {
	inline static T root[(1 << SIZE) + 1];
	inline static T inv_root[(1 << SIZE) + 1];
public:
	Numeric_Theory_Transform() {
		if (root[0]) return;
		root[0] = 1;
		for (int i = 1;i <= (1 << SIZE);i++) root[i] = root[i - 1] * PRIM_ROOT;
		inv_root[(1 << SIZE)] = 1;
		for (int i = (1 << SIZE) - 1;i >= 0;i--) inv_root[i] = inv_root[i + 1] * PRIM_ROOT;
	}
	void ntt(vector<T>& a, int log, bool inv) {
		int n = a.size();
		for (int i = 0;i < n;i++) {
			int j = 0;
			for (int k = 0;k < log;k++) j |= (i >> k & 1) << (log - 1 - k);
			if (i < j) swap(a[i], a[j]);
		}

		T s, t;
		for (int i = 1, u = 1;i < n;i <<= 1, u++) {
			for (int j = 0;j < i;j++) {
				if (j == 0) {
					for (int k = 0;k < n;k += i * 2) {
						s = a[j + k];
						t = a[j + k + i];
						a[j + k] = s + t;
						a[j + k + i] = s - t;
					}
				}
				else {
					T w = inv ? root[(1 << (SIZE - u)) * j] : inv_root[(1 << (SIZE - u)) * j];
					for (int k = 0;k < n;k += i * 2) {
						s = a[j + k];
						t = a[j + k + i] * w;
						a[j + k] = s + t;
						a[j + k + i] = s - t;
					}
				}
			}
		}
		if (inv) {
			for (int i = 0;i < n;i++) a[i] /= n;
		}
	}
};
template<typename T>
vector<T>convolution(vector<T>a, vector<T>b) {
	Numeric_Theory_Transform<T, 23, 15311432>ntt;
	int n = a.size(), m = b.size();
	int l = 1;
	while ((1 << l) < n + m - 1) l++;
	a.resize(1 << l, 0);
	b.resize(1 << l, 0);
	ntt.ntt(a, l, false);
	ntt.ntt(b, l, false);

	vector<T>c((1 << l));
	for (int i = 0;i < (1 << l);i++) c[i] = a[i] * b[i];
	ntt.ntt(c, l, true);
	c.resize(n + m - 1);
	return c;
}