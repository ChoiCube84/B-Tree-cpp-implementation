#ifndef __B_NODE__
#define __B_NODE__

#include "b_key_list.h"

template <typename T>
class BNode
{
private:
	BKeyList<T>* keys;
	// BNode* parent;
	BNode** children;
	size_t order;
	bool isLeaf;

	BNode* split() {
		BNode* tail = new BNode(order, keys->split(), isLeaf);

		currentSize = order / 2;

		for (int i = order / 2 + 1; i < order; i++) {
			tail->children[i - (order / 2 + 1)] = children[i];
			children[i] = nullptr;
		}

		return tail;
	}

public:

	// Basic constructor of BNode
	BNode(size_t order, bool isLeaf = false) : keys(new BKeyList<T>(order)), order(order), isLeaf(isLeaf)
	{
		if (isLeaf) {
			children = nullptr;
			std::cout << "B Node was made: This is leaf node" << std::endl;
		}
		else {
			children = new BNode*[order + 1];
			for (int i = 0; i < order + 1; i++) {
				children[i] = nullptr;
			}
			std::cout << "B Node was made" << std::endl;
		}
	}

	// Constructor of BNode for making a new root node
	BNode(size_t order, BNode* first, BNode* second) : keys(new BKeyList<T>(order)), order(order), isLeaf(false)
	{
		children = new BNode * [order + 1];

		children[0] = first;
		children[1] = second;

		for (int i = 2; i < order + 1; i++) {
			children[i] = nullptr;
		}

		std::cout << "New root B Node was made" << std::endl;
	}

	// Constructor of BNode for making a splitted node
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

	// Destructor of BNode
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

	void insert(const T& key) {
		// std::cout << "Not Implemented yet" << std::endl;
		// return false;

		size_t index = keys->findIndex(key);
		
		BNode* child = nullptr;

		if (isLeaf) {
			keys->insert(key);
		}
		else {
			child = children[index];
			child->insert(key);
		}

		bool splitRequired = keys->splitRequired();

		if (splitRequired) {
			// Should determine if this is root node or not
			// Split the node
			// If this is root node, make new root node
		}
	}

	void remove() {
		std::cout << "Not Implemented yet" << std::endl;
	}
};

#endif // !__B_NODE__
