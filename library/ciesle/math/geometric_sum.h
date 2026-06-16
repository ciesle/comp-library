template<typename T>
T geometric_sum(T st, T mul, ll n) {
	return st * (mul.pow(n) - 1) / (mul - 1);
}