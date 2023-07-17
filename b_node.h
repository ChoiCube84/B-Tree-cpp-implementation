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

	BNode* splitTailNode(void) {
		BNode* tail = new BNode(this);
		passChildrenToTailNode(tail);

		return tail;
	}

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

	void preOrder(std::stringstream& ss, bool useBrackets = false) {
		BNode* child = nullptr;

		ss << keys->traverse();

		if (!isLeaf) {
			for (size_t i = 0; i < keys->getCurrentSize() + 1; i++) {
				child = getChildByIndex(i);
				
				ss << " ";

				if (useBrackets) {
					ss << "(";
				}

				child->preOrder(ss, useBrackets);

				if (useBrackets) {
					ss << ")";
				}
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

	void postOrder(std::stringstream& ss, bool useBrackets = false) {
		BNode* child = nullptr;

		if (!isLeaf) {
			for (size_t i = 0; i < keys->getCurrentSize() + 1; i++) {
				child = getChildByIndex(i);

				if (useBrackets) {
					ss << "(";
				}
				
				child->postOrder(ss, useBrackets);

				if (useBrackets) { 
					ss << ")"; 
				}

				ss << " ";
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
			child->parent = this;
			child->childIndex = idx;
		}

		children[idx] = child; 
	}

	bool isDeficientNode(void) {
		if (parent == nullptr) {
			return false;
		}
		else {
			size_t numberOfChildrenNodes = keys->getCurrentSize() + 1;
			size_t minimumNumberOfRequiredKeys = (order + 1) / 2;

			return numberOfChildrenNodes < minimumNumberOfRequiredKeys;
		}
	}

	bool isExceedingNode(void) {
		size_t numberOfChildrenNodes = keys->getCurrentSize() + 1;
		size_t minimumNumberOfRequiredKeys = (order + 1) / 2;

		return numberOfChildrenNodes > minimumNumberOfRequiredKeys;
	}

public:
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

	~BNode()
	{
		delete keys;

		//TODO: This condition statement should be revised
		if (!isLeaf && children != nullptr) {
			for (size_t i = 0; i < order; i++) {
				BNode* child = children[i];
				delete child;
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
		bool deletionResult = false;

		if (isLeaf) {
			deletionResult = keys->remove(key);

			if (isDeficientNode()) {
				rebalance();
			}
		}
		else {
			size_t indexOfKey = keys->findIndex(key);

			if (keys->isExistingKey(key)) {
				BNode* leftChildOfKey = getChildByIndex(indexOfKey);
				BNode* rightChildOfKey = getChildByIndex(indexOfKey + 1);

				// TODO: New separator is selected from right subtree. Check if we should consider about left subtree as well.
				BNode* leftMostLeafNodeOfRightSubtree = rightChildOfKey->getLeftMostLeafNode();
				T newSeparator = leftMostLeafNodeOfRightSubtree->keys->getSmallestKey();

				// BNode* rightMostLeafNodeOfLeftSubtree = leftChildOfKey->getRightMostLeafNode();
				// T newSeparator = rightMostLeafNodeOfLeftSubtree->keys->getLargestKey();

				// TODO: Reconsider using this function
				keys->setKeyByIndex(newSeparator, indexOfKey);

				deletionResult = leftMostLeafNodeOfRightSubtree->remove(newSeparator);
				// deletionResult = rightMostLeafNodeOfLeftSubtree->remove(newSeparator);
			}

			else {
				BNode* child = getChildByIndex(indexOfKey);
				deletionResult = child->remove(key);
			}
		}

		return deletionResult;
	}

	BNode* getLeftMostLeafNode(void) {
		BNode* currentNode = this;

		while (currentNode->isLeaf == false) {
			currentNode = currentNode->getLeftMostChild();
		}

		return currentNode;
	}

	BNode* getRightMostLeafNode(void) {
		BNode* currentNode = this;

		while (currentNode->isLeaf == false) {
			currentNode = currentNode->getRightMostChild();
		}

		return currentNode;
	}

	BNode* getLeftMostChild(void) {
		return children[0];
	}

	BNode* getRightMostChild(void) {
		return children[keys->getCurrentSize()];
	}

	void rebalance(void) {
		BNode* leftSibling = getLeftSibling();
		BNode* rightSibling = getRightSibling();

		if (leftSibling != nullptr && leftSibling->isExceedingNode()) {
			rightRotation();
		}
		else if (rightSibling != nullptr && rightSibling->isExceedingNode()) {
			leftRotation();
		}
		else {
			mergeWithSiblingNode();
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

	void leftRotation(void) {
		size_t separatorIndex = childIndex;
		T oldSeparator = parent->keys->getKeyByIndex(separatorIndex);

		keys->insert(oldSeparator);

		BNode* rightSibling = getRightSibling();
		T newSeparator = rightSibling->keys->getSmallestKey();
		rightSibling->keys->remove(newSeparator);

		//TODO: Reconsider using this function
		parent->keys->setKeyByIndex(newSeparator, separatorIndex);
	}

	void rightRotation(void) {
		size_t separatorIndex = childIndex - 1;
		T oldSeparator = parent->keys->getKeyByIndex(separatorIndex);

		keys->insert(oldSeparator);

		BNode* leftSibling = getLeftSibling();
		T newSeparator = leftSibling->keys->getLargestKey();
		leftSibling->keys->remove(newSeparator);

		//TODO: Reconsider using this function
		parent->keys->setKeyByIndex(newSeparator, separatorIndex);
	}

	void mergeTwoChildrenNode(size_t leftChildNodeIndex, size_t rightChildNodeIndex) {
		// Warning: leftNodeIndex + 1 == rightNodeIndex should be true
		BNode* leftChildNode = getChildByIndex(leftChildNodeIndex);
		BNode* rightChildNode = getChildByIndex(rightChildNodeIndex);

		size_t numberOfChildrenOfLeftChildNode = leftChildNode->keys->getCurrentSize() + 1;
		size_t numberOfChildrenOfRightChildNode = rightChildNode->keys->getCurrentSize() + 1;

		size_t separatorIndex = leftChildNodeIndex;
		T separator = keys->getKeyByIndex(separatorIndex);

		keys->remove(separator);
		leftChildNode->keys->insert(separator);

		leftChildNode->keys->mergeWithOtherBKeyList(rightChildNode->keys);
		for (size_t i = keys->getCurrentSize() + 1; i > separatorIndex + 1; i--) {
			BNode* childToShift = getChildByIndex(i);
			setChildByIndex(childToShift, i - 1);
		}
		
		// TODO: Revise this condition statement
		if (!leftChildNode->isLeaf && !rightChildNode->isLeaf) {
			for (size_t i = 0; i < numberOfChildrenOfRightChildNode; i++) {
				BNode* childToShift = rightChildNode->getChildByIndex(i);
				leftChildNode->setChildByIndex(childToShift, numberOfChildrenOfLeftChildNode + i);
				rightChildNode->setChildByIndex(nullptr, i);
			}
		}

		delete rightChildNode;

		if (hasParent() && isDeficientNode()) {
			rebalance();
		}
	}

	void mergeWithSiblingNode(void) {
		if (childIndex < parent->keys->getCurrentSize()) {
			parent->mergeTwoChildrenNode(childIndex, childIndex + 1);
		}
		else {
			parent->mergeTwoChildrenNode(childIndex - 1, childIndex);
		}
	}

	std::string preOrder(bool useBrackets = false) {
		std::stringstream ss;
		preOrder(ss, useBrackets);
		return ss.str();
	}

	std::string inOrder(void) {
		std::stringstream ss;
		inOrder(ss);
		return ss.str();
	}

	std::string postOrder(bool useBrackets = false) {
		std::stringstream ss;
		postOrder(ss, useBrackets);
		return ss.str();
	}

	bool hasParent(void) { 
		return parent != nullptr; 
	}

	BNode* getParent(void) { 
		return parent; 
	}

	bool isEmpty(void) {
		return keys->isEmpty();
	}
};

#endif // !__B_NODE__
