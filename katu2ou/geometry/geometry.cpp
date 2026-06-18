/*
  <幾何ライブラリ>
    - 

    [実装/関数]
        - Point = complex<long double> で点を扱う (x,y軸は .real, .imagで取得できる)
        - long double型の値の一致判定は equal(long double a, long double b) で行う
        - Line(Point a, Point b) : 二点a,bを結ぶ線分(直線)
            - Line(ld a, ld b, ld c) : ax+by=cを表す直線(端点はルールに従って決める)
        - Circle(Point o, ld r) : 中心が点oで半径rの円
        - 

        - Point unitVector(Point a) : aの単位ベクトルを返す
        - Point normalVector(Point a) : aを左に90度回転した点を返す
        - ld dot(Point a, Point b) : aとbの内積を返す
        - ld cross(Point a, Point b) : aとbの外積を返す
        - Point rotate(Point a, ld theta) : aをthetaだけ左回転した点を返す
        - ld radianToDegree(ld a) : ラジアンaを度数に変換
        - ld degreeToRadian(ld a) : 度数aをラジアンに変換
        - Point projection(Line l, Point p) : 点pを直線lに射影した点を求める
        - Point reflection(Line l, Point p) : 線分lに対してpと対称な点を求める
        - int ccw(Point a, Point b, Point c) 
            - aからみてb,cがこの順で反時計回りなら1
            - aからみてb,cがこの順で時計回りなら-1
            - c,a,bがこの順で同一直線上にあるなら2
            - a,b,cがこの順で同一直線上にあるなら-2
            - cが線分a,b上にあるなら0
        - bool isOrthogonal(Line l1, Line l2)
        - bool isParallel(Line l1, Line l2)
        - Point crossPoint(Line l1, line l2) : 直線としての交点
        - ld distanceBetweenSegmentAndPoint(Line l1, Point p) : 
            点pから「線分」l1への距離
        - ld distanceBetweenLineAndPoint(Line l1, Point p) : 
            点pから「直線」l1への距離
        - ld distanceBetweenSegments(Line l1, Line l2) : 
            「線分」l1と「線分」l2の距離
        - ld PolygonArea(vector<Point> p) : 
            vectorで表される多角形が
        - bool isConvex(vector<Point> p) : 
            vectorで表される多角形
        - bool isContained (vector<Point> p, Point g) : 
            多角形pに点gが含まれているなら2
            辺上にあるなら1 (格子点だと判定が不正確なはずなので整数用を使う方が良さそう)
            外側にあるなら0
        - vector<Point> ConvexHull(vector<Point>) : 
            凸包に含まれる点集合を返す (O(nlogn))
        - int isIntersect(Circle c1, Circle c2) : 
            円c1と円c2の共通接線の個数を返す
        - Circle inCircle(Point a, Point b, Point c) : 
            3点a,b,cがなす三角形の内接円を返す
        - vector<Point> crossPoint(Circle c1, Line l) : 
            円c1と直線lの交点を列挙
        - vector<Point> crossPoint(Circle c1, Circle c2) : 
            円c1と円c2の交点を列挙
        - vector<Line> tangent(Circle c1, Circle c2) : 
            円c1,円c2の共通接線を列挙
    
    [計算時間]
        - 
    
    [備考]
        - 
    
    [参照]
        - https://bakamono1357.hatenablog.com/entry/2020/04/29/025320

    [verified at]
        - https://onlinejudge.u-aizu.ac.jp/courses/library/4/CGL/all
    
    [使用例]
        
        
*/

using Point = complex<long double>;
typedef Point pt;
const long double EPS = 1e-10;
const long double PI =3.1415926535;

inline bool equal(const long double &a, const long double &b) {
    return fabs(a - b) < EPS;
}

// 各種関数
// Point a=Point(1,2);
// a.real() real(a) //実部
// a.imag() imag(a) //虚部
// a.real(2) // aの実部を2に変更
// abs(a) //絶対値
// arg(a) //偏角
// norm(a) //複素数体のノルム
// conj(a) //共役複素数
// proj(a) //リーマン球面への射影を得る
// polar(a) //複素数を極形式で指定して作る

