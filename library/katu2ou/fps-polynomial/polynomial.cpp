/*
<多項式ライブラリ>
  - 

  [実装/関数]
      - 
      - 
      - 
  
  [計算時間]
      - 
  
  [備考]
      - 
  
  [参照]
      - chatgpt作

  [verified at]
      - 
      
*/

template<typename T>
class Polynomial {
public:
    vector<T> coeffs;

    Polynomial() {}

    Polynomial(size_t n, const T& init_val = T(0)) : coeffs(n, init_val) {
        trim();
    }
    
    Polynomial(const vector<T>& vec) : coeffs(vec) {
        trim();
    }

    T operator[](size_t idx) const {
        return idx < coeffs.size() ? coeffs[idx] : T(0);
    }
    
    T& operator[](size_t idx) {
        if (idx >= coeffs.size())
            coeffs.resize(idx + 1, T(0));
        return coeffs[idx];
    }

    size_t size() const {
        return coeffs.size();
    }
    
    int degree() const {
        return coeffs.empty() ? 0 : coeffs.size() - 1;
    }

    void trim() {
        while (!coeffs.empty() && coeffs.back() == T(0))
            coeffs.pop_back();
    }

    Polynomial operator+(const Polynomial& other) const {
        Polynomial res;
        size_t n = max(coeffs.size(), other.coeffs.size());
        res.coeffs.resize(n, T(0));
        for (size_t i = 0; i < n; ++i)
            res.coeffs[i] = (*this)[i] + other[i];
        res.trim();
        return res;
    }

    Polynomial operator-(const Polynomial& other) const {
        Polynomial res;
        size_t n = max(coeffs.size(), other.coeffs.size());
        res.coeffs.resize(n, T(0));
        for (size_t i = 0; i < n; ++i)
            res.coeffs[i] = (*this)[i] - other[i];
        res.trim();
        return res;
    }
    
    Polynomial operator*(const Polynomial& other) const {
        Polynomial res;
        res.coeffs.resize(coeffs.size() + other.coeffs.size(), T(0));
        for (size_t i = 0; i < coeffs.size(); ++i) {
            for (size_t j = 0; j < other.coeffs.size(); ++j) {
                res.coeffs[i+j] += coeffs[i] * other.coeffs[j];
            }
        }
        res.trim();
        return res;
    }

    pair<Polynomial, Polynomial> divmod(const Polynomial& divisor) const {
        assert(!divisor.coeffs.empty()); 
        Polynomial a(*this);
        Polynomial quotient;
        quotient.coeffs.resize(max(0, a.degree() - divisor.degree() + 1), T(0));
        
        while (a.degree() >= divisor.degree() && !a.coeffs.empty()) {
            int diff = a.degree() - divisor.degree();
            T factor = a.coeffs.back() / divisor.coeffs.back();
            quotient.coeffs[diff] = factor; 
            for (size_t i = 0; i < divisor.coeffs.size(); ++i) {
                a.coeffs[i + diff] -= divisor.coeffs[i] * factor;
            }
            a.trim();
        }
        quotient.trim();
        return {quotient, a};  // first: 商, second: 余り
    }
    
    Polynomial operator/(const Polynomial& divisor) const {
        return divmod(divisor).first;
    }
    
    Polynomial operator%(const Polynomial& divisor) const {
        return divmod(divisor).second;
    }
    
    void print() const {
        if (coeffs.empty()) {
            cout << "0" << endl;
            return;
        }
        for (int i = 0; i < coeffs.size(); ++i) {
            if (coeffs[i] == T(0)) continue;
            if (i > 0) cout << " + ";
            cout << "(" << coeffs[i] << ")*x^" << i;
        }
        cout << "\n";
    }
};
