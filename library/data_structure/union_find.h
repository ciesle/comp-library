class unionfind {
    int size = 0, grps = 0;
    vector<int>pa;
public:
    unionfind() {}
    unionfind(int n) {
        init(n);
    }
    void init(int n) {
        size = n, grps = n;
        pa.assign(n, -1);
    }
    int root(int x) {
        if (pa[x] < 0) return x;
        return pa[x] = root(pa[x]);
    }
    bool unite(int x, int y) {
        x = root(x); y = root(y);
        if (x == y) return false;
        grps--;
        if (pa[x] > pa[y]) swap(x, y);
        pa[x] += pa[y];
        pa[y] = x;
        return true;
    }
    bool same(int x, int y) {
        return root(x) == root(y);
    }
    bool isroot(int x) {
        return x == root(x);
    }
    int sz(int x) {
        return -pa[root(x)];
    }
    int groups() {
        return grps;
    }
    H operator[](int x) {
        x = root(x);
        return H{ x,-pa[x] };
    }
};
