#ifndef __B_NODE__
#define __B_NODE__
#include <iostream>
#include <string>
#include <sstream>

#include "b_key_list.h"

template <typename T>
class BNode
{
private:
	BKeyList<T>* keys;
	BNode* parent;
	BNode** children;
	size_t order;
	bool isLeaf;

	// Splits this node and return the tail node
	BNode* splitTailNode(void) {
		BNode* tail = new BNode(this);
		passChildrenToTailNode(tail);

		return tail;
	}

	// Constructor of BNode for making a splitted node
	BNode(BNode* currentNode)
		: order(currentNode->order), parent(currentNode->parent), keys(currentNode->keys->split()), isLeaf(currentNode->isLeaf)
	{
		if (isLeaf) {
			children = nullptr;
			std::cout << "This is leaf node" << std::endl;
		}
		else {
			children = new BNode * [order + 1];
			for (size_t i = 0; i < order + 1; i++) {
				setChildByIndex(nullptr, i);
			}
		}

		std::cout << "B Node was splitted" << std::endl;
	}

	void passChildrenToTailNode(BNode* tail) {
		if (!isLeaf) {
			for (size_t i = order / 2 + 1; i < order + 1; i++) {
				BNode* childToPass = getChildByIndex(i);
				tail->setChildByIndex(childToPass, i - (order / 2 + 1));
				setChildByIndex(nullptr, i);
			}
		}
	}

	void makeNewRootNode(BNode* tail) {
		setParent(new BNode(this->order));
		tail->setParent(parent);

		parent->setChildByIndex(this, 0);
		parent->setChildByIndex(tail, 1);

		for (size_t i = 2; i < order + 1; i++) {
			parent->setChildByIndex(nullptr, i);
		}
	}

	void connectTailNodeToParentNode(BNode* tail, T& promotedKey) {
		size_t indexOfPromotedKey = parent->keys->findIndex(promotedKey);

		shiftChildrenOfParentNode(indexOfPromotedKey);
		parent->setChildByIndex(tail, indexOfPromotedKey + 1);
	}

	void shiftChildrenOfParentNode(size_t indexOfPromotedKey) {
		for (size_t i = parent->keys->getCurrentSize(); i > indexOfPromotedKey + 1; i--) {
			BNode* childToShift = parent->getChildByIndex(i - 1);
			parent->setChildByIndex(childToShift, i);
		}
	}

	void preOrder(std::stringstream& ss) {
		BNode* child = nullptr;

		ss << keys->traverse();

		if (!isLeaf) {
			for (size_t i = 0; i < keys->getCurrentSize() + 1; i++) {
				child = getChildByIndex(i);
				
				// TODO: Delete these brackets later
				ss << " (";
				child->preOrder(ss);
				ss << ")"; 
			}
		}
	}

	void inOrder(std::stringstream& ss) {
		BNode* child = nullptr;

		if (isLeaf) {
			ss << keys->traverse();
		}
		else {
			for (size_t i = 0; i < keys->getCurrentSize(); i++) {
				child = getChildByIndex(i);
				child->inOrder(ss);
				ss << " " << keys->getKeyByIndex(i) << " ";
			}
			child = getChildByIndex(keys->getCurrentSize());
			child->inOrder(ss);
		}
	}

	void postOrder(std::stringstream& ss) {
		BNode* child = nullptr;

		if (!isLeaf) {
			for (size_t i = 0; i < keys->getCurrentSize() + 1; i++) {
				child = getChildByIndex(i);

				// TODO: Delete these brackets later
				ss << "(";
				child->postOrder(ss);
				ss << ") ";
			}
		}

		ss << keys->traverse();
	}

	void setParent(BNode* newParent) { parent = newParent; }

	BNode* getChildByIndex(size_t idx) { return children[idx]; }
	void setChildByIndex(BNode* child, size_t idx) { children[idx] = child; }

public:

	// Basic constructor of BNode
	BNode(size_t order, bool isLeaf = false) 
		: keys(new BKeyList<T>(order)), order(order), parent(nullptr), isLeaf(isLeaf)
	{
		if (isLeaf) {
			children = nullptr;
			std::cout << "B Node was made: This is leaf node" << std::endl;
		}
		else {
			children = new BNode*[order + 1];

			for (size_t i = 0; i < order + 1; i++) {
				setChildByIndex(nullptr, i);
			}

			std::cout << "B Node was made" << std::endl;
		}
	}

	// Destructor of BNode
	~BNode()
	{
		delete keys;

		if (!isLeaf) {
			for (size_t i = 0; i < order; i++) {
				delete children[i];
			}
			delete[] children;
		}

		std::cout << "B Node was destructed" << std::endl;
	}

	void insert(const T& key) {
		size_t indexOfKey = keys->findIndex(key);
		
		if (isLeaf) {
			keys->insert(key);
		}
		else {
			BNode* child = getChildByIndex(indexOfKey);
			child->insert(key);
		}

		bool splitRequired = keys->splitRequired();
		
		if (splitRequired) {
			T promotedKey = keys->getKeyByIndex(order / 2);
			BNode* tail = splitTailNode();
			
			if (!hasParent()) {
				makeNewRootNode(tail);
			}
			
			parent->keys->insert(promotedKey);
			connectTailNodeToParentNode(tail, promotedKey);
		}
	}

	void remove() {
		std::cout << "Not Implemented yet" << std::endl;
	}

	std::string preOrder() {
		std::stringstream ss;
		preOrder(ss);
		return ss.str();
	}

	std::string inOrder() {
		std::stringstream ss;
		inOrder(ss);
		return ss.str();
	}

	std::string postOrder() {
		std::stringstream ss;
		postOrder(ss);
		return ss.str();
	}

	bool hasParent() { return parent != nullptr; }
	BNode* getParent() { return parent; }
};

#endif // !__B_NODE__
