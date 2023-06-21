#ifndef __B_NODE__
#define __B_NODE__

#include "b_key_list.h"

template <typename T>
class BNode
{
private:
	BKeyList<T>* keys;
	BNode** children;
	size_t order;
	bool isLeaf;

public:
	BNode(size_t order, bool isLeaf = false, T* key = nullptr, BNode* first = nullptr, BNode* second = nullptr) : keys(new BKeyList<T>(order)), order(order), isLeaf(isLeaf)
	{
		if (isLeaf) {
			children = nullptr;
			std::cout << "B Node was made: This is leaf node" << std::endl;
		}
		else {
			children = new BNode*[order];
			children[0] = first;
			children[1] = second;

			for (int i = 2; i < order; i++) {
				children[i] = nullptr;
			}

			keys->insert(*key);

			std::cout << "B Node was made" << std::endl;
		}
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

	BNode** getChildren() { return children; }
	BKeyList<T>* getKeys() { return keys; }

	bool insert(const T& key) {
		int index = keys->findIndex(key);
		bool splitRequired = false;

		BNode* child = nullptr;

		if (isLeaf) {
			splitRequired = keys->insert(key);
		}
		else {
			child = children[index];
			splitRequired = child->insert(key);
		}

		if (splitRequired) {
			// TODO: Implented splitting
			if (isLeaf) {
				return true;
			}
			else {
				T promoted = child->keys[order / 2];
				int index = keys->findIndex(promoted);

				splitRequired = keys->insert(promoted);
				
				BKeyList<T>* temp = keys->split();
				for (int i = index + 1; i < keys->getCurrentSize(); i++) {
					// Push children to back
				}
			}
		}
		else {
			// lorem ipsum
		}

		std::cout << "Not Implemented yet" << std::endl;
		return false;
	}

	void remove() {
		std::cout << "Not Implemented yet" << std::endl;
	}
};

#endif // !__B_NODE__
