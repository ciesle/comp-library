#pragma once
#include<bits/stdc++.h>
using namespace std;

class Measure {
	vector<chrono::system_clock::time_point> points;
	unordered_map<int, chrono::system_clock::time_point>start_time;
	unordered_map<int, chrono::system_clock::duration>durations;
public:
	Measure() {}
	inline void start(int id) {
		start_time[id] = chrono::system_clock::now();
	}
	inline void end(int id) {
		durations[id] += chrono::system_clock::now() - start_time[id];
	}
	inline void add_point(int id = 0) {
		points.emplace_back(chrono::system_clock::now());
	}
	template<typename T = chrono::milliseconds>
	void output() {
		cerr << "Durations\n";
		for (auto [id, time] : durations) {
			cerr << "id " << id << " : " << chrono::duration_cast<T>(time) << endl;
		}
		cerr << endl;

		cerr << "Time points\n";
		for (int i = 1;i < points.size();i++) {
			cerr << "point " << i - 1 << " to " << i << " : " << chrono::duration_cast<T>(points[i] - points[i - 1]) << endl;
		}
		cerr << endl;
	}
};