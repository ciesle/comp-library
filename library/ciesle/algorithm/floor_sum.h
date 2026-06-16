/*
sum 0..n-1 (floor(a*i+b)/m)
あまり理解していない
*/
long long floor_sum(long long a, long long b, long long n, long long m) {
	long long ret = 0;
	if (a >= m) {
		ret += n * (n - 1) / 2 * (a / m);
		a %= m;
	}
	if (b >= m) {
		ret += n * (b / m);
		b %= m;
	}
	if (a == 0) return ret + (b / m) * n;

	long long y = (a * n + b) / m, z = (y * m - b);
	if (y == 0) return ret;
	ret += (n - (z + a - 1) / a) * y;
	return ret + floor_sum(m, (a - z % a) % a, y, a);
}