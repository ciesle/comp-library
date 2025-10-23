ll sqrt_ll(ll x) {
	ll mn = 0, mx = sqrt(LONG_LONG_MAX), mid;
	while (mx - mn > 1) {
		mid = (mx + mn) >> 1;
		if (mid * mid > x) mx = mid;
		else mn = mid;
	}
	return mn;
}