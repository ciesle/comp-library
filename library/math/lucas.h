#include<bits/stdc++.h>
using namespace std;
template<long long p>
long long lucas(long long n, long long m) {
	static vector<vector<long long>>table(p, vector<long long>(p, 0));
	for (int i = 0;i < p;i++) table[i][0] = table[i][i] = 1;
	for (int i = 1; i < p; i++) {
		for (int j = 1; j < i; j++) {
			table[i][j] = (table[i - 1][j - 1] + table[i - 1][j]) % p;
		}
	}
	long long res = 1;
	while (n > 0 || m > 0) {
		long long ni = n % p, mi = m % p;
		if (ni < mi) return 0;
		(res *= table[ni][mi]) %= p;
		n /= p, m /= p;
	}
	return res;
}