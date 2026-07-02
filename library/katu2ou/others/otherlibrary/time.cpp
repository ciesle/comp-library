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

