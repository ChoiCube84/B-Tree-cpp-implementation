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

	void insertByIndex(const T& key, size_t index) { 
		if (currentSize < order) {
			T prev = keys[index];

			for (size_t i = index; i < currentSize; i++) {
				swap<T>(prev, keys[i + 1]);
			}

			keys[index] = key;
			currentSize++;
		}
	}

	bool removeByIndex(const T& key, size_t index) {
		if (keys[index] != key) {
			return false;
		}

		else {
			currentSize--;

			for (size_t i = index; i < currentSize; i++) {
				keys[i] = keys[i + 1];
			}

			return true;
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
		size_t result = 0;

		if (currentSize > 0) {
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

			result = end;

			if (keys[end] < key)
				result++;
		}

		std::cout << "Index of the Key: " << result << std::endl; // For debugging
		return result;
	}

	void insert(const T& key) {
		size_t keyIndex = findIndex(key);
		insertByIndex(key, keyIndex);
	}

	bool remove(const T& key) {
		size_t keyIndex = findIndex(key);
		bool deletionResult = false;

		if (keyIndex < currentSize) {
			deletionResult = removeByIndex(key, keyIndex);
		}
		
		return deletionResult;
	}

	bool splitRequired() {
		return currentSize >= order; 
	}

	BKeyList* split() {
		if (currentSize < order) return nullptr;
		else {
			BKeyList* tail = new BKeyList(order);
			
			currentSize = order / 2;
			for (size_t i = order / 2 + 1; i < order; i++) {
				tail->insert(keys[i]);
			}

			return tail;
		}
	}

	std::string traverse() {
		std::stringstream ss;
		
		for (size_t i = 0; i < currentSize; i++) {
			if (i != 0) ss << " ";
			ss << keys[i];
		}

		return ss.str();
	}

	const size_t getCurrentSize() const { 
		return currentSize; 
	}

	const T& getKeyByIndex(size_t index) const {
		return keys[index];
	}

	const T& getSmallestKey(void) const {
		return keys[0];
	}

	const T& getLargestKey(void) const {
		return keys[currentSize - 1];
	}
	
	bool isExistingKey(const T& key) {
		size_t indexOfKey = findIndex(key);
		return (key == getKeyByIndex(indexOfKey));
	}

	//TODO: Check if this function is neccessary
	bool isEmpty(void) {
		return (currentSize == 0);
	}

	// TODO: Reconsider using this function
	void setKeyByIndex(const T& newKey, size_t index) {
		keys[index] = newKey;
	}

	void mergeWithOtherBKeyList(BKeyList* other) {
		for (size_t i = 0; i < other->getCurrentSize(); i++) {
			T currentKey = other->getKeyByIndex(i);
			insert(currentKey);
		}
	}
};

#endif // !__B_KEY_LIST__
