#include <iostream>
#include "b_key_list.h"

using namespace std;

int main(void) {
	BKeyList<int>* first = new BKeyList<int>(10);

	first->insert(4);
	first->insert(7);
	first->insert(3);
	first->insert(6);
	first->insert(2);
	first->insert(5);
	first->insert(1);
	first->insert(9);
	first->insert(10);
	first->insert(8);

	cout << "Insert complete" << endl;

	BKeyList<int>* second = first->split();
	cout << first->traverse() << " | " << second->traverse() << endl;

	delete first;
	delete second;

	return 0;
}