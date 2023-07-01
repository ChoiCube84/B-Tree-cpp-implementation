#ifndef __B_KEY_LIST__
#define __B_KEY_LIST__

#include <iostream>
#include <string>
#include <sstream>

template<typename T>
class BKeyList 
{
private:

	T* keys;			// An array that holds the keys
	size_t order;		// The order of B-Tree
	size_t currentSize; // Number of keys in the list currently

	// Insert a key in a specific index
	void insert(const T& key, size_t idx) { 
		if (currentSize < order) {
			T prev = keys[idx];

			for (size_t i = idx; i < currentSize; i++) {
				T temp = prev;
				prev = keys[i + 1];
				keys[i + 1] = temp;
			}

			keys[idx] = key;
			currentSize++;
		}
	}

	// Remove a key from a specific index and return success
	bool remove(const T& key, size_t idx) {
		if (keys[idx] != key) {
			return false;
		}

		else {
			currentSize--;

			for (size_t i = idx; i < currentSize; i++) {
				keys[i] = keys[i + 1];
			}

			return true;
		}
	}

public:
	
	// Initializer of BKeyList
	BKeyList(size_t order) : keys(new T[order]), order(order), currentSize(0)
	{
		std::cout << "B Key List was made" << std::endl;
	}

	// Destructor of BKeyList
	~BKeyList() {
		delete[] keys;
		std::cout << "B Key List was destructed" << std::endl;
	}

	// Find and return the index of a key
	size_t findIndex(const T& key) {
		size_t begin = 0;
		size_t end = currentSize - 1;
		
		while (begin < end) {
			size_t middle = (begin + end) / 2;
			if (keys[middle] < key) {
				begin = middle + 1;
			}
			else {
				end = middle;
			}
		}

		size_t result = end;
		
		if (keys[end] < key) 
			result++;
		
		std::cout << "Index of the Key: " << result << std::endl; // For debugging

		return result;
	}

	// Insert a key while maintaining the order
	void insert(const T& key) {
		size_t keyIndex = 0;
		if (currentSize > 0) keyIndex = findIndex(key);
		
		insert(key, keyIndex);
	}

	// Delete a key while maintaining the order
	int remove(const T& key) {
		size_t keyIndex = 0;
		if (currentSize > 0) keyIndex = findIndex(key);

		if (keyIndex < currentSize && remove(key, keyIndex)) {
			return -1;
		}
		else {
			return keyIndex;
		}
	}

	// Check if this BKeyList should be splitted
	bool splitRequired() {	return currentSize >= order; }

	// Split this BKeyList and return the tail part
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

	// Access to the key value by using [] operator
	T& operator[](int idx) {
		return keys[idx];
	}

	const T& operator[](int idx) const {
		return keys[idx];
	}

	// Returns the string of traverse of this BKeyList
	std::string traverse() {
		std::stringstream ss;
		for (size_t i = 0; i < currentSize; i++) {
			if (i != 0) ss << " ";
			ss << keys[i];
		}
		return ss.str();
	}

	// Returns the number of keys inside this BKeyList
	const size_t getCurrentSize() const { return currentSize; }
};

#endif // !__B_KEY_LIST__
