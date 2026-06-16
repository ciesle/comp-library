#pragma once
#include "LazySegmentTree.h"
namespace SegmentTree {
	template<typename T>
	class Max : virtual public SegmentTree::LazySegmentTree<T> {
	protected:
		T eval(T& l, T& r) {
			return max(l, r);
		}
	};
}