template<typename T>
T three_part_search(T begin, T end, function<bool(T, T)>func) {
	T l = begin, r = end;
	for (int i = 0;i < 50;i++) {
		T mid1 = (r - l) / 3 + l, mid2 = (r - l)*2 / 3 + l;
		if (func(mid1, mid2)) r = mid2;
		else l = mid1;
	}
	if ((r - l) / 2 + l >= end || func(l, (r - l) / 2 + l)) return l;
	return (r - l) / 2 + l;
}