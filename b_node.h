#ifndef __B_NODE__
#define __B_NODE__

#include <iostream>
#include <string>

#include "b_key_list.h"

template <typename T>
class BNode
{
public:
	const size_t order;
	const bool isLeaf;

private:
	size_t childIndex;
	BNode* parent;
	BKeyList<T>* keys;
	BNode** children;

public:
	BNode(size_t order, bool isLeaf = false) 
		: order(order), childIndex(childIndex), isLeaf(isLeaf), keys(new BKeyList<T>(order)), parent(nullptr), children(nullptr)
	{
		if (!isLeaf) {
			children = new BNode*[order + 1];

			for (size_t i = 0; i < order + 1; i++) {
				setChildByIndex(nullptr, i);
			}	
		}
	}

	~BNode()
	{
		delete keys;
		delete[] children;
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

				BNode* leftMostLeafNodeOfRightSubtree = rightChildOfKey->getLeftMostLeafNode();
				BNode* rightMostLeafNodeOfLeftSubtree = leftChildOfKey->getRightMostLeafNode();

				BNode* selectedLeafNode = nullptr;
				T newSeparator;

				if (leftMostLeafNodeOfRightSubtree->getCurrentSize() >= rightMostLeafNodeOfLeftSubtree->getCurrentSize()) {
					newSeparator = leftMostLeafNodeOfRightSubtree->keys->getSmallestKey();
					selectedLeafNode = leftMostLeafNodeOfRightSubtree;
				}
				else {
					newSeparator = rightMostLeafNodeOfLeftSubtree->keys->getLargestKey();
					selectedLeafNode = rightMostLeafNodeOfLeftSubtree;
				}

				keys->remove(key);
				keys->insert(newSeparator);

				deletionResult = selectedLeafNode->remove(newSeparator);
			}

			else {
				BNode* child = getChildByIndex(indexOfKey);
				deletionResult = child->remove(key);
			}
		}

		return deletionResult;
	}

	bool search(const T& key) {
		size_t indexOfKey = keys->findIndex(key);
		bool searchResult = false;

		if (isLeaf) {
			searchResult = keys->search(key);
		}
		else {
			BNode* child = getChildByIndex(indexOfKey);
			searchResult = child->search(key);
		}

		return searchResult;
	}

	std::string traverse(void) {
		return keys->traverse();
	}

private:
	void setChildByIndex(BNode* child, size_t idx) {
		if (child != nullptr) {
			child->parent = this;
			child->childIndex = idx;
		}

		children[idx] = child;
	}

	BNode* splitTailNode(void) {
		BNode* tail = new BNode(order, isLeaf);

		tail->childIndex = childIndex + 1;
		tail->keys = keys->split();
		tail->parent = parent;

		if (!tail->isLeaf) {
			tail->children = new BNode * [order + 1];
			for (size_t i = 0; i < order + 1; i++) {
				tail->setChildByIndex(nullptr, i);
			}
			passChildrenToTailNode(tail);
		}

		return tail;
	}

	void passChildrenToTailNode(BNode* tail) {
		for (size_t i = order / 2 + 1; i < order + 1; i++) {
			BNode* childToPass = getChildByIndex(i);
			tail->setChildByIndex(childToPass, i - (order / 2 + 1));
			setChildByIndex(nullptr, i);
		}
	}

	void makeNewRootNode(BNode* tail) {
		setParent(new BNode(order));
		tail->setParent(parent);

		parent->setChildByIndex(this, 0);
		parent->setChildByIndex(tail, 1);

		for (size_t i = 2; i < order + 1; i++) {
			parent->setChildByIndex(nullptr, i);
		}
	}

	void connectTailNodeToParentNode(BNode* tail, T& promotedKey) {
		size_t indexOfPromotedKey = parent->keys->findIndex(promotedKey);

		parent->shiftChildren(indexOfPromotedKey);
		parent->setChildByIndex(tail, indexOfPromotedKey + 1);
	}

	void shiftChildren(size_t indexOfPromotedKey) {
		for (size_t i = keys->getCurrentSize(); i > indexOfPromotedKey + 1; i--) {
			BNode* childToShift = getChildByIndex(i - 1);
			setChildByIndex(childToShift, i);
		}
	}

	void setParent(BNode* newParent) {
		parent = newParent;
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
		if (isLeaf) {
			return nullptr;
		}
		else {
			return children[0];
		}
	}

	BNode* getRightMostChild(void) {
		if (isLeaf) {
			return nullptr;
		}
		else {
			return children[keys->getCurrentSize()];
		}
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

	bool isDeficientNode(void) {
		if (!hasParent()) {
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

		parent->keys->remove(oldSeparator);
		parent->keys->insert(newSeparator);
	}

	void rightRotation(void) {
		size_t separatorIndex = childIndex - 1;
		T oldSeparator = parent->keys->getKeyByIndex(separatorIndex);

		keys->insert(oldSeparator);

		BNode* leftSibling = getLeftSibling();
		T newSeparator = leftSibling->keys->getLargestKey();
		leftSibling->keys->remove(newSeparator);

		parent->keys->remove(oldSeparator);
		parent->keys->insert(newSeparator);
	}

	void mergeWithSiblingNode(void) {
		size_t separatorIndex = 0;

		if (childIndex < parent->keys->getCurrentSize()) {
			separatorIndex = childIndex;
		}
		else {
			separatorIndex = childIndex - 1;
		}

		parent->mergeTwoChildrenNode(separatorIndex);
	}

	void mergeTwoChildrenNode(size_t separatorIndex) {
		size_t leftChildNodeIndex = separatorIndex;
		size_t rightChildNodeIndex = separatorIndex + 1;

		BNode* leftChildNode = getChildByIndex(leftChildNodeIndex);
		BNode* rightChildNode = getChildByIndex(rightChildNodeIndex);

		size_t numberOfChildrenOfLeftChildNode = leftChildNode->keys->getCurrentSize() + 1;
		size_t numberOfChildrenOfRightChildNode = rightChildNode->keys->getCurrentSize() + 1;

		T separator = keys->getKeyByIndex(separatorIndex);

		keys->remove(separator);
		leftChildNode->keys->insert(separator);

		leftChildNode->keys->mergeWithOtherBKeyList(rightChildNode->keys);
		for (size_t i = keys->getCurrentSize() + 1; i > separatorIndex + 1; i--) {
			BNode* childToShift = getChildByIndex(i);
			setChildByIndex(childToShift, i - 1);
		}

		bool isEitherChildLeaf = leftChildNode->isLeaf || rightChildNode->isLeaf;

		if (!isEitherChildLeaf) {
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

public:
	bool hasParent(void) { 
		return parent != nullptr; 
	}

	bool isEmpty(void) {
		return keys->isEmpty();
	}

	size_t getCurrentSize(void) {
		return keys->getCurrentSize();
	}

	BNode* getParent(void) { 
		return parent; 
	}

	BNode* getChildByIndex(size_t idx) {
		return children[idx];
	}	

	BNode* replaceRootNode(void) {
		BNode* newRootNode = getLeftMostChild();

		if (newRootNode == nullptr) {
			newRootNode = new BNode(order, true);
		}
		newRootNode->parent = nullptr;

		delete this;
		return newRootNode;
	}
};

#endif // !__B_NODE__
