//標準ライブラリのrandom
/*rand()で[0,32767]の整数乱数を生成するが、
    生成の範囲が狭い
    rand()%Nが一様でない
    周期があまり長くない
    乱数生成アルゴリズムが固定
    正規分布など、一様でない乱数生成が面倒
といった問題がある。
(rand()は線形合同法によって乱数を生成している)

    <構造体での使い方>
    RNG rng;

    long long x = rng.random_uniform(1, 6); // 1以上6以下の乱数
    cout << x << endl;

    <他>
    uniform_real_distribution<double>(0.0, 1.0); 
    bernoulli_distribution dist(0.3); p=0.3で1，他0
    normal_distribution<double> dist(0.0, 1.0); 平均0，分散1


*/

//高速で簡易的なrand
unsigned int randInt() {
        static unsigned int tx = 123456789, ty = 362436069, tz = 521288629, tw = 88675123;
        unsigned int tt = (tx ^ (tx << 11));
        tx = ty; ty = tz; tz = tw;
        return (tw = (tw ^ (tw >> 19)) ^ (tt ^ (tt >> 8)));
}

// 構造体として持つ
struct RNG {
    std::mt19937_64 eng;            // 64bit Mersenne Twister

    RNG() : eng(std::random_device{}()) {}   // ここでだけ OS 乱数を消費

    /// [0, n] の一様乱数を返す
    long long uniform(long long n) {
        std::uniform_int_distribution<long long> dist(0, n);
        return dist(eng);
    }
};

