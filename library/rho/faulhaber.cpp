/*
Require: modint998244353, FPS.inv()
Input: k
Ensure: output coefficients of P(x) such that P(N)=\sum_{i=0}^N i^k
*/

typedef modint998244353 mint;
constexpr int fact_size=2500000;
mint kj[fact_size+1],kji[fact_size+1];
mint binom(int r,int c){
    if(r<c)return 0;
    return kj[r]*kji[c]*kji[r-c];
}
void set_fact(){
    kj[0]=1;
    for(int i=1;i<=fact_size;i++)kj[i]=kj[i-1]*i;
    kji[fact_size]=1/kj[fact_size];
    for(int i=fact_size;i>=1;i--)kji[i-1]=kji[i]*i;
}
vector<mint>bernoulli(int k){
    FPS<mint>coeff(k+2);
    for(int i=0;i<=k+1;i++)coeff[i]=kji[i+1];
    coeff=coeff.inv(k+2);
    rep(i,k+1)coeff[i]*=kj[i];
    vector<mint>ans(k+2);
    for(int j=0;j<=k;j++){
        ans[k+1-j]=coeff[j]*binom(k+1,j)/mint(k+1);
    }
    return ans;
}