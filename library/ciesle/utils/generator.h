#pragma once
#include<bits/stdc++.h>

template <typename T> struct yield { T value; };
template <typename T>
struct generator {
	struct promise_type {
		T value_;
		auto get_return_object() { return generator(*this); }
		auto initial_suspend() noexcept { return std::suspend_always{}; }
		auto final_suspend() noexcept { return std::suspend_always{}; }
		// 式co_await yield{v}に対応するカスタイマイズポイント
		auto await_transform(yield<T>&& bag) {
			value_ = std::move(bag.value);
			return std::suspend_always{};
		}
		void return_void() {}
		void unhandled_exception() { throw; }
	};
	using coro_handle = std::coroutine_handle<promise_type>;

	generator(generator const&) = delete;
	generator(generator&& rhs) : coro_(rhs.coro_) { rhs.coro_ = nullptr; }
	~generator() { if (coro_) coro_.destroy(); }

	bool next() { return coro_ ? (coro_.resume(), !coro_.done()) : false; }
	T& value() { return coro_.promise().value_; }

private:
	generator(promise_type& p)
		: coro_(coro_handle::from_promise(p)) {
	}
	coro_handle coro_;
};