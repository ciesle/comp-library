/*
  <幾何ライブラリ>
    - complex<long double> によって 2 次元幾何を扱う
    - 点，直線，線分，円，多角形，凸包，交点，接線などを扱う
    - 浮動小数点誤差を EPS で吸収する

    [型]
        - Point = complex<long double>
            点またはベクトルを表す
            x 座標は real(p), y 座標は imag(p) で取得する

        - Line
            2 点 a, b によって直線または線分を表す

        - Circle
            中心 p，半径 r の円を表す

    [実装/関数]
        - bool equal(ld a, ld b)
            long double の一致判定をする

        - Point unitVector(Point a)
            a と同じ向きの単位ベクトルを返す

        - Point normalVector(Point a)
            a を反時計回りに 90 度回転したベクトルを返す

        - ld dot(Point a, Point b)
            内積 a・b を返す

        - ld cross(Point a, Point b)
            外積 a×b を返す

        - Point rotate(Point p, ld theta)
            p を theta ラジアンだけ反時計回りに回転する

        - ld radianToDegree(ld radian)
            ラジアンを度数に変換する

        - ld degreeToRadian(ld degree)
            度数をラジアンに変換する

        - Point projection(Line l, Point p)
            点 p を直線 l に射影した点を返す

        - Point reflection(Line l, Point p)
            直線 l に関して点 p と対称な点を返す

        - int ccw(Point a, Point b, Point c)
            a を基準として b, c の位置関係を返す

              1  : 反時計回り
             -1  : 時計回り
              2  : c-a-b の順で一直線上
             -2  : a-b-c の順で一直線上
              0  : c が線分 ab 上

        - bool isOrthogonal(Line a, Line b)
            2 直線が直交するかを返す

        - bool isParallel(Line a, Line b)
            2 直線が平行かを返す

        - bool isIntersect(Line s, Line t)
            線分 s, t が交差するかを返す

        - Point crossPoint(Line s, Line t)
            直線 s, t の交点を返す
            平行で交点がない場合は assert で落ちる
            同一直線の場合は t.a を返す

        - ld distanceBetweenSegmentAndPoint(Line l, Point p)
            点 p と線分 l の距離を返す

        - ld distanceBetweenLineAndPoint(Line l, Point p)
            点 p と直線 l の距離を返す

        - ld distanceBetweenSegments(Line s, Line t)
            線分 s, t の距離を返す

        - ld SignedPolygonArea(vector<Point> p)
            符号付き面積を返す
            反時計回りなら正，時計回りなら負

        - ld PolygonArea(vector<Point> p)
            多角形の面積を非負で返す

        - bool isConvex(vector<Point> p)
            多角形が凸かどうかを返す
            頂点列は時計回り・反時計回りのどちらでもよい

        - int isContained(vector<Point> g, Point p)
            多角形 g に点 p が含まれるかを返す

              2 : 内部
              1 : 辺上
              0 : 外部

        - vector<Point> ConvexHull(vector<Point> p, bool include_collinear = false)
            点集合 p の凸包を返す

            include_collinear == false:
                凸包の頂点のみを返す

            include_collinear == true:
                凸包の辺上の点も可能な限り含める

        - int isIntersect(Circle c1, Circle c2)
            2 円の位置関係を返す
            返り値は共通接線の本数に対応する

              4  : 離れている
              3  : 外接
              2  : 2 点で交わる
              1  : 内接
              0  : 内包
             -1  : 同一円

        - Circle inCircle(Point a, Point b, Point c)
            三角形 abc の内接円を返す

        - vector<Point> crossPoint(Circle c, Line l)
            円 c と直線 l の交点を返す

        - vector<Point> crossPoint(Circle c1, Circle c2)
            2 円の交点を返す
            同一円の場合は空配列を返す

        - vector<Point> tangentToCircle(Point p, Circle c)
            点 p から円 c へ引いた接線の接点を返す
            p が円の内側にある場合は空配列を返す

        - vector<Line> tangent(Circle a, Circle b)
            2 円の共通接線を返す
            同心円の場合は空配列を返す

    [計算時間]
        - 基本演算，距離，交点計算
            O(1)

        - isContained
            O(N)

        - ConvexHull
            O(N log N)

    [備考]
        - EPS = 1e-10
        - 直線や線分は，基本的に異なる 2 点で定義されることを仮定する
        - rotate の角度 theta は「度」ではなく「ラジアン」
        - PolygonArea は非負の面積を返す
        - 符号付き面積が欲しい場合は SignedPolygonArea を使う
        - ConvexHull は引数を値渡しするため，元の配列を破壊しない
        - 同一円の交点・接線は無限個なので，この実装では列挙しない

    [verified at]
        - g++ 14.2.0, -std=gnu++23
        - 基本例 + ランダムテストで確認
*/

