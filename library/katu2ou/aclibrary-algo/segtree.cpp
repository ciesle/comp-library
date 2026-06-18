/*
  <lazy segtree>
    - モノイド(S,・)に対して使える

    [実装/関数]
        - segtree<S, op, e> seg(int n)
        - egtree<S, op, e> seg(vector<T> v)
            - モノイドの型 S
            - S,S->S を計算する関数 S op(S a, S b)
            - eを返す関数 S e()

        - void seg.set(int p, int x) : a[p]=xをする (O(logN))
        - S seg.get(int p) : a[p]を返す (O(logN))
        - S seg.prod(int l, int r) : op(a[l],...,a[r-1])を返す l=rのときはe()を返す
        - S seg.all_prod() : op(a[0],...,a[n-1])を計算する O(1)

        - int seg.max_right<g>(int l) : 関数bool g(S x) を定義して渡す
        - int seg.max_right<G>(int l, G g) : Sを引数に取りboolを返す関数オブジェクトを渡す
            - 次の条件を両方満たすrをいずれか一つ返す
                - r = l もしくはg(op(a[l] , ..., a[r-1])) = true
                - r = n もしくはg(op(a[l], ..., a[r])) = false
            - gが単調とすると，g(op(a[l], ..., a[r-1])) = trueとなる最大のrを返すといえる．
            (true,true,....,true,false,false,...)
                - g(e()) = true が制約
        
        - int seg.min_left<g>(int r)
        - int seg.min_left<G>(int r, G g)
            - 次の条件を両方満たすlをいずれか一つ返す
                - l = r もしくはg(op(a[l], a[l + 1], ..., a[r - 1])) = true
                - l = 0 もしくは g(op(a[l - 1], a[l], ..., a[r - 1])) = false
            - gが単調だとすれば、g(op(a[l], a[l + 1], ..., a[r - 1])) = true となる
            最小のlを返すといえる．
            (false,false,...,false,true,true,...)
                - g(e()) = true が制約
                
    [計算時間]
        - 
    
    [備考]
        - 
    
    [参照]
        - 

    [verified at]
        - 
    
    [使用例] 
        
*/

namespace internal {

// @param n `0 <= n`
// @return minimum non-negative `x` s.t. `n <= 2**x`
int ceil_pow2(int n) {
    int x = 0;
    while ((1U << x) < (unsigned int)(n)) x++;
    return x;
}

// @param n `1 <= n`
// @return minimum non-negative `x` s.t. `(n & (1 << x)) != 0`
int bsf(unsigned int n) {
#ifdef _MSC_VER
    unsigned long index;
    _BitScanForward(&index, n);
    return index;
#else
    return __builtin_ctz(n);
#endif
}

}  // namespace internal


template <class S, S (*op)(S, S), S (*e)()> struct segtree {
  public:
    segtree() : segtree(0) {}
    segtree(int n) : segtree(std::vector<S>(n, e())) {}
    segtree(const std::vector<S>& v) : _n(int(v.size())) {
        log = internal::ceil_pow2(_n);
        size = 1 << log;
        d = std::vector<S>(2 * size, e());
        for (int i = 0; i < _n; i++) d[size + i] = v[i];
        for (int i = size - 1; i >= 1; i--) {
            update(i);
        }
    }

    void set(int p, S x) {
        assert(0 <= p && p < _n);
        p += size;
        d[p] = x;
        for (int i = 1; i <= log; i++) update(p >> i);
    }

    S get(int p) {
        assert(0 <= p && p < _n);
        return d[p + size];
    }

    S prod(int l, int r) {
        assert(0 <= l && l <= r && r <= _n);
        S sml = e(), smr = e();
        l += size;
        r += size;

        while (l < r) {
            if (l & 1) sml = op(sml, d[l++]);
            if (r & 1) smr = op(d[--r], smr);
            l >>= 1;
            r >>= 1;
        }
        return op(sml, smr);
    }

    S all_prod() { return d[1]; }

    template <bool (*f)(S)> int max_right(int l) {
        return max_right(l, [](S x) { return f(x); });
    }
    template <class F> int max_right(int l, F f) {
        assert(0 <= l && l <= _n);
        assert(f(e()));
        if (l == _n) return _n;
        l += size;
        S sm = e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!f(op(sm, d[l]))) {
                while (l < size) {
                    l = (2 * l);
                    if (f(op(sm, d[l]))) {
                        sm = op(sm, d[l]);
                        l++;
                    }
                }
                return l - size;
            }
            sm = op(sm, d[l]);
            l++;
        } while ((l & -l) != l);
        return _n;
    }

    template <bool (*f)(S)> int min_left(int r) {
        return min_left(r, [](S x) { return f(x); });
    }
    template <class F> int min_left(int r, F f) {
        assert(0 <= r && r <= _n);
        assert(f(e()));
        if (r == 0) return 0;
        r += size;
        S sm = e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!f(op(d[r], sm))) {
                while (r < size) {
                    r = (2 * r + 1);
                    if (f(op(d[r], sm))) {
                        sm = op(d[r], sm);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            sm = op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }

  private:
    int _n, size, log;
    std::vector<S> d;

    void update(int k) { d[k] = op(d[2 * k], d[2 * k + 1]); }
};
