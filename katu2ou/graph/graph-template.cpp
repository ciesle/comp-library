/*
- 重みなしグラフ・重み付きグラフ (Graph, WGraph)

    - Edge : 重み付きの場合やindexを保管する必要がある場合などに使う
*/

template<typename T>
struct Edge{
    int from, to, index;
    T cost;
    Edge() : from(-1), to(-1), index(-1), cost(0) {}
    Edge(int _to) : from(-1), to(_to), index(-1), cost(0) {}
    Edge(int _to, T _cost) : from(-1), to(_to), index(-1), cost(_cost) {}
    Edge(int _from, int _to, int _index) : from(_from), to(_to), index(_index), cost(0) {}
    Edge(int _from, int _to, int _index, T _cost) 
        : from(_from), to(_to), index(_index), cost(_cost) {}
    bool operator<(const Edge<T>& other) const {
        return cost < other.cost; 
    }
    Edge &operator=(const int &x) {
        to = x;
        return *this;
    }
    operator int() const { return to; }
};
using Graph = vector<vector<int>>; //重みなしグラフ(行き先のみを情報にもつ)
template <typename T>
using WGraph = vector<vector<Edge<T>>>; //重みを持つグラフ
