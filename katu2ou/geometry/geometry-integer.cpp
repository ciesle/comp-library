/*
  <幾何ライブラリ(格子点)>
    - 格子点上の幾何を扱うためのライブラリ
    - 座標が整数であるため，直線上判定・外積・凸包などを誤差なく扱える

    [型]
        - Point
            点を表す構造体
            メンバは x, y

        - Line
            2 点 a, b によって直線を表す構造体
            線分として使いたい場合も，同じく端点 a, b を使う

    [実装/関数]
        - i64 dot(Point a, Point b)
            内積 a・b を返す

        - i64 cross(Point a, Point b)
            外積 a×b を返す

        - i64 det(Point a, Point b, Point c)
            ベクトル b-a と c-a の外積を返す
            det(a,b,c) > 0 なら a -> b -> c は反時計回り
            det(a,b,c) < 0 なら時計回り
            det(a,b,c) = 0 なら一直線上

        - bool collinear(Point a, Point b, Point c)
            a,b,c が一直線上にあるかを返す

        - bool on_line(Line L, Point p)
            p が直線 L 上にあるかを返す

        - bool on_segment(Point a, Point b, Point p)
            p が線分 ab 上にあるかを返す

        - Line perpendicular_line(Line L, Point P)
            点 P を通り，直線 L に垂直な直線を返す
            P は L 上になくてもよい

        - vector<Point> convex_hull(vector<Point> pts, bool include_collinear = false)
            点集合 pts の凸包を返す
            返り値は反時計回り順
            始点は辞書順最小の点

            include_collinear == false の場合:
                凸包の頂点だけを返す
                辺上の余分な共線点は含めない

            include_collinear == true の場合:
                凸包の辺上にある共線点も可能な限り含める
                ただし，全点が一直線上の場合は，重複なしでソート順に返す

        - InPoly point_in_polygon(vector<Point> poly, Point p)
            点 p が多角形 poly の内部・境界上・外部のどこにあるかを返す

            返り値:
                InPoly::IN   : 内部
                InPoly::ON   : 境界上
                InPoly::OUT  : 外部

    [計算時間]
        - dot, cross, det, collinear, on_line, on_segment
            O(1)

        - perpendicular_line
            O(1)

        - convex_hull
            O(N log N)

        - point_in_polygon
            O(N)

    [備考]
        - 座標や外積の計算結果が long long に収まることを仮定している
        - 座標の絶対値が 10^9 程度なら，多くの場合 long long で十分
        - より大きい座標を扱う場合は，cross / det の内部計算を __int128 にすること
        - point_in_polygon は単純多角形を仮定する
        - poly の最後に先頭点を重複して入れる必要はない

    [参照]
        - https://bakamono1357.hatenablog.com/entry/2020/04/29/025320

    [verified at]
        - g++ 14.2.0, -std=gnu++23
        - ランダムテストで確認
*/

using i64 = long long;

struct Point {
    i64 x, y;

    bool operator==(const Point &o) const noexcept {
        return x == o.x && y == o.y;
    }

    bool operator<(const Point &o) const noexcept {
        return x != o.x ? x < o.x : y < o.y;
    }
};

struct Line {
    Point a, b;

    Line() = default;

    Line(Point a, Point b) : a(a), b(b) {}
};

i64 dot(const Point &a, const Point &b) {
    return a.x * b.x + a.y * b.y;
}

i64 cross(const Point &a, const Point &b) {
    return a.x * b.y - a.y * b.x;
}

i64 det(const Point &a, const Point &b, const Point &c) noexcept {
    return cross(
        Point{b.x - a.x, b.y - a.y},
        Point{c.x - a.x, c.y - a.y}
    );
}

bool collinear(const Point &a, const Point &b, const Point &c) noexcept {
    return det(a, b, c) == 0;
}

bool on_line(const Line &L, const Point &p) noexcept {
    return collinear(L.a, L.b, p);
}

bool on_segment(const Point &a, const Point &b, const Point &p) noexcept {
    return collinear(a, b, p) &&
           min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) &&
           min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}

Line perpendicular_line(const Line &L, const Point &P) noexcept {
    i64 dx = L.b.x - L.a.x;
    i64 dy = L.b.y - L.a.y;

    // L の方向ベクトル (dx, dy) に垂直な方向ベクトルは (-dy, dx)
    Point Q{P.x - dy, P.y + dx};

    return Line{P, Q};
}

vector<Point> convex_hull(vector<Point> pts, bool include_collinear = false) {
    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());

    int n = (int)pts.size();

    if (n <= 1) return pts;

    bool all_collinear = true;
    for (int i = 2; i < n; i++) {
        if (det(pts[0], pts[1], pts[i]) != 0) {
            all_collinear = false;
            break;
        }
    }

    if (all_collinear) {
        if (include_collinear) return pts;
        return vector<Point>{pts.front(), pts.back()};
    }

    auto should_pop = [&](const Point &a, const Point &b, const Point &c) {
        i64 d = det(a, b, c);

        if (include_collinear) {
            return d < 0;
        } else {
            return d <= 0;
        }
    };

    vector<Point> lower, upper;

    for (const auto &p : pts) {
        while ((int)lower.size() >= 2 &&
               should_pop(lower[(int)lower.size() - 2], lower.back(), p)) {
            lower.pop_back();
        }
        lower.push_back(p);
    }

    for (auto it = pts.rbegin(); it != pts.rend(); ++it) {
        while ((int)upper.size() >= 2 &&
               should_pop(upper[(int)upper.size() - 2], upper.back(), *it)) {
            upper.pop_back();
        }
        upper.push_back(*it);
    }

    lower.pop_back();
    upper.pop_back();

    lower.insert(lower.end(), upper.begin(), upper.end());

    return lower;
}

enum class InPoly {
    OUT,
    ON,
    IN
};

InPoly point_in_polygon(const vector<Point> &poly, const Point &p) {
    assert((int)poly.size() >= 3);

    bool inside = false;
    int n = (int)poly.size();

    for (int i = 0, j = n - 1; i < n; j = i++) {
        const Point &a = poly[j];
        const Point &b = poly[i];

        if (on_segment(a, b, p)) {
            return InPoly::ON;
        }

        bool intersect_y = (a.y > p.y) != (b.y > p.y);
        i64 d = det(a, b, p);

        if (intersect_y && (d > 0) == (b.y > a.y)) {
            inside = !inside;
        }
    }

    return inside ? InPoly::IN : InPoly::OUT;
}