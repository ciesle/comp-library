#pragma once
#include<bits/stdc++.h>
using namespace std;

template<typename T>
struct matrix {
	int h, w;
	vector<vector<T>>dat;
	matrix() {}
	matrix(int h, int w, bool identity = false) :h(h), w(w), dat(h, vector<T>(w, T())) {
		if (identity) for (int i = 0;i < h;i++) dat[i][i] = 1;
	}
	matrix<T> operator+(matrix<T>& a) const {
		matrix<T> b(h, w);
		for (int i = 0;i < h;i++)for (int j = 0;j < w;j++) {
			b[i][j] = dat[i][j] + a[i][j];
		}
		return b;
	}
	matrix<T> operator-(matrix<T>& a) const {
		matrix<T> b(h, w);
		for (int i = 0;i < h;i++)for (int j = 0;j < w;j++) {
			b[i][j] = dat[i][j] - a[i][j];
		}
		return b;
	}
	matrix<T> operator*(matrix<T>& a) const {
		matrix<T> b(h, a.w);
		for (int i = 0;i < h;i++)for (int j = 0;j < a.w;j++)
			for (int k = 0;k < w;k++) {
				b[i][j] += dat[i][k] * a.dat[k][j];
			}
		return b;
	}
	matrix<T>pow(long long t) {
		matrix<T>b = *this, c(h, w, true);
		while (t > 0) {
			if (t & 1) c = c * b;
			b = b * b;
			t >>= 1;
		}
		return c;
	}
	T det() {
		matrix<T>b(*this);
		assert(w == h);
		T ret = 1;
		for (int i = 0;i < w;i++) {
			int idx = -1;
			for (int j = i;j < h;j++) if (b[j][i] != 0) idx = j;
			if (idx == -1) return 0;
			if (i != idx) {
				ret *= -1;
				swap(b[i], b[idx]);
			}
			ret *= b[i][i];
			T vv = b[i][i];
			for (int j = 0;j < w;j++) b[i][j] /= vv;
			for (int j = i + 1;j < h;j++) {
				T a = b[j][i];
				for (int k = 0;k < w;k++) b[j][k] -= b[i][k] * a;
			}
		}
		return ret;
	}
	vector<T>& operator[](const int i) {
		return dat[i];
	}
	void print() {
		for (int i = 0;i < h;i++)
			for (int j = 0;j < w;j++)
				cout << dat[i][j] << " \n"[j == w - 1];
	}

	//not verified
	//is_extended: trueなら、最後の列は Ax=b の b として扱う
	int sweep(bool is_extended = false) {
		int rank = 0;
		for (int col = 0;col < w;col++) {
			if (is_extended && col == w - 1) break;
			int pivot = -1;
			for (int row = rank;row < h;row++) {
				if (dat[row][col] != 0) {
					pivot = row;
					break;
				}
			}
			if (pivot == -1) continue;
			swap(dat[pivot], dat[rank]);
			for (int col2 = 0;col2 < w;col2++) dat[rank][col2] /= dat[rank][col];
			for (int row = 0;row < h;row++) {
				if (row != rank && dat[row][col]) {
					T tmp = dat[row][col];
					for (int col2 = col; col2 < w; col2++) {
						dat[row][col2] -= dat[rank][col2] * tmp;
					}
				}
			}
			rank++;
		}
		return rank;
	}
};


struct bitmatrix {
	int h, w;
	static const size_t max_row = 510, max_col = 510;
	bitset<max_col>dat[max_row];

	bitmatrix() {}
	bitmatrix(int h, int w, bool identity = false) :h(h), w(w) {
		if (identity) for (int i = 0;i < h;i++) dat[i][i] = 1;
	}
	bitmatrix operator+(bitmatrix& a) const {
		bitmatrix b(h, w);
		for (int i = 0;i < h;i++) b[i] = dat[i] ^ a[i];
		return b;
	}
	bitmatrix operator*(bitmatrix& a) const {
		bitmatrix b(h, a.w);
		for (int i = 0;i < h;i++)for (int j = 0;j < a.w;j++)
			for (int k = 0;k < w;k++) {
				b[i][j] = b[i][j] ^ (dat[i][k] & a.dat[k][j]);
			}
		return b;
	}
	bitmatrix pow(long long t) {
		bitmatrix b = *this, c(h, w, true);
		while (t > 0) {
			if (t & 1) c = c * b;
			b = b * b;
			t >>= 1;
		}
		return c;
	}
	bitset<max_col>& operator[](const int i) {
		return dat[i];
	}
	void print() {
		for (int i = 0;i < h;i++)
			for (int j = 0;j < w;j++)
				cout << (dat[i][j] ? 1 : 0) << " \n"[j == w - 1];
	}

	int sweep(bool is_extended = false) {
		int rank = 0;
		for (int col = 0;col < w;col++) {
			if (is_extended && col == w - 1) break;
			int pivot = -1;
			for (int row = rank;row < h;row++) {
				if (dat[row][col]) {
					pivot = row;
					break;
				}
			}
			if (pivot == -1) continue;
			swap(dat[pivot], dat[rank]);
			for (int row = 0;row < h;row++) {
				if (row != rank && dat[row][col]) dat[row] ^= dat[rank];
			}
			rank++;
		}
		return rank;
	}
};

vector<bitset<bitmatrix::max_col>> kernel_basis(bitmatrix A) {
	A.sweep();

	int h = A.h, w = A.w;
	vector<int> pivot_col;
	vector<int> free_col;

	int row = 0;
	for (int col = 0; col < w; col++) {
		if (row >= h) {
			free_col.push_back(col);
			continue;
		}
		if (A[row][col]) {
			pivot_col.push_back(col);
			row++;
		}
		else {
			free_col.push_back(col);
		}
	}

	vector<bitset<bitmatrix::max_col>> basis;
	for (int free : free_col) {
		bitset<bitmatrix::max_col> v;
		v.reset();
		v[free] = 1;

		for (int i = 0; i < (int)pivot_col.size(); i++) {
			int pc = pivot_col[i];
			if (A[i][free]) v[pc] = 1;
		}
		basis.push_back(v);
	}
	return basis;
}