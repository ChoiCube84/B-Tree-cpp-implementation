#ifndef __B_TREE__
#define __B_TREE__

#include <iostream>
#include <string>
#include <functional>

#include "b_node.h"

template <typename T>
class BTree
{
private:
	const size_t order;
	BNode<T>* root;

	void destructorHelper(BNode<T>* currentNode) {
		if (!currentNode->isLeaf) {
			for (size_t i = 0; i < currentNode->getCurrentSize(); i++) {
				BNode<T>* child = currentNode->getChildByIndex(i);
				destructorHelper(child);
			}
		}
		delete currentNode;
	}

	void traverseChildNode(
		std::stringstream& ss, 
		std::function<void(std::stringstream&, BNode<T>*, bool)> orderType,
		BNode<T>* child, 
		bool useBrackets) 
	{
		ss << " ";

		if (useBrackets) {
			ss << "(";
		}

		orderType(ss, child, useBrackets);

		if (useBrackets) {
			ss << ")";
		}
	}

	void preOrderHelper(std::stringstream& ss, BNode<T>* currentNode, bool useBrackets) {
		BNode<T>* child = nullptr;

		ss << currentNode->traverse();

		if (!currentNode->isLeaf) {
			for (size_t i = 0; i < currentNode->getCurrentSize() + 1; i++) {
				child = currentNode->getChildByIndex(i);
				traverseChildNode(
					ss, 
					[this](std::stringstream& ss, BNode<T>* currentNode, bool useBrackets) {
						this->preOrderHelper(ss, currentNode, useBrackets);
					},
					child, 
					useBrackets
				);
			}
		}
	}

	void inOrderHelper(std::stringstream& ss, BNode<T>* currentNode) {
		BNode<T>* child = nullptr;

		if (currentNode->isLeaf) {
			ss << currentNode->traverse();
		}
		else {
			for (size_t i = 0; i < currentNode->getCurrentSize(); i++) {
				child = currentNode->getChildByIndex(i);
				inOrderHelper(ss, child);
				ss << " " << currentNode->getKeyByIndex(i) << " ";
			}
			child = currentNode->getChildByIndex(currentNode->getCurrentSize());
			inOrderHelper(ss, child);
		}
	}

	void postOrderHelper(std::stringstream& ss, BNode<T>* currentNode, bool useBrackets) {
		BNode<T>* child = nullptr;

		if (!currentNode->isLeaf) {
			for (size_t i = 0; i < currentNode->getCurrentSize() + 1; i++) {
				child = currentNode->getChildByIndex(i);
				traverseChildNode(
					ss,
					[this](std::stringstream& ss, BNode<T>* currentNode, bool useBrackets) {
						this->postOrderHelper(ss, currentNode, useBrackets);
					},
					child,
					useBrackets
				);
			}
		}

		ss << currentNode->traverse();
	}

public:
	BTree(size_t order) : order(order), root(new BNode<T>(order, true)) {
	}

	~BTree() {
		destructorHelper(root);
	}

	void insert(const T& key) {
		root->insert(key);
		if (root->hasParent()) {
			root = root->getParent();
		}
	}

	bool remove(const T& key) {
		bool deletionResult = root->remove(key);

		if (root->isEmpty()) {
			root = root->replaceRootNode();
		}

		return deletionResult;
	}

	bool find(const T& key) {
		bool findResult = root->find(key);
		return findResult;
	}

	std::string preOrder(bool useBrackets = false) {
		std::stringstream ss;
		preOrderHelper(ss, root, useBrackets);
		return ss.str();
	}

	std::string inOrder(void) {
		std::stringstream ss;
		inOrderHelper(ss, root);
		return ss.str();
	}

	std::string postOrder(bool useBrackets = false) {
		std::stringstream ss;
		postOrderHelper(ss, root, useBrackets);
		return ss.str();
	}
};

#endif // !__B_TREE__