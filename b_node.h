#ifndef __B_NODE__
#define __B_NODE__

#include "b_key_list.h"

template <typename T>
struct BNode
{
	BKeyList<T>* keys;
	BNode** children;
	size_t order;

	BNode(size_t order) : keys(new BKeyList<T>(order)), children(nullptr), order(order)
	{
		std::cout << "B Node was made" << std::endl;
	}
	~BNode()
	{
		delete keys;

		if (children != nullptr) {
			for (size_t i = 0; i < order; i++) {
				delete children[i];
			}
			delete[] children;
		}

		std::cout << "B Node was destructed" << std::endl;
	}
};

#endif // !__B_NODE__