// 単位ベクトル(unit vector)を求める
Point unitVector(const Point &a) { return a / abs(a); }

// 法線ベクトル(normal vector)を求める
// 90度回転した単位ベクトルをかける
// -90度がよければPoint(0, -1)をかける
Point normalVector(const Point &a) { return a * Point(0, 1); }

// 内積(dot product) : a・b = |a||b|cosΘ
long double dot(const Point &a, const Point &b) {
    return (a.real() * b.real() + a.imag() * b.imag());
}

// 外積(cross product) : a×b = |a||b|sinΘ
long double cross(const Point &a, const Point &b) {
    return (a.real() * b.imag() - a.imag() * b.real());
}

// 点pを反時計回りにtheta度回転
Point rotate(const Point &p, const long double &theta) {
    return Point(cos(theta) * p.real() - sin(theta) * p.imag(),
                 sin(theta) * p.real() + cos(theta) * p.imag());
}    

// ラジアン->度
long double radianToDegree(const long double &radian) { return radian * 180.0 / PI; }

// 度->ラジアン
long double degreeToRadian(const long double &degree) { return degree * PI / 180.0; }

// Line : 直線を表す構造体
// b - a で直線・線分を表せる
struct Line {
    Point a, b;
    Line() = default;
    Line(Point a, Point b) : a(a), b(b) {}
    // Ax+By=C
    Line(long double A, long double B, long double C) {
        if(equal(A, 0)) {
            a = Point(0, C / B), b = Point(1, C / B);
        } else if(equal(B, 0)) {
            b = Point(C / A, 0), b = Point(C / A, 1);
        } else {
            a = Point(0, C / B), b = Point(C / A, 0);
        }
    }
};

// Circle : 円を表す構造体
// pが中心の位置ベクトル、rは半径
struct Circle {
    Point p;
    long double r;

    Circle() = default;

    Circle(Point p, long double r) : p(p), r(r) {}
};

// 射影(projection)
// 直線(線分)lに点pから引いた垂線の足を求める
Point projection(const Line &l, const Point &p) {
    long double t = dot(p - l.a, l.a - l.b) / norm(l.a - l.b);
    return l.a + (l.a - l.b) * t;
}

// 反射(reflection)
// 直線lを対称軸として点pと線対称の位置にある点を求める
Point reflection(const Line &l, const Point &p) {
    return p + (projection(l, p) - p) * (long double)2.0;
}

// 点の回転方向
// 点a, b, cの位置関係について(aが基準点)
int ccw(const Point &a, Point b, Point c) {
    b -= a, c -= a;
    // 点a, b, c が
    // 反時計回りの時、
    if(cross(b, c) > EPS) {
        return 1;
    }
    // 時計回りの時、
    if(cross(b, c) < -EPS) {
        return -1;
    }
    // c, a, bがこの順番で同一直線上にある時、
    if(dot(b, c) < 0) {
        return 2;
    }
    // a, b, cがこの順番で同一直線上にある場合、
    if(norm(b) < norm(c)) {
        return -2;
    }
    // cが線分ab上にある場合、
    return 0;
}

// 2直線の直交判定 : a⊥b <=> dot(a, b) = 0
bool isOrthogonal(const Line &a, const Line &b) {
    return equal(dot(a.b - a.a, b.b - b.a), 0);
}
// 2直線の平行判定 : a//b <=> cross(a, b) = 0
bool isParallel(const Line &a, const Line &b) {
    return equal(cross(a.b - a.a, b.b - b.a), 0);
}

// 線分sと線分tが交差しているかどうか
bool isIntersect(const Line &s, const Line &t) {
    return ccw(s.a, s.b, t.a) * ccw(s.a, s.b, t.b) <= 0 &&
           ccw(t.a, t.b, s.a) * ccw(t.a, t.b, s.b) <= 0;
}

// 直線s, tの交点の計算
Point crossPoint(const Line &s, const Line &t) {
    long double d1 = cross(s.b - s.a, t.b - t.a);
    long double d2 = cross(s.b - s.a, s.b - t.a);
    if(equal(abs(d1), 0) && equal(abs(d2), 0)) {
        return t.a;
    }
    return t.a + (t.b - t.a) * (d2 / d1);
}

