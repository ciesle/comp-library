//ゼータ変換の逆
template<typename T>
vector<T>fast_mebius_transform(vector<T>& a) {
	int n = 0;
	while ((1 << n) < a.size()) n++;
	vector<T>ret(1 << n);
	for (int i = 0;i < a.size();i++) ret[i] = a[i];
	for (int j = 0;j < n;j++) {
		for (int i = 0;i < (1 << n);i++) {
			if ((i >> j) & 1) {
				ret[i] -= ret[i ^ (1 << j)];
			}
		}
	}
	return ret;
}