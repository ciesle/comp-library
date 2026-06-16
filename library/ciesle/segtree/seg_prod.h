#pragma once
#include "LazySegmentTree.h"
namespace SegmentTree {
	template<typename T>
	class Prod : virtual public SegmentTree::LazySegmentTree<T> {
	protected:
		T eval(T& l, T& r) {
			return l * r;
		}
	};
}