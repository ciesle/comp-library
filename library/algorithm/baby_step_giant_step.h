#pragma once
#include<functional>
using std::function;
//start, goal, singlestep, multistep
//ret=step数, 不可能なら-1
template<typename T>
long long baby_step_giant_step(T x, T y, long long step, function<T(T)>single, function<T(T, long long)>multi) {
	if (x == y) return 0;
	long long m = sqrt(step) + 1;
	vector<pair<T, long long>>small(m);
	small[0] = make_pair(y, 0);
	for (long long i = 1;i < m;i++) {
		small[i] = make_pair(single(small[i - 1].first), -i);
	}
	sort(small.begin(), small.end());
	for (long long i = m;i <= step + 3 * m;i += m) {
		T now = multi(x, i);
		int k = lower_bound(small.begin(), small.end(), make_pair(now, -m)) - small.begin();
		if (k < small.size() && small[k].first == now) {
			return i + small[k].second;
		}
	}
	return -1;
}