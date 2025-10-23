template<typename T>
struct matrix {
	int h, w;
	vector<vector<T>>dat;
	matrix() {}
	matrix(int h, int w, bool identity = false) :h(h), w(w), dat(h, vector<T>(w, T())) {
		if (identity) for (int i = 0;i < h;i++) dat[i][i] = 1;
	}
	matrix<T> operator+(matrix<T>& a) const {
		matrix<T> b(h, w);
		for (int i = 0;i < h;i++)for (int j = 0;j < w;j++) {
			b[i][j] = dat[i][j] + a[i][j];
		}
		return b;
	}
	matrix<T> operator-(matrix<T>& a) const {
		matrix<T> b(h, w);
		for (int i = 0;i < h;i++)for (int j = 0;j < w;j++) {
			b[i][j] = dat[i][j] - a[i][j];
		}
		return b;
	}
	matrix<T> operator*(matrix<T>& a) const {
		matrix<T> b(h, a.w);
		for (int i = 0;i < h;i++)for (int j = 0;j < a.w;j++)
			for (int k = 0;k < w;k++) {
				b[i][j] += dat[i][k] * a.dat[k][j];
			}
		return b;
	}
	matrix<T>pow(long long t) {
		matrix<T>b = *this, c(h, w, true);
		while (t > 0) {
			if (t & 1) c = c * b;
			b = b * b;
			t >>= 1;
		}
		return c;
	}
	T det() {
		matrix<T>b(*this);
		assert(w == h);
		T ret = 1;
		for (int i = 0;i < w;i++) {
			int idx = -1;
			for (int j = i;j < h;j++) if (b[j][i] != 0) idx = j;
			if (idx == -1) return 0;
			if (i != idx) {
				ret *= -1;
				swap(b[i], b[idx]);
			}
			ret *= b[i][i];
			T vv = b[i][i];
			for (int j = 0;j < w;j++) b[i][j] /= vv;
			for (int j = i + 1;j < h;j++) {
				T a = b[j][i];
				for (int k = 0;k < w;k++) b[j][k] -= b[i][k] * a;
			}
		}
		return ret;
	}
	vector<T>& operator[](const int i) {
		return dat[i];
	}
	void print() {
		for (int i = 0;i < h;i++)
			for (int j = 0;j < w;j++)
				cout << dat[i][j] << " \n"[j == w - 1];
	}
};