using ld = long double;
using Point = complex<long double>;
using pt = Point;

const long double EPS = 1e-10L;
const long double PI = acosl(-1.0L);

inline bool equal(const long double &a, const long double &b) {
    return fabsl(a - b) < EPS;
}

inline int sgn(const long double &x) {
    if (x > EPS) return 1;
    if (x < -EPS) return -1;
    return 0;
}

Point unitVector(const Point &a) {
    assert(abs(a) > EPS);
    return a / abs(a);
}

Point normalVector(const Point &a) {
    return a * Point(0, 1);
}

long double dot(const Point &a, const Point &b) {
    return a.real() * b.real() + a.imag() * b.imag();
}

long double cross(const Point &a, const Point &b) {
    return a.real() * b.imag() - a.imag() * b.real();
}

Point rotate(const Point &p, const long double &theta) {
    return Point(
        cos(theta) * p.real() - sin(theta) * p.imag(),
        sin(theta) * p.real() + cos(theta) * p.imag()
    );
}

long double radianToDegree(const long double &radian) {
    return radian * 180.0L / PI;
}

long double degreeToRadian(const long double &degree) {
    return degree * PI / 180.0L;
}

struct Line {
    Point a, b;

    Line() = default;

    Line(Point a, Point b) : a(a), b(b) {}

    // Ax + By = C を表す直線を作る
    Line(long double A, long double B, long double C) {
        assert(!equal(A, 0) || !equal(B, 0));

        if (equal(A, 0)) {
            a = Point(0, C / B);
            b = Point(1, C / B);
        } else if (equal(B, 0)) {
            a = Point(C / A, 0);
            b = Point(C / A, 1);
        } else {
            a = Point(0, C / B);
            b = Point(C / A, 0);
        }
    }
};

struct Circle {
    Point p;
    long double r;

    Circle() = default;

    Circle(Point p, long double r) : p(p), r(r) {}
};

Point projection(const Line &l, const Point &p) {
    assert(abs(l.b - l.a) > EPS);

    long double t = dot(p - l.a, l.b - l.a) / norm(l.b - l.a);
    return l.a + (l.b - l.a) * t;
}

Point reflection(const Line &l, const Point &p) {
    return p + (projection(l, p) - p) * 2.0L;
}

int ccw(const Point &a, Point b, Point c) {
    b -= a;
    c -= a;

    if (cross(b, c) > EPS) return 1;
    if (cross(b, c) < -EPS) return -1;
    if (dot(b, c) < -EPS) return 2;
    if (norm(b) + EPS < norm(c)) return -2;

    return 0;
}

bool isOrthogonal(const Line &a, const Line &b) {
    return equal(dot(a.b - a.a, b.b - b.a), 0);
}

bool isParallel(const Line &a, const Line &b) {
    return equal(cross(a.b - a.a, b.b - b.a), 0);
}

bool isIntersect(const Line &s, const Line &t) {
    return ccw(s.a, s.b, t.a) * ccw(s.a, s.b, t.b) <= 0 &&
           ccw(t.a, t.b, s.a) * ccw(t.a, t.b, s.b) <= 0;
}

Point crossPoint(const Line &s, const Line &t) {
    long double d1 = cross(s.b - s.a, t.b - t.a);
    long double d2 = cross(s.b - s.a, s.b - t.a);

    if (equal(d1, 0) && equal(d2, 0)) {
        return t.a;
    }

    assert(!equal(d1, 0));

    return t.a + (t.b - t.a) * (d2 / d1);
}

