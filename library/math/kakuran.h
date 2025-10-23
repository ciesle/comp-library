template<typename T>
vector<T> kakuran(int n) {
	vector<T>a(n);
	a[0] = 0; if (n == 1) return a;
	a[1] = 0; if (n == 2) return a;
	a[2] = 1; if (n == 3) return a;
	for (int i = 3;i < n;i++) {
		a[i] = (a[i - 2] + a[i - 1]) * (i - 1);
	}
	return a;
}