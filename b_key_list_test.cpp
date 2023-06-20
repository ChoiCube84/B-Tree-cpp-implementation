#include <iostream>
#include "b_key_list.h"

using namespace std;

int main(void) {
	BKeyList<int> list(10);

	list.insert(4);
	list.insert(7);
	list.insert(3);
	list.insert(6);
	list.insert(2);
	list.insert(5);
	list.insert(1);
	list.insert(6);
	list.insert(2);
	list.insert(5);
	list.insert(1);

	cout << list.traverse() << endl;

	return 0;
}