// 線分lと点pの距離を求める
// 定義：点pから「線分lのどこか」への最短距離
long double distanceBetweenSegmentAndPoint(const Line &l, const Point &p) {
    if(dot(l.b - l.a, p - l.a) < EPS) {
        return abs(p - l.a);
    }
    if(dot(l.a - l.b, p - l.b) < EPS) {
        return abs(p - l.b);
    }
    return abs(cross(l.b - l.a, p - l.a)) / abs(l.b - l.a);
}

long double distanceBetweenLineAndPoint(const Line &l, const Point &p) {
    return abs(cross(l.b - l.a, p - l.a)) / abs(l.b - l.a);
}

// 線分sとtの距離
long double distanceBetweenSegments(const Line &s, const Line &t) {
    if(isIntersect(s, t)) {
        return (long double)(0);
    }
    long double ans = distanceBetweenSegmentAndPoint(s, t.a);
    chmin(ans, distanceBetweenSegmentAndPoint(s, t.b));
    chmin(ans, distanceBetweenSegmentAndPoint(t, s.a));
    chmin(ans, distanceBetweenSegmentAndPoint(t, s.b));
    return ans;
}

// 多角形の面積を求める
long double PolygonArea(const vector<Point> &p) {
    long double res = 0;
    int n = p.size();
    for(int i = 0; i < n - 1; i++) {
        res += cross(p[i], p[i + 1]);
    }
    res += cross(p[n - 1], p[0]);
    return res * 0.5;
}

// 凸多角形かどうか
bool isConvex(const vector<Point> &p) {
    int n = p.size();
    int now, pre, nxt;
    for(int i = 0; i < n; i++) {
        pre = (i - 1 + n) % n;
        nxt = (i + 1) % n;
        now = i;
        if(ccw(p[pre], p[now], p[nxt]) == -1) {
            return false;
        }
    }
    return true;
}

// 多角形gに点pが含まれているか?
// 含まれる:2, 辺上にある:1, 含まれない:0
int isContained(const vector<Point> &g, const Point &p) {
    bool in = false;
    int n = (int)g.size();
    for(int i = 0; i < n; i++) {
        Point a = g[i] - p, b = g[(i + 1) % n] - p;
        if(imag(a) > imag(b)) {
            swap(a, b);
        }
        if(imag(a) <= EPS && EPS < imag(b) && cross(a, b) < -EPS) {
            in = !in;
        }
        if(cross(a, b) == 0 && dot(a, b) <= 0) {
            return 1;
        }
    }
    return (in ? 2 : 0);
}

// 凸包 O(NlogN)
vector<Point> ConvexHull(vector<Point> &p) {
    int n = (int)p.size(), k = 0;
    sort(all(p), [](const Point &a, const Point &b) {
        return (real(a) != real(b) ? real(a) < real(b) : imag(a) < imag(b));
    });
    vector<Point> ch(2 * n);
    // 一直線上の3点を含める -> (< -EPS)
    // 含めない -> (< EPS)
    for(int i = 0; i < n; ch[k++] = p[i++]) { // lower
        while(k >= 2 && cross(ch[k - 1] - ch[k - 2], p[i] - ch[k - 1]) < EPS)
            --k;
    }
    for(int i = n - 2, t = k + 1; i >= 0; ch[k++] = p[i--]) { // upper
        while(k >= t && cross(ch[k - 1] - ch[k - 2], p[i] - ch[k - 1]) < EPS)
            --k;
    }
    ch.resize(k - 1);
    return ch;
}

// 2つの円の交差判定
// 返り値は共通接線の数
int isIntersect(const Circle &c1, const Circle &c2) {
    long double d = abs(c1.p - c2.p);
    // 2つの円が離れている場合
    if(d > c1.r + c2.r + EPS) {
        return 4;
    }
    // 外接している場合
    if(equal(d, c1.r + c2.r)) {
        return 3;
    }
    // 内接している場合
    if(equal(d, abs(c1.r - c2.r))) {
        return 1;
    }
    // 内包している場合
    if(d < abs(c1.r - c2.r) - EPS) {
        return 0;
    }
    return 2;
}
 
