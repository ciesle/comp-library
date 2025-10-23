template<typename T, bool is_min = true, bool is_index = false>
class sptable {
	int n, height;
	vector<int>log;
	vector<T>dat;
	using S = typename conditional<is_index, int, T>::type;
	vector<vector<S>>table;
	inline bool func(S a, S b) {
		if constexpr (is_index) {
			if (a == -1) return false;
			if (b == -1) return true;
			if constexpr (is_min) return dat[a] <= dat[b];
			else return dat[a] >= dat[b];
		}
		else {
			if constexpr (is_min) return a < b;
			else return a > b;
		}
	};
	static constexpr S zero = is_index ? S(-1) : S(((is_min ? numeric_limits<T>::max() : numeric_limits<T>::min())));
public:
	sptable() {}
	sptable(vector<T>a) :n(a.size()), dat(a), log(n + 1) {
		height = 1;
		while ((1 << height) <= n) height++;
		for (int i = 0;i <= height;i++) {
			for (int j = (1 << i);j <= n && j < (1 << (i + 1));j++) {
				log[j] = i;
			}
		}
		dat.resize((1 << height), is_min ? numeric_limits<T>::max() : numeric_limits<T>::min());
		table.assign(height, vector<T>(n, zero));
		for (int i = 0;i < n;i++) table[0][i] = is_index ? i : a[i];
		for (int i = 0;i < height - 1;i++) {
			for (int j = 0;j + (1 << (i + 1)) <= n;j++) {
				if (func(table[i][j], table[i][j + (1 << i)])) table[i + 1][j] = table[i][j];
				else table[i + 1][j] = table[i][j + (1 << i)];
			}
		}
	}
	//[l,r]
	S a(int l, int r) {
		return b(l, r + 1);
	}
	//[l,r)
	S b(int l, int r) {
		if (r <= l) return zero;
		int h = log[r - l];
		if (func(table[h][l], table[h][r - (1 << h)])) return table[h][l];
		return table[h][r - (1 << h)];
	}
};