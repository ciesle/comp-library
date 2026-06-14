template<typename T>
class mydeque {
	vector<T> front, back;
	pair<int, int>lenf, lenb;
public:
	mydeque() {
		lenf = { 0,0 };
		lenb = { 0,0 };
	}
	void push_front(T x) {
		if (lenb.fs > 0) {
			lenb.fs--;
			back[lenb.fs] = x;
		}
		else {
			if (lenf.sc < front.size()) {
				front[lenf.sc] = x;
				lenf.sc++;
			}
			else {
				front.push_back(x);
				lenf.sc++;
			}
		}
	}
	void push_back(T x) {
		if (lenf.fs > 0) {
			lenf.fs--;
			front[lenf.fs] = x;
		}
		else {
			if (lenb.sc < back.size()) {
				back[lenb.sc] = x;
				lenb.sc++;
			}
			else {
				back.push_back(x);
				lenb.sc++;
			}
		}
	}
	void emplace_back(T x) {
		push_back(x);
	}
	void pop_front() {
		if (lenf.sc > 0) {
			lenf.sc--;
		}
		else {
			lenb.fs++;
		}
	}
	void pop_back() {
		if (lenb.sc > 0) {
			lenb.sc--;
		}
		else {
			lenf.fs++;
		}
	}
	void clear() {
		lenf = { 0,0 };
		lenb = { 0,0 };
	}
	int size() {
		return lenf.sc - lenf.fs + lenb.sc - lenb.fs;
	}
	T& operator[](int i) {
		if (i < lenf.sc - lenf.fs) {
			return front[lenf.sc - i - 1];
		}
		else {
			return back[lenb.fs + i - (lenf.sc - lenf.fs)];
		}
	}
};