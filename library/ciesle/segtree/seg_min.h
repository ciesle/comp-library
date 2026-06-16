#pragma once
#include "LazySegmentTree.h"
namespace SegmentTree {
	template<typename T>
	class Min : virtual public SegmentTree::LazySegmentTree<T> {
	protected:
		T eval(T& l, T& r) {
			return min(l, r);
		}
	};
}