long double distanceBetweenSegmentAndPoint(const Line &l, const Point &p) {
    assert(abs(l.b - l.a) > EPS);

    if (dot(l.b - l.a, p - l.a) < EPS) {
        return abs(p - l.a);
    }

    if (dot(l.a - l.b, p - l.b) < EPS) {
        return abs(p - l.b);
    }

    return abs(cross(l.b - l.a, p - l.a)) / abs(l.b - l.a);
}

long double distanceBetweenLineAndPoint(const Line &l, const Point &p) {
    assert(abs(l.b - l.a) > EPS);

    return abs(cross(l.b - l.a, p - l.a)) / abs(l.b - l.a);
}

long double distanceBetweenSegments(const Line &s, const Line &t) {
    if (isIntersect(s, t)) {
        return 0;
    }

    long double ans = distanceBetweenSegmentAndPoint(s, t.a);

    ans = min(ans, distanceBetweenSegmentAndPoint(s, t.b));
    ans = min(ans, distanceBetweenSegmentAndPoint(t, s.a));
    ans = min(ans, distanceBetweenSegmentAndPoint(t, s.b));

    return ans;
}

long double SignedPolygonArea(const vector<Point> &p) {
    long double res = 0;
    int n = (int)p.size();

    for (int i = 0; i < n; i++) {
        res += cross(p[i], p[(i + 1) % n]);
    }

    return res * 0.5L;
}

long double PolygonArea(const vector<Point> &p) {
    return fabsl(SignedPolygonArea(p));
}

bool isConvex(const vector<Point> &p) {
    int n = (int)p.size();

    if (n < 3) return false;

    bool pos = false;
    bool neg = false;

    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        int k = (i + 2) % n;

        int s = sgn(cross(p[j] - p[i], p[k] - p[j]));

        if (s > 0) pos = true;
        if (s < 0) neg = true;

        if (pos && neg) return false;
    }

    return true;
}

int isContained(const vector<Point> &g, const Point &p) {
    bool in = false;
    int n = (int)g.size();

    for (int i = 0; i < n; i++) {
        Point a = g[i] - p;
        Point b = g[(i + 1) % n] - p;

        if (imag(a) > imag(b)) {
            swap(a, b);
        }

        if (imag(a) <= EPS && EPS < imag(b) && cross(a, b) < -EPS) {
            in = !in;
        }

        if (equal(cross(a, b), 0) && dot(a, b) <= EPS) {
            return 1;
        }
    }

    return in ? 2 : 0;
}

vector<Point> ConvexHull(vector<Point> p, bool include_collinear = false) {
    sort(p.begin(), p.end(), [](const Point &a, const Point &b) {
        if (!equal(real(a), real(b))) return real(a) < real(b);
        return imag(a) < imag(b) - EPS;
    });

    p.erase(
        unique(p.begin(), p.end(), [](const Point &a, const Point &b) {
            return abs(a - b) < EPS;
        }),
        p.end()
    );

    int n = (int)p.size();

    if (n <= 1) return p;

    vector<Point> ch(2 * n);
    int k = 0;

    auto bad = [&](const Point &a, const Point &b, const Point &c) {
        long double cr = cross(b - a, c - b);

        if (include_collinear) {
            return cr < -EPS;
        } else {
            return cr < EPS;
        }
    };

    for (int i = 0; i < n; i++) {
        while (k >= 2 && bad(ch[k - 2], ch[k - 1], p[i])) {
            --k;
        }
        ch[k++] = p[i];
    }

    for (int i = n - 2, t = k + 1; i >= 0; i--) {
        while (k >= t && bad(ch[k - 2], ch[k - 1], p[i])) {
            --k;
        }
        ch[k++] = p[i];
    }

    ch.resize(k - 1);

    if (!include_collinear && ch.size() == 2 && abs(ch[0] - ch[1]) < EPS) {
        ch.pop_back();
    }

    return ch;
}