// 三角形の内接円
Circle inCircle(const Point &a, const Point &b, const Point &c) {
     long double A = abs(b - c), B = abs(a - c), C = abs(a - b);
     Point p(A * real(a) + B * real(b) + C * real(c),
                 A * imag(a) + B * imag(b) + C * imag(c));
     p /= (A + B + C);
     long double r = distanceBetweenLineAndPoint(Line(a, b), p);
     return Circle(p, r);
}

// 円cと直線lの交点
vector<Point> crossPoint(const Circle &c, const Line &l) {
    vector<Point> res;
    long double d = distanceBetweenLineAndPoint(l, c.p);
    // 交点を持たない
    if(d > c.r + EPS) {
        return res;
    }
    // 接する
    Point h = projection(l, c.p);
    if(equal(d, c.r)) {
        res.emplace_back(h);
        return res;
    }
    Point e = unitVector(l.b - l.a);
    long double ph = sqrt(c.r * c.r - d * d);
    res.emplace_back(h - e * ph);
    res.emplace_back(h + e * ph);
    return res;
}

// 2つの円の交点
vector<Point> crossPoint(const Circle &c1, const Circle &c2) {
    vector<Point> res;
    int mode = isIntersect(c1, c2);
    // 2つの中心の距離
    long double d = abs(c1.p - c2.p);
    // 2円が離れている場合
    if(mode == 4) {
        return res;
    }
    // 1つの円がもう1つの円に内包されている場合
    if(mode == 0) {
        return res;
    }
    // 2円が外接する場合
    if(mode == 3) {
        long double t = c1.r / (c1.r + c2.r);
        res.emplace_back(c1.p + (c2.p - c1.p) * t);
        return res;
    }
    // 内接している場合
    if(mode == 1) {
        if(c2.r < c1.r - EPS) {
            res.emplace_back(c1.p + (c2.p - c1.p) * (c1.r / d));
        } else {
            res.emplace_back(c2.p + (c1.p - c2.p) * (c2.r / d));
        }
        return res;
    }
    // 2円が重なる場合
    long double rc1 = (c1.r * c1.r + d * d - c2.r * c2.r) / (2 * d);
    long double rs1 = sqrt(c1.r * c1.r - rc1 * rc1);
    if(c1.r - abs(rc1) < EPS) {
        rs1 = 0;
    }
    Point e12 = (c2.p - c1.p) / abs(c2.p - c1.p);
    res.emplace_back(c1.p + rc1 * e12 + rs1 * e12 * Point(0, 1));
    res.emplace_back(c1.p + rc1 * e12 + rs1 * e12 * Point(0, -1));
    return res;
}

// 点pを通る円cの接線
// 2本あるので、接点のみを返す
vector<Point> tangentToCircle(const Point &p, const Circle &c) {
    return crossPoint(c, Circle(p, sqrt(norm(c.p - p) - c.r * c.r)));
}

vector<Line> tangent(const Circle &a, const Circle &b) {
    vector<Line> ret;
    // 2円の中心間の距離
    long double g = abs(a.p - b.p);
    // 円が内包されている場合
    if(equal(g, 0)) {
        return ret;
    }
    Point u = unitVector(b.p - a.p);
    Point v = rotate(u, PI / 2);
    for(int s : {-1, 1}) {
        long double h = (a.r + b.r * s) / g;
        if(equal(h * h, 1)) {
            ret.emplace_back(a.p + (h > 0 ? u : -u) * a.r,
                             a.p + (h > 0 ? u : -u) * a.r + v);

        } else if(1 - h * h > 0) {
            Point U = u * h, V = v * sqrt(1 - h * h);
            ret.emplace_back(a.p + (U + V) * a.r, b.p - (U + V) * (b.r * s));
            ret.emplace_back(a.p + (U - V) * a.r, b.p - (U - V) * (b.r * s));
        }
    }
    return ret;
}

