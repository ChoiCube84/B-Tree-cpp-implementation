#ifndef __B_NODE__
#define __B_NODE__

#include "b_key_list.h"

template <typename T>
class BNode
{
private:
	BKeyList<T>* keys; // A pointer that points to a BKeyList holding the keys of this node
	BNode* parent;	   // A pointer that points to the parent node
	BNode** children;  // An array of pointers that point to each child node
	size_t order;	   // The order of this B-Tree
	bool isLeaf;	   // Indicator of whether this node is a leaf node

	BNode* split() {
		BNode* tail = new BNode(order, parent, keys->split(), isLeaf);

		currentSize = order / 2;

		for (int i = order / 2 + 1; i < order; i++) {
			tail->children[i - (order / 2 + 1)] = children[i];
			children[i] = nullptr;
		}

		return tail;
	}

public:

	// Basic constructor of BNode
	BNode(size_t order, BNode* parent, bool isLeaf = false) : keys(new BKeyList<T>(order)), order(order), parent(parent), isLeaf(isLeaf)
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
		parent = nullptr;
		children = new BNode * [order + 1];

		children[0] = first;
		children[1] = second;

		for (int i = 2; i < order + 1; i++) {
			children[i] = nullptr;
		}

		std::cout << "New root B Node was made" << std::endl;
	}

	// Constructor of BNode for making a splitted node
	BNode(size_t order, BNode* parent, BKeyList<T>* keys, bool isLeaf) : order(order), keys(keys), isLeaf(isLeaf)
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
			if (parent == nullptr) {
				 // Handle when it is root node
			}
			else {
				T promoted = keys[order / 2];

				int index = parent->keys->findIndex(promoted);
				parent->keys->insert(promoted); 

				BKeyList<T>* tail = keys->split();

				for (int i = parent->keys->getCurrentSize(); i > index + 1; i--) {
					parent->children[i] = parent->children[i - 1];
				}

				parent->children[index + 1] = new BNode(order, parent, tail, isLeaf);
			}
		}
	}

	void remove() {
		std::cout << "Not Implemented yet" << std::endl;
	}
};

#endif // !__B_NODE__
