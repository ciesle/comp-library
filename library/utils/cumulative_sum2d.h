template<typename T>
class csum2 {
	int r, c;
	vector<vector<T>>arr;
	inline void calc(int x, int y, T val) {
		arr[x][y] = val + (x > 0 ? arr[x - 1][y] : 0) + (y > 0 ? arr[x][y - 1] : 0) - (x > 0 && y > 0 ? arr[x - 1][y - 1] : 0);
	}
public:
	csum2() {}
	csum2(int r, int c) :r(r), c(c), arr(r, vector<T>(c)) {}
	csum2(vector<vector<T>>& arr) :r(arr.size()), c(arr[0].size()), arr(arr) { build(); }
	void update(int x, int y, T val) {
		arr[x][y] = val;
	}
	void build() {
		for (int i = 0;i < r;i++)
			for (int j = 0;j < c;j++) {
				calc(i, j, arr[i][j]);
			}
	}
	T a(int x, int y, int x2, int y2) {
		if (x > x2 || y > y2) return 0;
		return arr[x2][y2] - (x > 0 ? arr[x - 1][y2] : 0) - (y > 0 ? arr[x2][y - 1] : 0) + ((x > 0 && y > 0) ? arr[x - 1][y - 1] : 0);
	}
	T b(int x, int y, int xlen, int ylen) {
		return a(x, y, x + xlen - 1, y + ylen - 1);
	}
};