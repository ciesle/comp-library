#pragma once
#include<bits/stdc++.h>
#include<immintrin.h>
#include"../math/modpow.h"
#include"../math/modint.h"
#include"../utils/bit.h"
using namespace std;

namespace NumericTheoryTranslation {
	using i256 = __m256i;
	constexpr unsigned long long mod = 998244353ll, proot = 3;
	constexpr unsigned long long mod_ = 998244351ll;
	constexpr unsigned long long r = (1ll << 32) % mod,
		r2 = (0ull - mod) % mod;

	inline uint mod_add(uint a, uint b) {
		uint c = a + b;
		return c < 2 * mod ? c : c - 2 * mod;
	}
	inline uint mod_sub(uint a, uint b) {
		long long c = (long long)a - (long long)b;
		return c < 0 ? uint(c + 2 * mod) : uint(c);
	}
	inline uint reduce(unsigned long long a) {
		return uint((a + (uint(uint(a) * mod_) * mod)) >> 32);
	}
	inline uint normalize(uint a) {
		return a < mod ? a : a - mod;
	}
	inline uint to_mongomery(uint a) {
		return reduce(a * r2);
	}
	inline uint mod_mul(uint a, uint b) {
		return reduce((unsigned long long)a * b);
	}
	i256 mod_add_simd(i256 a, i256 b) {
		i256 c = _mm256_add_epi32(a, b);
		i256 d = _mm256_sub_epi32(c, _mm256_set1_epi32(2 * mod));
		i256 mask = _mm256_cmpgt_epi32(_mm256_set1_epi32(0), d);
		return _mm256_blendv_epi8(d, c, mask);

	}
	i256 mod_sub_simd(i256 a, i256 b) {
		i256 c = _mm256_sub_epi32(a, b);
		i256 d = _mm256_sub_epi32(_mm256_add_epi32(a, _mm256_set1_epi32(2 * mod)), b);
		i256 mask = _mm256_cmpgt_epi32(b, a);
		return _mm256_blendv_epi8(c, d, mask);
	}
	i256 mod_mul_simd(i256 a, i256 b) {
		i256 even = _mm256_mul_epu32(a, b);
		i256 odd = _mm256_mul_epu32(_mm256_shuffle_epi32(a, 0b10110001), _mm256_shuffle_epi32(b, 0b10110001));
		i256 even2 = _mm256_mul_epu32(even, _mm256_set1_epi32((int)mod_));
		i256 odd2 = _mm256_mul_epu32(odd, _mm256_set1_epi32((int)mod_));
		i256 even3 = _mm256_mul_epu32(even2, _mm256_set1_epi32((int)mod));
		i256 odd3 = _mm256_mul_epu32(odd2, _mm256_set1_epi32((int)mod));
		i256 even4 = _mm256_add_epi64(even, even3);
		i256 odd4 = _mm256_add_epi64(odd, odd3);
		return _mm256_blend_epi32(_mm256_shuffle_epi32(even4, 0b10110001), odd4, 0b10101010);
	}

	struct fft_info {
		static constexpr uint rank2 = 23;
		array<uint, max(0u, rank2 + 1)>root;
		array<uint, max(0u, rank2 + 1)>iroot;
		array<uint, max(0u, rank2 - 2 + 1)>rate2;
		array<uint, max(0u, rank2 - 2 + 1)>irate2;
		fft_info() {
			root[rank2] = to_mongomery(modpow(proot, (mod - 1) >> rank2, mod));
			iroot[rank2] = to_mongomery(modpow(modpow(proot, (mod - 1) >> rank2, mod), mod - 2, mod));
			for (int i = rank2 - 1;i >= 0;i--) {
				root[i] = mod_mul(root[i + 1], root[i + 1]);
				iroot[i] = mod_mul(iroot[i + 1], iroot[i + 1]);
			}
			uint prod = to_mongomery(1), iprod = to_mongomery(1);
			for (int i = 0; i <= rank2 - 2; i++) {
				rate2[i] = mod_mul(root[i + 2], iprod);
				irate2[i] = mod_mul(iroot[i + 2], prod);
				prod = mod_mul(prod, root[i + 2]);
				iprod = mod_mul(iprod, iroot[i + 2]);
			}
		}
	};

