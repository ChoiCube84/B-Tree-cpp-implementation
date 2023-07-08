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
	size_t order;
	size_t childIndex;
	bool isLeaf;
	BKeyList<T>* keys;
	BNode* parent;
	BNode** children;

	// Splits this node and return the tail node
	BNode* splitTailNode(void) {
		BNode* tail = new BNode(this);
		passChildrenToTailNode(tail);

		return tail;
	}

	// Constructor of BNode for making a splitted node
	BNode(BNode* currentNode)
		: order(currentNode->order), childIndex(currentNode->childIndex + 1), isLeaf(currentNode->isLeaf), keys(currentNode->keys->split()), parent(currentNode->parent), children(nullptr)
	{
		if (!isLeaf) {
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
				ss << child->childIndex << "th child: ";
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

	void setParent(BNode* newParent) { 
		parent = newParent; 
	}

	BNode* getChildByIndex(size_t idx) { 
		return children[idx]; 
	}

	void setChildByIndex(BNode* child, size_t idx) { 
		if (child != nullptr) {
			child->childIndex = idx;
		}

		children[idx] = child; 
	}

public:

	// Basic constructor of BNode
	BNode(size_t order, size_t childIndex = 0, bool isLeaf = false) 
		: order(order), childIndex(childIndex), isLeaf(isLeaf), keys(new BKeyList<T>(order)), parent(nullptr), children(nullptr)
	{
		if (!isLeaf) {
			children = new BNode*[order + 1];

			for (size_t i = 0; i < order + 1; i++) {
				setChildByIndex(nullptr, i);
			}	
		}

		std::cout << "B Node was made" << std::endl;
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

	bool remove(const T& key) {
		if (isLeaf) {
			bool deletionSuccess = keys->remove(key);
			
			if (keys->isEmpty()) {
				// TODO: Rebalancing required
			}
		}

		else {
			// TODO: Handle when it is internal node
		}
	}

	BNode* getLeftSibling(void) {
		if (childIndex == 0) {
			return nullptr;
		}
		else {
			return parent->children[childIndex - 1];
		}
	}

	BNode* getRightSibling(void) {
		if (childIndex == parent->keys->getCurrentSize()) {
			return nullptr;
		}
		else {
			return parent->children[childIndex + 1];
		}
	}

	std::string preOrder(void) {
		std::stringstream ss;
		preOrder(ss);
		return ss.str();
	}

	std::string inOrder(void) {
		std::stringstream ss;
		inOrder(ss);
		return ss.str();
	}

	std::string postOrder(void) {
		std::stringstream ss;
		postOrder(ss);
		return ss.str();
	}

	bool hasParent(void) { 
		return parent != nullptr; 
	}

	BNode* getParent(void) { 
		return parent; 
	}
};

#endif // !__B_NODE__
