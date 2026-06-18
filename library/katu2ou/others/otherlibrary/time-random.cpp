/* 時間測定 */
// time(簡易版)
    double time=0.0;
        clock_t start = clock();
        clock_t end = clock();
    time= static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000.0;
    cout<<(double)(end - start) / CLOCKS_PER_SEC << endl;

// Chronoを用いた方法
    auto start = chrono::steady_clock::now(); //時間計測の開始
    auto now = std::chrono::steady_clock::now(); //現在時刻と開始時刻の差を測定
    double elapsed = std::chrono::duration<double>(now - start).count(); //時間をdouble型で取得


/* 乱数生成 */
// mt19937(メルカトルツイスター)を用いる方法
    random_device rd;
    mt19937 gen(rd()); // genという名前の
    uniform_int_distribution<> dist1(1, 100); //1以上100以下の整数から一様ランダム
    uniform_real_distribution<> dist2(0, 1); //0以上1以下のdouble値から一様ランダム
        //<long double> などともできる
    normal_distribution<> dist3(0, 1); //平均0分散1の正規分布から取得
    // ここまでのコードは，乱数を使う前に1度だけ呼び出しておく
    int random_value = dist1(gen);
    
// xor shift (0 ~ 2^32-1)
unsigned int randInt() {
    static unsigned int tx = 123456789, ty = 362436069, tz = 521288629, tw = 88675123;
    unsigned int tt = (tx ^ (tx << 11));
    tx = ty; ty = tz; tz = tw;
    return (tw = (tw ^ (tw >> 19)) ^ (tt ^ (tt >> 8)));
}