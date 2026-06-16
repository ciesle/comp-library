//if about ret>max/2 return max/2
template<typename T>
T powi(T n, ll k) {
    T ret = 1;
    for (int i = 0;i < 64;i++) {
        if ((k >> i) & 1) {
            if (ret >= numeric_limits<T>::max() / 2 / n) return numeric_limits<T>::max() / 2;
            ret *= n;
        }
        if ((k >> (i + 1)) == 0) break;
        if (n >= numeric_limits<T>::max() / 2 / n) return numeric_limits<T>::max() / 2;
        n *= n;
    }
    return ret;
}