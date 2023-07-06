#ifndef __B_TREE__
#define __B_TREE__

#include <iostream>
#include <string>
#include <sstream>

#include "b_node.h"

template <typename T>
class BTree
{
private:
	size_t order;
	BNode<T>* root;

	std::string preOrder(BNode<T>* curr) {
		std::stringstream ss;

		if (curr != nullptr) {
			ss << curr->getKeys()->traverse();
			for (size_t i = 0; i < order; i++) {
				BNode<T>* temp = curr->getChildren()[i];
				if (temp != nullptr) ss << preOrder(temp);
			}
		}
		
		return ss.str();
	}

	std::string inOrder(BNode<T>* curr) {
		std::stringstream ss;
		
		size_t currentSize = curr->getKeys()->getCurrentSize();
		for (size_t i = 0; i < currentSize; i++) {
			BNode<T>* temp = curr->getChildren()[i];

			if (temp != nullptr) ss << inOrder(temp);
			ss << curr->getKeys()[i];
		}

		return ss.str();
	}

	std::string postOrder(BNode<T>* curr) {
		std::stringstream ss;

		if (curr != nullptr) {
			for (size_t i = 0; i < order; i++) {
				BNode<T>* temp = curr->getChildren()[i];
				if (temp != nullptr) ss << postOrder(temp);
			}
			ss << curr->getKeys()->traverse();
		}

		return ss.str();
	}

public:
	BTree(size_t order) : order(order), root(new BNode<T>(order, nullptr, true))
	{
		std::cout << "B Tree was made" << std::endl;
	}
	~BTree() {
		delete root;
		std::cout << "B Tree was destructed" << std::endl;
	}

	void insert(const T& data) {
		std::cout << "Not implemented yet" << std::endl;
	}

	void remove(const T& data) {
		std::cout << "Not implemented yet" << std::endl;
	}

	bool find(const T& data) {
		std::cout << "Not implemented yet" << std::endl;
		return false;
	}
	
	std::string preOrder() {
		std::string result = preOrder(root);
		if (!result.empty()) result.pop_back();
		return result;
	}

	std::string inOrder() {
		std::string result = inOrder(root);
		if (!result.empty()) result.pop_back();
		return result;
	}

	std::string postOrder() {
		std::string result = postOrder(root);
		if (!result.empty()) result.pop_back();
		return result;
	}
	
};

#endif // !__B_TREE__