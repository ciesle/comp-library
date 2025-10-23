void bin_sweep(vector<long long>& v) {
	int n = v.size();
	int cnt = 0;
	for (int i = 63;i >= 0;i--) {
		for (int j = cnt;j < n;j++) {
			if ((v[j] >> i) & 1) swap(v[j], v[cnt]);
		}
		if ((v[cnt] >> i) & 1) {
			for (int j = cnt + 1;j < n;j++) {
				if ((v[j] >> i) & 1) v[j] = v[j] ^ v[cnt];
			}
			cnt++;
		}
	}
	while (siz(v) > cnt) v.pop_back();
}