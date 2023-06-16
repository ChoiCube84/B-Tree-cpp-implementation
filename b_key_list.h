#ifndef __B_KEY_LIST__
#define __B_KEY_LIST__

#include <iostream>
#include <string>

template<typename T>
class BKeyList 
{
private:
	T* keys;
	size_t currentSize;
public:
	BKeyList(size_t order) : keys(new T[order - 1]), currentSize(0)
	{
		std::cout << "B Key List was made" << std::endl;
	}
	~BKeyList() {
		delete[] keys;
		std::cout << "B Key List was destructed" << std::endl;
	}

	void insert(const T& key) {
		std::cout << "Not Implemented yet" << std::endl;
	}

	void remove(const T& key) {
		std::cout << "Not Implemented yet" << std::endl;
	}

	T& operator[](int idx) {
		return keys[idx];
	}

	const T& operator[](int idx) const {
		return keys[idx];
	}

	std::string traverse() {
		std::string result = "";
		for (size_t i = 0; i < currentSize; i++) {
			result += std::to_string(keys[i]) + " ";
		}
		return result;
	}

	const size_t getCurrentSize() const { return currentSize;  }
};

#endif // !__B_KEY_LIST__
