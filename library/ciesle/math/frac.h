struct frac {
	long long x, y;
	frac(long long top, long long bottom) {
		ll g = gcd(top, bottom);
		x = top / g, y = bottom / g;
		if (y < 0) x *= -1, y *= -1;
	}
	bool operator<(const frac& a)const {
		return x * a.y < y * a.x;
	}
};