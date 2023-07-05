#include <iostream>
#include <vector>
#include "b_node.h"

using namespace std;

template <typename T>
void insertAndPrint(BNode<T>* node, T key) {
	cout << "Current status: " << node->preOrder() << endl;
	node->insert(key);
	cout << "After inserting " << key << ": " << node->preOrder() << endl << endl;
}

int main(void) {
	BNode<int>* node = new BNode<int>(5, nullptr, true);
	vector<int> keys = { 4, 7, 3, 6, 2, 5, 1, 9, 10, 8, 11 };

	for (auto i : keys) {
		insertAndPrint<int>(node, i);
	}

	delete node;

	return 0;
}