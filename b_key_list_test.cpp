#include <iostream>
#include <vector>

#include "b_key_list.h"

using namespace std;

template <typename T>
void insertAndPrint(BKeyList<T>* list, T key) {
	cout << "Current status: " << list->traverse() << endl;
	list->insert(key);
	
	if (list->splitRequired()) {
		cout << "Split Required" << endl;
	}

	cout << endl;
}

template <typename T>
void deleteAndPrint(BKeyList<T>* list, T key) {
	cout << "Current status: " << list->traverse() << endl;

	int check = list->remove(key);
	
	if (check == -1) {
		std::cout << "Deletion Success" << std::endl;
	}
	else {
		std::cout << "Deletion Failed" << std::endl;
	}
	
	cout << endl;
}

int main(void) {

	vector<int> keys = { 4, 7, 3, 6, 2, 5, 1, 9, 10, 8, 11 };
	BKeyList<int>* first = new BKeyList<int>(keys.size());

	for (auto i : keys) {
		insertAndPrint<int>(first, i);
	}

	cout << "Insert complete" << endl;

	BKeyList<int>* second = first->split();
	cout << first->traverse() << " | " << second->traverse() << endl;

	delete second;

	keys = { 3, 2, 5, 5, 1, 4 };

	for (auto i : keys) {
		deleteAndPrint<int>(first, i);
	}

	delete first;

	return 0;
}