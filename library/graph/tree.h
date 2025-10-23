#pragma once
#include<bits/stdc++.h>
using namespace std;
class Tree {
	void build_dfs(int v, int p) {
		for (auto& to : e[v]) {
			if (to == e[v].back() && to == p) continue;
			if (to == p) swap(to, e[v].back());
			build_dfs(to, v);
		}
		e[v].pop_back();
	}
	vector<int>dfs_order;
	void construct_dfs(int v, int p = -1) {
		dfs_order.emplace_back(v);
		for (auto& to : e[v]) {
			if (to != p) construct_dfs(to, v);
		}
	}
	vector<int>dep;
	void build_depth(int v, int p, int d) {
		dep[v] = d;
		for (auto& to : e[v]) {
			if (to != p) build_depth(to, v, d + 1);
		}
	}
public:
	int n;
	int root;
	vector<vector<int>>e;
	//number of nodes, root(-1 if not rooted)
	Tree(int n, int root = 0) :n(n), e(n), root(root) {}
	void input_all() {
		for (int i = 0;i < n - 1;i++) {
			int a, b; cin >> a >> b;
			a--; b--;
			e[a].emplace_back(b);
			e[b].emplace_back(a);
		}
		if (~root) build(root);
	}
	void add_edge(int a, int b) {
		e[a].emplace_back(b);
		e[b].emplace_back(a);
	}
	void build(int root = 0) {
		build_dfs(root, -1);
		dep.resize(n);
		build_depth(root, -1, 0);
	}
	const int depth(int v) {
		return dep[v];
	}
	const int* begin() {
		if (dfs_order.empty()) construct_dfs(root);
		return dfs_order.data();
	}
	const int* end() {
		if (dfs_order.empty()) construct_dfs(root);
		return dfs_order.data() + dfs_order.size();
	}
	vector<int>& operator[](int k) {
		return e[k];
	}
};