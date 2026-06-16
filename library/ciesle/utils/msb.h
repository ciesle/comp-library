template<int max = 1 << 20>int msb(long long x) {
	static vector<int>ret(max, -1);
	if (ret[1] < 0) {
		for (int i = 0;(1 << i) < max;i++) {
			for (int j = 0;j < (1 << i);j++) {
				ret[(1 << i) + j] = i;
			}
		}
	}
	if (x < max) return ret[x];
	else {
		for (long long i = 63;i >= 0;i--) {
			if ((x >> i) & 1) return i;
		}
		return -1;
	}
}