#pragma once
#include "LazySegmentTree.h"
namespace SegmentTree {
	template<typename T>
	class Update : virtual public SegmentTree::LazySegmentTree<T> {
	protected:
		void update_one(Mono<T>& pos, T& x) {
			pos.val = x * pos.len;
			pos.lazy = x;
		}
		void fold(Mono<T>& pos) { pos.lazy = Mono<T>().lazy; }
		void propagate(Mono<T>& par, Mono<T>& child) {
			if (par.lazy != numeric_limits<T>::min()) {
				child.val = par.lazy * child.len;
				child.lazy = par.lazy;
			}
		}
	};
}