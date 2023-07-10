#include <vector>
#include <random>
#include <string>
#include <iostream>

#include "b_key_list.h"
#include "b_node.h"
#include "b_tree.h"

using namespace std;

template <typename T>
void insertAndPrint(BKeyList<T>* list, T key);
template <typename T>
void deleteAndPrint(BKeyList<T>* list, T key);
template <typename T>
void insertAndPrint(BNode<T>* node, T key);
template <typename T>
void deleteAndPrint(BNode<T>* node, T key);
template <typename T>
void insertAndPrint(BTree<T>* tree, T key);
template <typename T>
void deleteAndPrint(BTree<T>* tree, T key);

void BKeyListTest(void);
void BNodeTest(void);
void BTreeTest(void);

int main(void) {
	// BKeyListTest();
	BNodeTest();
	// BTreeTest();

	return 0;
}

template <typename T>
void insertAndPrint(BKeyList<T>* list, T key) {
	cout << "Inserting " << key << endl;
	cout << "Before Insert: " << list->traverse() << endl;
	list->insert(key);
	cout << "After Insert: " << list->traverse() << endl;
	cout << endl;
}

template <typename T>
void deleteAndPrint(BKeyList<T>* list, T key) {
	cout << "Before deletion: " << list->traverse() << endl;
	
	bool deletionSuccess = list->remove(key);
	if (deletionSuccess) {
		std::cout << "Deletion Success" << std::endl;
	}
	else {
		std::cout << "Deletion Failed" << std::endl;
	}

	cout << "After deletion: " << list->traverse() << endl;
	cout << endl;
}

template <typename T>
void insertAndPrint(BNode<T>* node, T key) {
	cout << "Inserting " << key << endl;
	cout << "Before Insert: " << node->preOrder(true) << endl;
	node->insert(key);
	cout << "After Insert: " << node->preOrder(true) << endl;
	cout << endl;
}

template <typename T>
void deleteAndPrint(BNode<T>* node, T key) {
	// TODO: Make the test code
}

template <typename T>
void insertAndPrint(BTree<T>* tree, T key) {
	// TODO: Make the test code
}

template <typename T>
void deleteAndPrint(BTree<T>* tree, T key) {
	// TODO: Make the test code
}

void BKeyListTest(void) {
	BKeyList<int>* first = new BKeyList<int>(11);
	vector<int> keys = { 4, 7, 3, 6, 2, 5, 1, 9, 10, 8, 11 };

	cout << "========== Insert test start ==========" << endl;
	for (auto i : keys) {
		insertAndPrint<int>(first, i);
	}
	cout << "========== Insert test done! ==========" << endl;

	cout << endl;

	cout << "========== Split test start ==========" << endl;
	BKeyList<int>* second = first->split();
	cout << first->traverse() << " | " << second->traverse() << endl;
	delete second;
	cout << "========== Split test done! ==========" << endl;

	cout << endl;

	cout << "========== Delete test start ==========" << endl;
	keys = { 3, 2, 5, 5, 1, 4 };
	for (auto i : keys) {
		deleteAndPrint<int>(first, i);
	}
	cout << "========== Delete test done! ==========" << endl;

	delete first;
}

void BNodeTest(void) {
	BNode<int>* node = new BNode<int>(5, 0, true);
	vector<int> keys = { 4, 7, 3, 6, 2, 5, 1, 9, 10, 8, 11, 13, 12, 14, 15, 16, 17, 18, 19 };

	cout << "========== Insert test start ==========" << endl;
	for (auto i : keys) {
		if (node->hasParent()) {
			cout << "Parent Detected!" << endl;
			node = node->getParent();
			
			cout << "Current Status: " << node->preOrder() << endl;

			cout << endl;
		}
		insertAndPrint<int>(node, i);
	}
	cout << "========== Insert test done! ==========" << endl;

	delete node;
}

void BTreeTest(void) {
	BTree<int> tree(3);
	cout << "Hello B Tree!" << endl;
}