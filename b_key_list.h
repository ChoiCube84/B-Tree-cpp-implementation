#ifndef __B_KEY_LIST__
#define __B_KEY_LIST__

#include <iostream>
#include <string>
#include <sstream>

template<typename T>
class BKeyList 
{
private:
	T* keys;
	size_t order;
	size_t currentSize;

	bool insert(const T& key, size_t idx) {
		if (currentSize < order) {
			T prev = keys[idx];
			for (size_t i = idx; i < currentSize; i++) {
				T temp = prev;
				prev = keys[i + 1];
				keys[i + 1] = temp;
				// swap(prev, keys[i + 1]);
			}
			keys[idx] = key;
			currentSize++;
		}
		if (currentSize < order) return false;
		else return true;
	}

	bool remove(const T& key, size_t idx) {
		if (keys[idx] != key) return false;

		currentSize--;
		for (size_t i = idx; i < currentSize; i++) {
			keys[i] = keys[i + 1];
		}
		
		return true;
	}

	// TODO: Verify if findIndex works properly
	// TODO: Make this function not use recursion 
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

public:
	BKeyList(size_t order) : keys(new T[order]), order(order), currentSize(0)
	{
		std::cout << "B Key List was made" << std::endl;
	}
	~BKeyList() {
		delete[] keys;
		std::cout << "B Key List was destructed" << std::endl;
	}

	size_t findIndex(const T& key) {
		return findIndex(key, 0, currentSize - 1);
	}

	bool insert(const T& key) {
		size_t keyIndex = 0;
		if (currentSize > 0) keyIndex = findIndex(key);
		std::cout << "Index of the Key: " << keyIndex << std::endl;

		if (insert(key, keyIndex)) {
			std::cout << "Split Required" << std::endl;
			std::cout << "Current status: " << traverse() << std::endl;
			return true;
		}
		else {
			std::cout << "Current status: " << traverse() << std::endl;
			return false;
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

	BKeyList* split() {
		if (currentSize < order) return nullptr;
		else {
			BKeyList* tail = new BKeyList(order);
			
			currentSize = order / 2;
			for (int i = order / 2 + 1; i < order; i++) {
				tail->insert(keys[i]);
			}

			return tail;
		}
	}

	T& operator[](int idx) {
		return keys[idx];
	}

	const T& operator[](int idx) const {
		return keys[idx];
	}

	std::string traverse() {
		std::stringstream ss;
		for (size_t i = 0; i < currentSize; i++) {
			if (i != 0) ss << " ";
			ss << keys[i];
		}
		return ss.str();
	}

	const size_t getCurrentSize() const { return currentSize; }
};

#endif // !__B_KEY_LIST__
