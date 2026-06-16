#pragma once
#include<bits/stdc++.h>
using namespace std;
class eratos_sieve {
    int n;
    static constexpr int max_size = 1000010;
    vector<bool>dat;
    vector<int>primes, cnt;
public:
    eratos_sieve() :n(max_size), dat(max_size, true), cnt(max_size) {
        dat[0] = dat[1] = false;
        for (int i = 2;i < n;i++)if (dat[i]) {
            primes.emplace_back(i);
            for (int j = i * 2;j < n;j += i) dat[j] = false;
        }
        for (int i = 1;i < n;i++) cnt[i] = cnt[i - 1] + dat[i];
    }
    bool operator[](const int i)const { return dat[i]; }
    int count(int n) const {
        return cnt[n];
    }
    int count(int l, int r) const {
        return cnt[r] - (l == 0 ? 0 : cnt[l - 1]);
    }
    vector<int>& list() {
        return primes;
    }
};