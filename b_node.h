#ifndef __B_NODE__
#define __B_NODE__

#include <string>

#include "b_tree_constraints.h"
#include "b_key_list.h"

template <UsableInBTree T>
class BNode
{
public: // Public member variable
	const size_t order;
	const bool isLeaf;

private: // Private member variable
	size_t childIndex;
	BNode* parent; 
	BKeyList<T>* keys;
	BNode** children;
	
public: // Public primary functions
	BNode(size_t order, bool isLeaf = false) 
		: order(ensureValidOrder(order)), isLeaf(isLeaf), childIndex(0), parent(nullptr), keys(new BKeyList<T>(order)), children(nullptr)
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
			
			BNode* tail = new BNode(order, isLeaf);
			splitBNode(tail);
			
			if (!hasParent()) {
				makeNewRootNode(tail);
			}
			
			parent->keys->insert(promotedKey);
			connectTailNodeToParentNode(tail, promotedKey);
		}
	}

	void remove(const T& key) {
		if (isLeaf) {
			keys->remove(key);
			
			if (isDeficientNode()) {
				rebalance();
			}
		}
		else {
			size_t indexOfKey = keys->findIndex(key);
			bool isExistingkey = keys->search(key);

			if (isExistingkey) {
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

				selectedLeafNode->remove(newSeparator);
			}

			else {
				BNode* child = getChildByIndex(indexOfKey);
				child->remove(key);
			}
		}
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

public: // Public helper functions
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

private: // Private helper functions
	void setChildByIndex(BNode* child, size_t idx) {
		if (child != nullptr) {
			child->parent = this;
			child->childIndex = idx;
		}

		children[idx] = child;
	}

	void splitBNode(BNode* tail) {
		tail->childIndex = childIndex + 1;
		
		tail->keys = new BKeyList<T>(order);
		keys->splitBKeyList(tail->keys);
		
		tail->parent = parent;

		if (!isLeaf) {
			tail->children = new BNode * [order + 1];
			for (size_t i = 0; i < order + 1; i++) {
				tail->setChildByIndex(nullptr, i);
			}
			passChildrenToTailNode(tail);
		}
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

		parent->shiftChildrenToRight(indexOfPromotedKey + 1);
		parent->setChildByIndex(tail, indexOfPromotedKey + 1);
	}

	void shiftChildrenToLeft(size_t indexToBegin) {
		for (size_t i = indexToBegin; i < getCurrentSize(); i++) {
			BNode* childToShift = getChildByIndex(i + 1);
			setChildByIndex(childToShift, i);
		}
		setChildByIndex(nullptr, getCurrentSize());
	}

	void shiftChildrenToRight(size_t indexToBegin) {
		for (size_t i = getCurrentSize(); i > indexToBegin; i--) {
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
			return children[getCurrentSize()];
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
			size_t numberOfChildrenNodes = getCurrentSize() + 1;
			size_t minimumNumberOfRequiredKeys = (order + 1) / 2;

			return numberOfChildrenNodes < minimumNumberOfRequiredKeys;
		}
	}

	bool isExceedingNode(void) {
		size_t numberOfChildrenNodes = getCurrentSize() + 1;
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
		if (childIndex == parent->getCurrentSize()) {
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

		if (!isLeaf) {
			BNode* leftMostChildOfRightSibling = rightSibling->getLeftMostChild();
			setChildByIndex(leftMostChildOfRightSibling, getCurrentSize());
			rightSibling->shiftChildrenToLeft(0);
		}
		
		parent->keys->remove(oldSeparator);
		parent->keys->insert(newSeparator);

		rightSibling->keys->remove(newSeparator);
	}

	void rightRotation(void) {
		size_t separatorIndex = childIndex - 1;
		T oldSeparator = parent->keys->getKeyByIndex(separatorIndex);

		keys->insert(oldSeparator);

		BNode* leftSibling = getLeftSibling();
		T newSeparator = leftSibling->keys->getLargestKey();

		if (!isLeaf) {
			BNode* rightMostChildOfLeftSibling = leftSibling->getRightMostChild();
			
			shiftChildrenToRight(0);
			setChildByIndex(rightMostChildOfLeftSibling, 0);
			
			leftSibling->setChildByIndex(nullptr, leftSibling->getCurrentSize());
		}

		parent->keys->remove(oldSeparator);
		parent->keys->insert(newSeparator);

		leftSibling->keys->remove(newSeparator);
	}

	void mergeWithSiblingNode(void) {
		size_t separatorIndex = 0;

		if (childIndex < parent->getCurrentSize()) {
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

		size_t numberOfChildrenOfLeftChildNode = leftChildNode->getCurrentSize() + 1;
		size_t numberOfChildrenOfRightChildNode = rightChildNode->getCurrentSize() + 1;

		T separator = keys->getKeyByIndex(separatorIndex);

		keys->remove(separator);
		leftChildNode->keys->insert(separator);

		leftChildNode->keys->mergeBKeyList(rightChildNode->keys);
		
		for (size_t i = separatorIndex + 2; i < getCurrentSize() + 2; i++) {
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
};

#endif // !__B_NODE__
