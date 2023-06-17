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

	int find(const T& key, int begin, int end) {
		int middle = (begin + end) / 2;

		if (begin > end) {
			return -1;
		} 
		else if (keys[middle] == key) {
			return middle;
		}
		else if (keys[middle] > key) {
			return find(key, begin, middle - 1);
		}
		else {
			return find(key, middle + 1, end);
		}
	}

	int insert(const T& key) {
		std::cout << "Not Implemented yet" << std::endl;
		return 0;
	}

	int remove(const T& key) {
		std::cout << "Not Implemented yet" << std::endl;
		return 0;
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

	const size_t getCurrentSize() const { return currentSize; }
};

#endif // !__B_KEY_LIST__
