#include<bits/stdc++.h>
using namespace std;

template <typename T>
struct is_vector : std::false_type {};
template <typename T, typename A>
struct is_vector<std::vector<T, A>> : std::true_type {};

template <typename T>
struct is_pair : std::false_type {};
template <typename T, typename U>
struct is_pair<std::pair<T, U>> : std::true_type {};

template <typename T>
int preflight_debug(const T& value) {
	int ret = 0;
	if constexpr (is_vector<T>::value) ret = preflight_debug(value[0]) + 1;
	return ret;
}
template <typename T>
void print_debug(const T& value, vector<int>hist = vector<int>(), int idx = -1, int indent = 0) {
	if constexpr (is_vector<T>::value) {
		int dept = preflight_debug(value);
		if (idx == 0)  cerr << endl;
		cerr << string(indent, ' ');
		if (dept > 1 && ~idx) {
			cerr << "(";
			for (int i = 0;i < hist.size();i++) cerr << hist[i] << (i == hist.size() - 1 ? ")" : ", ");
		}
		cerr << "[ ";
		for (size_t i = 0; i < value.size(); ++i) {
			hist.push_back(i);
			print_debug(value[i], hist, (i == 0 ? 0 : (i == value.size() - 1 ? 2 : 1)), indent + 2);
			hist.pop_back();
		}
		if (dept > 1) cerr << string(indent, ' ');
		cerr << "]" << ((idx == 0 || idx == 1) ? "," : "") << endl;
	}
	else if constexpr (is_pair<T>::value) {
		cerr << "{ first: " << value.first
			 << ", second: " << value.second << "} ";
	}
	else {
		cerr << value << " ";
	}
}

template <typename T, size_t N>
int preflight_debug(const T(&arr)[N]) {
	int ret = 0;
	ret = preflight_debug(arr[0]) + 1;
	return ret;
}
template <typename T, size_t N>
void print_debug(const T(&arr)[N], vector<int>hist = vector<int>(), int idx = -1, int indent = 0) {
	int dept = preflight_debug(arr);
	if (idx == 0)  cerr << endl;
	cerr << string(indent, ' ');
	if (dept > 1 && ~idx) {
		cerr << "(";
		for (int i = 0;i < hist.size();i++) cerr << hist[i] << (i == hist.size() - 1 ? ")" : ", ");
	}
	cerr << "[ ";
	for (size_t i = 0; i < N; ++i) {
		hist.push_back(i);
		print_debug(arr[i], hist, (i == 0 ? 0 : (i == N - 1 ? 2 : 1)), indent + 2);
		hist.pop_back();
	}
	if (dept > 1) cerr << string(indent, ' ');
	cerr << "]" << ((idx == 0 || idx == 1) ? "," : "") << endl;
}


template <typename... Args>
void debug(const Args... args) {
	((print_debug(args), cerr << ' '), ...);
	cerr << endl;
}
