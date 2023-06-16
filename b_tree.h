#ifndef __B_TREE__
#define __B_TREE__

#include <iostream>

template <typename T>
class BTree
{
private:
	T num;
public:
	BTree(T num) : num(num)
	{
		std::cout << "B Tree was made" << std::endl;
	}
	~BTree() {
		std::cout << "B Tree was destructed" << std::endl;
	}
	T showNum() { return num; }
};

#endif