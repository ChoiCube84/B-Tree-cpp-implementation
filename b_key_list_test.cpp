#include <iostream>
#include "b_key_list.h"

using namespace std;

int main(void) {
	BKeyList<int> list(3);

	list.insert(3);
	list.insert(1);
	list.insert(2);

	cout << list.traverse() << endl;

	return 0;
}