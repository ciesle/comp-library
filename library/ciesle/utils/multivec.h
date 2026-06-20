
#pragma once
#include <bits/stdc++.h>

template<typename T>
T multivec(const T& value) {
	return value;
}

template<typename T, typename Size, typename Next, typename... Args>
auto multivec(Size size, Next next, Args... args) {
	static_assert(std::is_integral_v<Size>, "multivec sizes must be integers");
	auto inner = multivec<T>(next, args...);
	return std::vector<decltype(inner)>(static_cast<std::size_t>(size), inner);
}
