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
void RandomBTreeTest(bool print);

int main(void) {
	// BKeyListTest();
	// BNodeTest();
	BTreeTest();
	// RandomBTreeTest(false);

	return 0;
}

template <typename T>
void insertAndPrint(BKeyList<T>* list, T key) {
	cout << "Inserting " << key << endl;
	cout << "Before Insert (" << key << ") : " << list->traverse() << endl;
	list->insert(key);
	cout << "After Insert (" << key << ") : " << list->traverse() << endl;
	cout << endl;
}

template <typename T>
void deleteAndPrint(BKeyList<T>* list, T key) {
	cout << "Before deletion (" << key << ") : " << list->traverse() << endl;
	
	bool deletionSuccess = list->remove(key);
	if (deletionSuccess) {
		std::cout << "Deletion Success" << std::endl;
	}
	else {
		std::cout << "Deletion Failed" << std::endl;
	}

	cout << "After deletion (" << key << ") : " << list->traverse() << endl;
	cout << endl;
}

template <typename T>
void insertAndPrint(BTree<T>* tree, T key) {
	cout << "Before Insert (" << key << ") : " << tree->preOrder(true) << endl;
	tree->insert(key);
	cout << "After Insert (" << key << ") : " << tree->preOrder(true) << endl;
	cout << endl;
}

template <typename T>
void deleteAndPrint(BTree<T>* tree, T key) {
	cout << "Before deletion (" << key << ") : " << tree->preOrder(true) << endl;

	bool deletionSuccess = tree->remove(key);
	if (deletionSuccess) {
		std::cout << "Deletion Success" << std::endl;
	}
	else {
		std::cout << "Deletion Failed" << std::endl;
	}

	cout << "After deletion (" << key << ") : " << tree->preOrder(true) << endl;
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
	vector<int> keys;

	cout << "========== Insert test start ==========" << endl;
	keys = { 5003, 6072, 3462, 4435, 5497, 9870, 5346, 8118, 2812, 3598, 4703, 1856, 7733, 6252, 9125, 7921, 4500, 3301, 5416, 7451, 7263, 3812, 5934, 2965, 3622 };
	for (auto i : keys) {
		insertAndPrint<int>(tree, i);
	}
	cout << "========== Insert test done! ==========" << endl;

	cout << endl;

	cout << "========== Delete test start ==========" << endl;
	keys = { 9870, 6252, 6072, 8118, 5497, 3598, 7451, 2812, 5346, 2965, 4500, 7263, 3812, 5003, 4435, 3622, 7733, 3301, 9125, 1856, 3462, 7921, 5934, 4703, 5416 };
	for (auto i : keys) {
		deleteAndPrint<int>(tree, i);
	}
	cout << "========== Delete test done! ==========" << endl;

	cout << endl;

	delete tree;
}

void RandomBTreeTest(bool print) {
	unsigned int success = 0;
	unsigned int objective = 100000;

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<int> randomOrder(3, 7);
	std::uniform_int_distribution<int> randomKey(0, 9999);

	vector<int> keys;

	while (success < objective) {
		size_t order = static_cast<size_t>(randomOrder(gen));
		order = 3; // TODO: delete this
		BTree<int>* tree = new BTree<int>(order);
		
		int depth = 2;

		for (size_t i = 0; i < pow(order, depth); i++) {
			int key = randomKey(gen);
			keys.emplace_back(key);
		}

		if (print) {
			cout << "Test case: ";
			for (auto i : keys) {
				cout << i << ", ";
			}
			cout << endl;
			cout << endl;
		}
		

		if (print) cout << "========== Insert test start ==========" << endl;
		for (auto i : keys) {
			if (print) {
				insertAndPrint(tree, i);
			}
			else {
				tree->insert(i);
			}
		}
		if (print) cout << "========== Insert test done! ==========" << endl;

		if (print) cout << endl;

		random_shuffle(keys.begin(), keys.end());

		if (print) {
			cout << "Test case: ";
			for (auto i : keys) {
				cout << i << ", ";
			}
			cout << endl;
			cout << endl;
		}

		if (print) cout << "========== Delete test start ==========" << endl;
		for (auto i : keys) {
			if (print) {
				deleteAndPrint(tree, i);
			}
			else {
				tree->remove(i);
			}
		}
		if (print) cout << "========== Delete test done! ==========" << endl;

		if (print) cout << endl;

		keys.clear();
		success++;

		if (print) cout << "Test " << success << " success! (order: " << order << ")" << endl;

		if (print) cout << endl;

		delete tree;
	}

	cout << "ALL CLEAR!" << endl;
}