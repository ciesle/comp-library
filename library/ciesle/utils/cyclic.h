template<typename T>
class cyclic {
	int n;
	vector<T>v;
public:
	cyclic() {}
	cyclic(int size, T ini = T()) :n(size), v(n, ini) {}
	T& operator[](int idx)& { return v[(idx % n + n) % n]; }
	const T& operator[](int idx) const& { return v[(idx % n + n) % n]; }
	T operator[](int idx)&& { return v[(idx % n + n) % n]; }
};