int isIntersect(const Circle &c1, const Circle &c2) {
    long double d = abs(c1.p - c2.p);

    if (equal(d, 0) && equal(c1.r, c2.r)) {
        return -1;
    }

    if (d > c1.r + c2.r + EPS) {
        return 4;
    }

    if (equal(d, c1.r + c2.r)) {
        return 3;
    }

    if (d < abs(c1.r - c2.r) - EPS) {
        return 0;
    }

    if (equal(d, abs(c1.r - c2.r))) {
        return 1;
    }

    return 2;
}

Circle inCircle(const Point &a, const Point &b, const Point &c) {
    long double A = abs(b - c);
    long double B = abs(a - c);
    long double C = abs(a - b);

    assert(A + B + C > EPS);
    assert(abs(cross(b - a, c - a)) > EPS);

    Point p(
        A * real(a) + B * real(b) + C * real(c),
        A * imag(a) + B * imag(b) + C * imag(c)
    );

    p /= A + B + C;

    long double r = distanceBetweenLineAndPoint(Line(a, b), p);

    return Circle(p, r);
}

vector<Point> crossPoint(const Circle &c, const Line &l) {
    vector<Point> res;

    long double d = distanceBetweenLineAndPoint(l, c.p);

    if (d > c.r + EPS) {
        return res;
    }

    Point h = projection(l, c.p);

    if (equal(d, c.r)) {
        res.emplace_back(h);
        return res;
    }

    Point e = unitVector(l.b - l.a);
    long double ph = sqrt(max((long double)0, c.r * c.r - d * d));

    res.emplace_back(h - e * ph);
    res.emplace_back(h + e * ph);

    return res;
}

vector<Point> crossPoint(const Circle &c1, const Circle &c2) {
    vector<Point> res;

    int mode = isIntersect(c1, c2);
    long double d = abs(c1.p - c2.p);

    if (mode == 4 || mode == 0 || mode == -1) {
        return res;
    }

    if (mode == 3) {
        long double t = c1.r / (c1.r + c2.r);
        res.emplace_back(c1.p + (c2.p - c1.p) * t);
        return res;
    }

    if (mode == 1) {
        assert(d > EPS);

        if (c2.r < c1.r - EPS) {
            res.emplace_back(c1.p + (c2.p - c1.p) * (c1.r / d));
        } else {
            res.emplace_back(c2.p + (c1.p - c2.p) * (c2.r / d));
        }

        return res;
    }

    long double rc1 = (c1.r * c1.r + d * d - c2.r * c2.r) / (2 * d);
    long double sq = max((long double)0, c1.r * c1.r - rc1 * rc1);
    long double rs1 = sqrt(sq);

    Point e12 = (c2.p - c1.p) / d;

    res.emplace_back(c1.p + rc1 * e12 + rs1 * e12 * Point(0, 1));
    res.emplace_back(c1.p + rc1 * e12 + rs1 * e12 * Point(0, -1));

    return res;
}

vector<Point> tangentToCircle(const Point &p, const Circle &c) {
    long double d2 = norm(c.p - p);

    if (d2 < c.r * c.r - EPS) {
        return {};
    }

    return crossPoint(c, Circle(p, sqrt(max((long double)0, d2 - c.r * c.r))));
}

vector<Line> tangent(const Circle &a, const Circle &b) {
    vector<Line> ret;

    long double g = abs(a.p - b.p);

    if (equal(g, 0)) {
        return ret;
    }

    Point u = unitVector(b.p - a.p);
    Point v = rotate(u, PI / 2);

    for (int s : {-1, 1}) {
        long double h = (a.r + b.r * s) / g;

        if (equal(h * h, 1)) {
            ret.emplace_back(
                a.p + (h > 0 ? u : -u) * a.r,
                a.p + (h > 0 ? u : -u) * a.r + v
            );
        } else if (1 - h * h > EPS) {
            Point U = u * h;
            Point V = v * sqrt(max((long double)0, 1 - h * h));

            ret.emplace_back(
                a.p + (U + V) * a.r,
                b.p - (U + V) * (b.r * s)
            );

            ret.emplace_back(
                a.p + (U - V) * a.r,
                b.p - (U - V) * (b.r * s)
            );
        }
    }

    return ret;
}