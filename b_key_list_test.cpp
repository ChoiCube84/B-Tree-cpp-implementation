#include <iostream>
#include "b_key_list.h"

using namespace std;

void printStatus(BKeyList<int>* list);
void insertAndPrint(BKeyList<int>* list, int key);

int main(void) {
	BKeyList<int>* first = new BKeyList<int>(10);

	insertAndPrint(first, 4);
	insertAndPrint(first, 7);
	insertAndPrint(first, 3);
	insertAndPrint(first, 6);
	insertAndPrint(first, 2);
	insertAndPrint(first, 5);
	insertAndPrint(first, 1);
	insertAndPrint(first, 9);
	insertAndPrint(first, 10);
	insertAndPrint(first, 8);

	cout << "Insert complete" << endl;

	BKeyList<int>* second = first->split();
	cout << first->traverse() << " | " << second->traverse() << endl;

	delete first;
	delete second;

	return 0;
}

void printStatus(BKeyList<int>* list) {
	if (list->splitRequired()) {
		cout << "Split Required" << endl;
	}
	cout << "Current status: " << list->traverse() << endl;
}

void insertAndPrint(BKeyList<int>* list, int key) {
	list->insert(key);
	printStatus(list);
}