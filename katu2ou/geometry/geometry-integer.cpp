/*
  <幾何ライブラリ(格子点)>
    - 格子点上の幾何を扱う．直線上の存在判定が正確にできたりする

    [実装/関数]
        - Point = pair<ll,ll> で点を扱う
        - 
    
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

struct Point {
    i64 x, y;
    bool operator==(const Point &o) const noexcept { return x == o.x && y == o.y; }
    bool operator<(const Point &o) const noexcept {
        return x != o.x ? x < o.x : y < o.y;
    }
};
// Line : 直線を表す構造体
// b - a で直線・線分を表せる
struct Line {
    Point a, b; 
    Line() = default;
    Line(Point a, Point b) : a(a), b(b) {}
};

long long dot(const Point &a, const Point &b) {
    return (a.x * b.x + a.y* b.second);
}

// 外積(cross product) : a×b = |a||b|sinΘ
long long cross(const Point &a, const Point &b) {
    return (a.x * b.second - a.second * b.x);
}

long long det(const Point &a, const Point &b, const Point &c) noexcept {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool collinear(const Point &a, const Point &b, const Point &c) noexcept {
    return det(a, b, c) == 0;
}

inline bool on_line(const Line &L, const Point &p) noexcept {
    return collinear(L.a, L.b, p);
}

inline Line perpendicular_line(const Line &L, const Point &P) noexcept {
    #ifdef LOCAL
        assert(on_line(L, P));
    #endif
        long long dx = L.b.x - L.a.x;
        long long dy = L.b.y - L.a.y;
        // Perpendicular direction (−dy, dx)
        Point Q{P.x - dy, P.y + dx};
        return {P, Q};
    }

inline std::vector<Point> convex_hull(std::vector<Point> pts) {
    if (pts.size() <= 1) return pts;
    std::sort(pts.begin(), pts.end());
    std::vector<Point> lo, up;
    // lower
    for (auto &p : pts) {
        while (lo.size() >= 2 && det(lo[lo.size()-2], lo.back(), p) < 0) lo.pop_back();
        lo.push_back(p);
    }
    // upper
    for (auto it = pts.rbegin(); it != pts.rend(); ++it) {
        while (up.size() >= 2 && det(up[up.size()-2], up.back(), *it) < 0) up.pop_back();
        up.push_back(*it);
    }
    lo.pop_back(); up.pop_back();
    lo.insert(lo.end(), up.begin(), up.end());
    return lo; // counter‑clockwise, starting from lexicographically smallest.
}

//-------------------------------- 4) point‑in‑polygon (O(n)) ------------//
enum class InPoly { OUT, ON, IN };
inline InPoly point_in_polygon(const std::vector<Point> &poly, const Point &p) {
    // polygon must be simple; last vertex need not equal first.
    bool inside = false;
    int n = poly.size();
    for (int i = 0, j = n-1; i < n; j = i++) {
        const Point &a = poly[j], &b = poly[i];
        if (collinear(a, b, p) && std::min(a.x,b.x) <= p.x && p.x <= std::max(a.x,b.x)
                               && std::min(a.y,b.y) <= p.y && p.y <= std::max(a.y,b.y))
            return InPoly::ON;
        bool c1 = (a.y > p.y) != (b.y > p.y);
        i64 d = det(a, b, p);
        if (c1 && d == 0) return InPoly::ON; // edge case but collinear already handled
        if (c1 && (d > 0) == (b.y > a.y)) inside = !inside;
    }
    return inside ? InPoly::IN : InPoly::OUT;
}