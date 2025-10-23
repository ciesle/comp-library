template<typename T>
vector<T>natural_number_decomposition(T n) {
	vector<T>ret;
	T x = 1;
	while (n) {
		ret.emplace_back(min(x, n));
		n -= min(x, n);
		x *= 2;
	}
	return ret;
}