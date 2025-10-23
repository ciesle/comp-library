template<typename T>
class TopK {
	vector<T>idx;
	BIT<T>cnt, val;
	int num_cnt = 0;
public:
	TopK(vector<T>index) :idx(index) {
		sort(idx.begin(), idx.end());
		idx.erase(unique(idx.begin(), idx.end()), idx.end());
		cnt.init(idx.size());
		val.init(idx.size());
	}
	void add(T x) {
		cnt.add(lower_bound(idx.begin(), idx.end(), x) - idx.begin(), 1);
		val.add(lower_bound(idx.begin(), idx.end(), x) - idx.begin(), x);
		num_cnt++;
	}
	void remove(T x) {
		cnt.add(lower_bound(idx.begin(), idx.end(), x) - idx.begin(), -1);
		val.add(lower_bound(idx.begin(), idx.end(), x) - idx.begin(), -x);
		num_cnt--;
	}
	//大きいほうからK個目(0~)
	T topK(int k) {
		assert(k < num_cnt);
		return idx[cnt.lower_bound(num_cnt - k)];
	}
	//大きいほうからk個の和(1~)
	T topKSum(int k) {
		if (k == 0) return 0;
		int t = cnt.lower_bound(num_cnt - k + 1);
		return val.query(t, idx.size()) - idx[t] * (cnt.query(t, idx.size()) - k);
	}
	//以下未検証
	//小さいほうからK個目(0~)
	T bottomK(int k) {
		return idx[cnt.lower_bound(k + 1)];
	}
	//小さいほうからk個の和(1~)
	T bottomKSum(int k) {
		int t = cnt.lower_bound(k);
		return val.query(0, t + 1) - idx[t] * (cnt.query(0, t + 1) - k);
	}
};