	void ntt(vector<uint>& a) {
		static const fft_info info;
		int size = a.size();
		int width = size;
		while (width > 1) {
			int offset = width >> 1;
			if (offset < 8) {
				uint root = to_mongomery(1);
				for (int i = 0, block = 0; i < size; i += width, block++) {
					for (int j = i;j < i + offset;j++) {
						uint c0 = a[j];
						uint c1 = mod_mul(a[j + offset], root);
						a[j] = mod_add(c0, c1);
						a[j + offset] = mod_sub(c0, c1);
					}
					root = mod_mul(root, info.rate2[cto(block)]);
				}
			}
			else {
				i256 root = _mm256_set1_epi32(to_mongomery(1));
				for (int i = 0, block = 0; i < size; i += width, block++) {
					if (block == 0) {
						for (int j = i;j < i + offset;j += 8) {
							i256 c0 = _mm256_loadu_si256(reinterpret_cast<i256*>(&a[j]));
							i256 c1 = _mm256_loadu_si256(reinterpret_cast<i256*>(&a[j + offset]));
							_mm256_storeu_si256(reinterpret_cast<i256*>(&a[j]), mod_add_simd(c0, c1));
							_mm256_storeu_si256(reinterpret_cast<i256*>(&a[j + offset]), mod_sub_simd(c0, c1));
						}
					}
					else {
						for (int j = i;j < i + offset;j += 8) {
							i256 c0 = _mm256_loadu_si256(reinterpret_cast<i256*>(&a[j]));
							i256 c1 = mod_mul_simd(_mm256_loadu_si256(reinterpret_cast<i256*>(&a[j + offset])), root);
							_mm256_storeu_si256(reinterpret_cast<i256*>(&a[j]), mod_add_simd(c0, c1));
							_mm256_storeu_si256(reinterpret_cast<i256*>(&a[j + offset]), mod_sub_simd(c0, c1));
						}
					}
					root = mod_mul_simd(root, _mm256_set1_epi32(info.rate2[cto(block)]));
				}
			}
			width >>= 1;
		}
	}
	void intt(vector<uint>& a) {
		static const fft_info info;
		int size = a.size();
		int width = 2;
		while (width <= size) {
			int offset = width >> 1;
			if (offset < 8) {
				uint root = to_mongomery(1);
				for (int i = 0, block = 0; i < size; i += width, block++) {
					for (int j = i;j < i + offset;j++) {
						uint c0 = a[j];
						uint c1 = a[j + offset];
						a[j] = mod_add(c0, c1);
						a[j + offset] = mod_mul(mod_sub(c0, c1), root);
					}
					root = mod_mul(root, info.irate2[cto(block)]);
				}
			}
			else {
				i256 root = _mm256_set1_epi32(to_mongomery(1));
				for (int i = 0, block = 0; i < size; i += width, block++) {
					if (block == 0) {
						for (int j = i;j < i + offset;j += 8) {
							i256 c0 = _mm256_loadu_si256(reinterpret_cast<i256*>(&a[j]));
							i256 c1 = _mm256_loadu_si256(reinterpret_cast<i256*>(&a[j + offset]));
							_mm256_storeu_si256(reinterpret_cast<i256*>(&a[j]), mod_add_simd(c0, c1));
							_mm256_storeu_si256(reinterpret_cast<i256*>(&a[j + offset]), mod_sub_simd(c0, c1));
						}
					}
					else {
						for (int j = i;j < i + offset;j += 8) {
							i256 c0 = _mm256_loadu_si256(reinterpret_cast<i256*>(&a[j]));
							i256 c1 = _mm256_loadu_si256(reinterpret_cast<i256*>(&a[j + offset]));
							_mm256_storeu_si256(reinterpret_cast<i256*>(&a[j]), mod_add_simd(c0, c1));
							_mm256_storeu_si256(reinterpret_cast<i256*>(&a[j + offset]), mod_mul_simd(mod_sub_simd(c0, c1), root));
						}
					}
					root = mod_mul_simd(root, _mm256_set1_epi32(info.irate2[cto(block)]));
				}
			}
			width <<= 1;
		}
		uint iv = to_mongomery(modpow(size, mod - 2, mod));
		for (int i = 0; i < size; i++) {
			a[i] = mod_mul(a[i], iv);
		}
	}
};
vector<mint>convolution(vector<mint>a, vector<mint>b) {
	using namespace NumericTheoryTranslation;
	int n = a.size(), m = b.size();
	int l = 1;
	while ((1 << l) < n + m - 1) l++;
	vector<uint>a2(1 << l), b2(1 << l);
	for (int i = 0;i < n;i++) a2[i] = to_mongomery(a[i].v);
	for (int i = 0;i < m;i++) b2[i] = to_mongomery(b[i].v);
	ntt(a2);
	ntt(b2);

	for (int i = 0;i < (1 << l);i++) a2[i] = mod_mul(a2[i], b2[i]);
	intt(a2);
	vector<mint>ret(n + m - 1);
	for (int i = 0;i < n + m - 1;i++) ret[i] = normalize(reduce(a2[i]));

	return ret;
}