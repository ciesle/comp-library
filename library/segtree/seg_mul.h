#pragma once
#include "LazySegmentTree.h"
namespace SegmentTree {
	template<typename T>
	class Mul : virtual public SegmentTree::LazySegmentTree<T> {
	protected:
		void update_one(Mono<T>& pos, T& x) {
			pos.val *= x.pow(pos.len);
			pos.lazy *= x;
		}
		void fold(Mono<T>& pos) { pos.lazy = Mono<T>().lazy; }
		void propagate(Mono<T>& par, Mono<T>& child) {
			child.val *= par.lazy.pow(child.len);
			child.lazy *= par.lazy;
		}
	};
}