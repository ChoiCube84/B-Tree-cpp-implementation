#ifndef __B_KEY_LIST__
#define __B_KEY_LIST__

#include <iostream>
#include <string>

template<typename T>
class BKeyList 
{
private:
	T* keys;
	size_t order;
	size_t currentSize;

	bool insert(const T& key, size_t idx) {
		if (currentSize == order) return false;

		T prev = keys[idx];
		for (size_t i = idx; i < currentSize; i++) {
			T temp = prev;
			prev = keys[i + 1];
			keys[i + 1] = temp;
			// swap(prev, keys[i + 1]);
		}
		currentSize++;
		keys[idx] = key;
		
		return true;
	}

	bool remove(const T& key, size_t idx) {
		if (keys[idx] != key) return false;

		currentSize--;
		for (size_t i = idx; i < currentSize; i++) {
			keys[i] = keys[i + 1];
		}
		
		return true;
	}

public:
	BKeyList(size_t order) : keys(new T[order - 1]), order(order), currentSize(0)
	{
		std::cout << "B Key List was made" << std::endl;
	}
	~BKeyList() {
		delete[] keys;
		std::cout << "B Key List was destructed" << std::endl;
	}


	// TODO: Verify if findIndex works properly

	size_t findIndex(const T& key, size_t begin, size_t end) { 
		size_t middle = (begin + end) / 2;

		if (begin >= end) {
			if (keys[end] < key) return end + 1;
			else return end;
		}
		else if (keys[middle] < key) {
			return findIndex(key, middle + 1, end);
		}
		else {
			return findIndex(key, begin, middle);
		}
	}

	int insert(const T& key) {
		size_t keyIndex = 0;
		if (currentSize > 0) keyIndex = findIndex(key, 0, currentSize - 1);
		std::cout << "Index of the Key: " << keyIndex << std::endl;

		if (insert(key, keyIndex)) {
			std::cout << "Insert Success" << std::endl;
			std::cout << "Current status: " << traverse() << std::endl;
			return keyIndex;
		}
		else {
			std::cout << "Insert Failed" << std::endl;
			std::cout << "Current status: " << traverse() << std::endl;
			return -1;
		}
	}

	int remove(const T& key) {
		size_t keyIndex = 0;
		if (currentSize > 0) keyIndex = findIndex(key, 0, currentSize - 1);
		std::cout << "Index of the Key: " << keyIndex << std::endl;

		if (remove(key, keyIndex)) {
			std::cout << "Removal Success" << std::endl; 
			std::cout << "Current status: " << traverse() << std::endl;
			return keyIndex;
		}
		else {
			std::cout << "Removal Failed" << std::endl;
			std::cout << "Current status: " << traverse() << std::endl;
			return -1;
		}
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
