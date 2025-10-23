#pragma once
#include<bits/stdc++.h>
#include "../utils/generator.h"
using namespace std;

template<uint crd, typename C = long long>
class Dijkstra {
public:
	using T = array<int, crd>;
	T n;
	int encode(const array<int, crd>& a) {
		int res = 0;
		for (size_t i = 0; i < crd; i++) {
			res = res * n[i] + a[i];
		}
		return res;
	}
	array<int, crd> decode(int v) {
		array<int, crd> res{};
		for (int i = int(crd) - 1; i >= 0; --i) {
			res[i] = v % n[i];
			v /= n[i];
		}
		return res;
	}
	int n_;
	struct edge {
		int to;
		C cost;
	};
	vector<vector<edge>>e;
public:
	bool directed;
	Dijkstra(T n, bool directed = false) :n(n), directed(directed) {
		n_ = 1;
		for (int i = 0;i < crd;i++) n_ *= n[i];
		e.resize(n_);
	}
	void add_edge(T a, T b, C cost) {
		e[encode(a)].push_back({ encode(b), cost });
		if (!directed) e[encode(b)].push_back({ encode(a), cost });
	}

	generator<pair<T, C>> exec(T s) {
		vector<C>dist(n_, numeric_limits<C>::max());
		using P = pair<C, int>;
		priority_queue<P, vector<P>, greater<P>>pq;
		dist[encode(s)] = 0;
		pq.push({ 0, encode(s) });
		while (!pq.empty()) {
			auto [d, v] = pq.top(); pq.pop();
			if (dist[v] < d) continue;
			co_yield{ decode(v), d };
			for (auto& ne : e[v]) {
				if (dist[ne.to] > dist[v] + ne.cost) {
					dist[ne.to] = dist[v] + ne.cost;
					pq.push({ dist[ne.to], ne.to });
				}
			}
		}
	}
};