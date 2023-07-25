#ifndef __B_TREE_CONSTRAINTS_H__
#define __B_TREE_CONSTRAINTS_H__

#include <string>
#include <sstream>
#include <concepts>

#include "b_tree_errors.h"

template<typename T>
concept Comparable = requires (T a, T b) {
	{ a == b } -> std::convertible_to<bool>;
	{ a != b } -> std::convertible_to<bool>;
	{ a < b } -> std::convertible_to<bool>;
	{ a > b } -> std::convertible_to<bool>;
};

template<typename T>
concept StringStreamUsable = requires (T a, std::stringstream ss) {
	{ ss << a };
};

template<typename T>
concept UsableInBTree = Comparable<T> && StringStreamUsable<T>;

size_t ensureValidOrder(size_t order) {
	if (order < 3) {
		throw InvalidOrder("The order of B-Tree should be bigger than 2");
	}

	return order;
}

#endif // !__B_TREE_CONSTRAINTS_H__
