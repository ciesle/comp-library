int popcount(ll x) {
	int cnt = 0;
	for (int i = 0;i < 64;i++) {
		if (((x >> i) & 1) == 1) cnt++;
	}
	return cnt;
}