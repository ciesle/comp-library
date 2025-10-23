class Rhash {
	inline static constexpr int MAX_SIZE = 3000005;
	inline static long long b1 = 1007, b2 = 1009;
	inline static long long m1 = 1e9 + 7, m2 = 1e9 + 9;
	inline static vector<long long>p1, p2;
	int size;
	vector<long long>h1, h2;
	static void init(int size = MAX_SIZE) {
		p1.assign(size + 1, 1);
		p2.assign(size + 1, 1);
		for (int i = 0; i < size; i++) {
			p1[i + 1] = p1[i] * b1 % m1;
			p2[i + 1] = p2[i] * b2 % m2;
		}
	}
public:
	Rhash() { if (p1.empty()) init(); }
	Rhash(const string& s) : size(s.size()), h1(s.size() + 1, 0), h2(s.size() + 1, 0) {
		if (p1.empty()) init();
		for (int i = 0; i < s.size(); i++) {
			h1[i + 1] = (h1[i] + s[i]) * b1 % m1;
			h2[i + 1] = (h2[i] + s[i]) * b2 % m2;
		}
	}
	//default: get hash of all part of the string
	pair<long long, long long> get(int l = 0, int r = -1) {
		if (r < 0) l = 0, r = size;
		int t1 = ((h1[r] - h1[l] * p1[r - l]) % m1 + m1) % m1;
		int t2 = ((h2[r] - h2[l] * p2[r - l]) % m2 + m2) % m2;
		return make_pair(t1, t2);
	}

	//connect two hashes
	static pair<long long, long long> connect(pair<long long, long long> l, pair<long long, long long> r, int r_len) {
		return make_pair((l.fs * p1[r_len] + r.fs % m1) % m1,
			(l.sc * p2[r_len] + r.sc % m2) % m2);
	}
};