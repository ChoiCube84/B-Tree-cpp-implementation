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
void insertAndPrint(BTree<T>* tree, T key, bool print);
template <typename T>
void deleteAndPrint(BTree<T>* tree, T key, bool print);

void BKeyListTest(void);
void BTreeTest(bool print);
void RandomBTreeTest(bool print);

int main(void) {
	// BKeyListTest();
	// BTreeTest(true);
	RandomBTreeTest(false);

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
void insertAndPrint(BTree<T>* tree, T key, bool print) {
	if (print) cout << "Before Insert (" << key << ") : " << tree->preOrder(true) << endl;
	tree->insert(key);
	if (print) {
		cout << "After Insert (" << key << ") : " << tree->preOrder(true) << endl;
		cout << endl;
	}
}

template <typename T>
void deleteAndPrint(BTree<T>* tree, T key, bool print) {
	if (print) cout << "Before deletion (" << key << ") : " << tree->preOrder(true) << endl;

	bool deletionSuccess = tree->remove(key);
	
	if (print) {
		if (deletionSuccess) {
			std::cout << "Deletion Success" << std::endl;
		}
		else {
			std::cout << "Deletion Failed" << std::endl;
		}
	}
	
	if (print) {
		cout << "After deletion (" << key << ") : " << tree->preOrder(true) << endl;
		cout << endl;
	}	
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

void BTreeTest(bool print) {
	BTree<int>* tree = new BTree<int>(3);
	vector<int> keys;

	cout << "========== Insert test start ==========" << endl;
	keys = { 7155, 8120, 6557, 1275, 2773, 4782, 2806, 7294, 9073 };
	for (auto i : keys) {
		insertAndPrint<int>(tree, i, print);
	}
	cout << "========== Insert test done! ==========" << endl;

	cout << endl;

	cout << "========== Delete test start ==========" << endl;
	keys = { 1275, 9073, 2806, 6557, 2773, 7294, 8120, 7155, 4782 };
	for (auto i : keys) {
		deleteAndPrint<int>(tree, i, print);
	}
	cout << "========== Delete test done! ==========" << endl;

	cout << endl;

	delete tree;
}

void RandomBTreeTest(bool print) {
	unsigned int success = 0;
	unsigned int objective = 1000;

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<int> randomOrder(3, 10);
	std::uniform_int_distribution<int> randomKey(0, 9999);
	std::uniform_int_distribution<int> randomDepth(2, 4);
	
	vector<int> keys;

	while (success < objective) {
		size_t order = static_cast<size_t>(randomOrder(gen));
		BTree<int>* tree = new BTree<int>(order);
		
		int depth = randomDepth(gen);

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
			insertAndPrint(tree, i, print);
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
			deleteAndPrint(tree, i, print);
		}
		if (print) cout << "========== Delete test done! ==========" << endl;

		if (print) cout << endl;

		keys.clear();
		success++;

		cout << "Test " << success << " success! (order: " << order << ")" << endl;

		if (print) cout << endl;

		delete tree;
	}

	cout << "ALL CLEAR!" << endl;
}