template<typename T>
vector<T>pow_preload(int n, T mul) {
	vector<T>ret(n + 1);
	ret[0] = 1;
	for (int i = 1;i <= n;i++) ret[i] = ret[i - 1] * mul;
	return ret;
}