class eratos_sieve {
    int n;
    vector<bool>dat;
    vector<int>primes;
public:
    eratos_sieve() {}
    eratos_sieve(int max_size) :n(max_size) {
        dat.assign(n + 1, true);
        dat[0] = dat[1] = false;
        for (int i = 2;i <= n;i++)if (dat[i]) {
            primes.emplace_back(i);
            for (int j = i * 2;j <= n;j += i) dat[j] = false;
        }
    }
    bool operator[](const int i)const { return dat[i]; }
    int count(int n) const {
        int cnt = 0;
        for (int i = 2;i <= n;i++) if (dat[i]) cnt++;
        return cnt;
    }
    int count(int l, int r) const {
        int cnt = 0;
        for (int i = l;i < r;i++) if (dat[i]) cnt++;
        return cnt;
    }
    vector<int>& list() {
        return primes;
    }
};