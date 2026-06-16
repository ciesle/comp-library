template<typename T>
vector<T> inverse_transform(vector<T>a) {
	vector<T>ret(a.size(), -1);
	for (int i = 0;i < a.size();i++) if (~a[i]) ret[a[i]] = i;
	return ret;
}