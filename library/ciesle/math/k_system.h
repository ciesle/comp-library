//nをk進法の値に直す
//6 -> 0,1,1
template<typename T>
vector<T> k_system(T n, T k) {
    vector<T>ret;
    if (k <= 1) return ret;
    while (n > 0) {
        ret.emplace_back(n % k);
        n /= k;
    }
    return ret;
}