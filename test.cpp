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
void insertAndPrint(BTree<T>* tree, T key);
template <typename T>
void deleteAndPrint(BTree<T>* tree, T key);

void BKeyListTest(void);
void BTreeTest(void);
void RandomBTreeTest(void);

int main(void) {
	// BKeyListTest();
	// BNodeTest();
	// BTreeTest();
	RandomBTreeTest();

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
void insertAndPrint(BTree<T>* tree, T key) {
	cout << "Inserting " << key << endl;
	cout << "Before Insert: " << tree->preOrder(true) << endl;
	tree->insert(key);
	cout << "After Insert: " << tree->preOrder(true) << endl;
	cout << endl;
}

template <typename T>
void deleteAndPrint(BTree<T>* tree, T key) {
	cout << "Before deletion: " << tree->preOrder(true) << endl;

	bool deletionSuccess = tree->remove(key);
	if (deletionSuccess) {
		std::cout << "Deletion Success" << std::endl;
	}
	else {
		std::cout << "Deletion Failed" << std::endl;
	}

	cout << "After deletion: " << tree->preOrder(true) << endl;
	cout << endl;
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

void BTreeTest(void) {
	BTree<int>* tree = new BTree<int>(5);
	vector<int> keys = { 4, 7, 3, 6, 2, 5, 1, 9, 10, 8, 11, 13, 12, 14, 15, 16, 17, 18, 19 };

	cout << "========== Insert test start ==========" << endl;
	for (auto i : keys) {
		insertAndPrint<int>(tree, i);
	}
	cout << "========== Insert test done! ==========" << endl;

	cout << endl;

	cout << "========== Delete test start ==========" << endl;
	keys = { 20, 5, 15, 1, 19, 8, 9, 13, 10, 4, 16, 2, 11, 3, 12, 14, 7, 6, 18, 17, 17 };
	for (auto i : keys) {
		deleteAndPrint<int>(tree, i);
	}
	cout << "========== Delete test done! ==========" << endl;

	delete tree;
}

void RandomBTreeTest(void) {
	unsigned int success = 0;

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<int> randomOrder(3, 7);
	std::uniform_int_distribution<int> randomKey(0, 9999);

	vector<int> keys;

	while (1) {
		size_t order = static_cast<size_t>(randomOrder(gen));
		BTree<int>* tree = new BTree<int>(order);

		for (size_t i = 0; i < pow(order, 3); i++) {
			int key = randomKey(gen);
			keys.emplace_back(key);
		}

		for (auto i : keys) {
			insertAndPrint(tree, i);
			// tree->insert(i);
		}

		random_shuffle(keys.begin(), keys.end());

		for (auto i : keys) {
			deleteAndPrint(tree, i);
			// tree->remove(i);
		}

		keys.clear();
		success++;

		cout << "Test " << success << "success! (order: " << order << ")" << endl;

		delete tree;
	}

	
}