#pragma once
template<typename T>
struct Mono;
namespace SegmentTree {
	template<typename T>
	class LazySegmentTree {
	protected:
		int n, rr, height;
		vector<Mono<T>>dat;
		LazySegmentTree() {}
		LazySegmentTree(int n) :n(n) {
			rr = height = 1;
			while (rr < n) rr <<= 1, height++;
			dat.assign(rr * 2, Mono<T>());
			for (int i = 0;i < height;i++) {
				for (int j = 0;j < (1 << i);j++) {
					dat[(1 << i) + j].l = j * ((1 << (height - i - 1)));
					dat[(1 << i) + j].r = (j + 1) * ((1 << (height - i - 1)));
					dat[(1 << i) + j].len = 1 << (height - i - 1);
				}
			}
		}
		LazySegmentTree(LazySegmentTree&& other) :n(other.n), rr(other.rr), height(other.height), dat(move(other.dat)) {}
		LazySegmentTree& operator=(LazySegmentTree&& other) noexcept {
			if (this != &other) n = other.n, rr = other.rr, height = other.height, dat = move(other.dat);
			return *this;
		}
		virtual void update_one(Mono<T>& pos, T& x) = 0;
		virtual T eval(T& l, T& r) = 0;
		virtual void fold(Mono<T>& pos) = 0;
		virtual void propagate(Mono<T>& par, Mono<T>& child) = 0;
		virtual bool judge(T pos, T x) = 0;
	private:
		void update_all(int i) { propagate(dat[i], dat[i * 2]), propagate(dat[i], dat[i * 2 + 1]), fold(dat[i]); }
	public:
		//change dat[pos]->x
		void change(int pos, T x) {
			pos += rr;
			for (int i = height - 1;i > 0;i--) update_all(pos >> i);
			dat[pos].val = x;
			while (pos >>= 1, pos) {
				dat[pos].val = eval(dat[pos * 2].val, dat[pos * 2 + 1].val);
			}
		}
		void update(int l, int r, T x) {
			int a = l + rr, b = r + rr - 1;
			for (int i = height - 1;i > 0;i--) update_all(a >> i), update_all(b >> i);
			b += 1;
			while (a < b) {
				if (a & 1) {
					update_one(dat[a], x);
					if (a < rr) update_all(a);
					a++;
				}
				if (b & 1) {
					b--;
					update_one(dat[b], x);
					if (b < rr) update_all(b);
				}
				a >>= 1, b >>= 1;
			}
			a = l + rr, b = r + rr - 1;
			while (a >>= 1, b >>= 1, (a > 0 && b > 0)) {
				update_all(a), update_all(b);
				dat[a].val = eval(dat[a * 2].val, dat[a * 2 + 1].val);
				dat[b].val = eval(dat[b * 2].val, dat[b * 2 + 1].val);
			}
		}
		T query(int l, int r) {
			int a = l + rr, b = r + rr - 1;
			for (int i = height - 1;i > 0;i--) update_all(a >> i), update_all(b >> i);
			b += 1;
			T left = Mono<T>().val, right = Mono<T>().val;
			while (a < b) {
				if (a & 1) left = eval(left, dat[a++].val);
				if (b & 1) right = eval(dat[--b].val, right);
				a >>= 1, b >>= 1;
			}
			return eval(left, right);
		}
		//judge(l,ret-1)==false, judge(l,ret)==true. 
		//if judge(l,r)==false, ret=-1
		int lower_bound(int l, int r, T x) {
			if (l >= r || l >= n || l < 0) return -1;
			int a = l + rr, b = r + rr - 1;
			for (int i = height - 1;i > 0;i--) update_all(a >> i), update_all(b >> i);
			T cum = Mono<T>().val;
			int cur = l, pos = l + rr;
			do {
				while (!(pos & 1) && (pos >> 1) && dat[pos >> 1].r <= r) pos >>= 1;
				if (dat[pos].r <= r && !judge(eval(cum, dat[pos].val), x)) {
					cum = eval(cum, dat[pos].val);
					cur = dat[pos].r;
					pos++;
				}
				else break;
			} while (cur < r && pos < 2 * rr);
			while (cur < r && pos < rr) {
				update_all(pos);
				if (r < dat[pos * 2].r || judge(eval(cum, dat[pos * 2].val), x)) pos <<= 1;
				else cum = eval(cum, dat[pos * 2].val), cur = dat[pos * 2].r, pos = pos * 2 + 1;
			}
			if (cur == r) return -1;
			return cur + 1;
		}
	};
	//put function classes here.

};