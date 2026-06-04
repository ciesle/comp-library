#pragma once
#include "LazySegmentTree.h"
namespace SegmentTree {
	template<typename T>
	class None : virtual public SegmentTree::LazySegmentTree<T> {
	protected:
		void update_one(Mono<T>& pos, T& x) {
		}
		void fold(Mono<T>& pos) {}
		void propagate(Mono<T>& par, Mono<T>& child) {
		}
	};
}