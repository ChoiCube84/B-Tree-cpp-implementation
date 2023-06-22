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
			children = new BNode*[order + 1];
			children[0] = first;
			children[1] = second;

			for (int i = 2; i < order + 1; i++) {
				children[i] = nullptr;
			}

			keys->insert(*key);

			std::cout << "B Node was made" << std::endl;
		}
	}

	BNode(size_t order, BKeyList<T>* keys, bool isLeaf) : order(order), keys(keys), isLeaf(isLeaf)
	{
		if (isLeaf) {
			children = nullptr;
			std::cout << "B Node was splitted: This is leaf node" << std::endl;
		}
		else {
			children = new BNode*[order + 1];
			for (int i = 0; i < order + 1; i++) {
				children[i] = nullptr;
			}

			std::cout << "B Node was splitted" << std::endl;
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

	BNode* split() {
		if (keys->getCurrentSize() < order) return nullptr;
		else {
			BNode* tail = new BNode(order, keys->split(), isLeaf);

			currentSize = order / 2;

			for (int i = order / 2 + 1; i < order; i++) {
				tail->children[i] = 
			}

			return tail;
		}
	}

	bool insert(const T& key) {
		// std::cout << "Not Implemented yet" << std::endl;
		// return false;

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
			if (isLeaf) {
				return true;
			}
			else {
				T promoted = child->keys[order / 2];
				
				int index = keys->findIndex(promoted);
				splitRequired = keys->insert(promoted); // We should return this value
				BKeyList<T>* temp = keys->split();

				for (int i = keys->getCurrentSize(); i > index + 1; i--) {
					children[i] = children[i - 1];
				}
				children[index + 1] = temp;
				return splitRequired;
			}
		}
		else {
			return false;
		}
	}

	void remove() {
		std::cout << "Not Implemented yet" << std::endl;
	}
};

#endif // !__B_NODE__
