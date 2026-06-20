/*
and/or/xor convolution -> ../katu2ou/convolution

*/

#include<vector>
using namespace std;
void subset_of_subset(int T,int N){
    for(int U=(1<<N)-1;U>=0;--U){
        U&=T;
        //do something
    }
}
void supset_of_subset(int T,int N){
    for(int U=T;U<(1<<N);U=(U+1)|T){
        //...
    }
}
void size_k_subset(int k,int N){
    int x,y;
    for(int T=(1<<k)-1;T<(1<<N);x=T&-T,y=T+x,T=(((T&~y)/x)>>1)|y){
        //...
    }
}
template<typename T>
void fast_zeta_transform(int N,std::vector<T>&A){
    // A[i]=\sum_{j \subseteq i} A[j]
    for(int j = 0; j < N; j++){
        for(int i = 0; i < (1 << N); i++){
            if(i & (1 << j)){
                A[i] += A[i & ~(1 << j)];
            }
        }
    }
}
template<typename T>
void fast_moebius_transform(int N,std::vector<T>&A){
    for(int j = 0; j < N; j++){
        for(int i = 0; i < (1 << N); i++){
            if(i & (1 << j)){
                A[i] -= A[i & ~(1 << j)];
            }
        }
    }
}
