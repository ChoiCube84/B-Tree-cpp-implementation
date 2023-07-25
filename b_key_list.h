#ifndef __B_KEY_LIST__
#define __B_KEY_LIST__

#include <string>
#include <sstream>
#include <stdexcept>

#include "b_tree_errors.h"
#include "b_tree_constraints.h"

template<UsableInBTree T>
class BKeyList 
{
public: // Public member variable
	const size_t order;
	
private: // Private member variable
	T* keys;
	size_t currentSize;

public: // Public primary functions
	BKeyList(size_t order) : order(ensureValidOrder(order)), currentSize(0), keys(new T[order]) {
	}

	~BKeyList() {
		delete[] keys;
	}

	void insert(const T& key) {
		size_t keyIndex = findIndex(key);
		insertByIndex(key, keyIndex);
	}

	void remove(const T& key) {
		size_t keyIndex = findIndex(key);
		removeByIndex(key, keyIndex);
	}

	const bool search(const T& key) const {
		size_t indexOfKey = findIndex(key);
		try {
			return (key == getKeyByIndex(indexOfKey));
		}
		catch (std::out_of_range&) {
			return false;
		}
	}

	const std::string traverse() const {
		std::stringstream ss;

		for (size_t i = 0; i < currentSize; i++) {
			if (i != 0) ss << " ";
			ss << keys[i];
		}

		return ss.str();
	}

public: // Public helper functions
	const size_t findIndex(const T& key) const {
		size_t result = 0;

		if (currentSize == 0) {
			return 0;
		}
		else {
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

			return result;
		}
	}

	const bool splitRequired() const {
		return currentSize >= order; 
	}

	void splitBKeyList(BKeyList* tail) {
		if (tail == nullptr) {
			throw std::invalid_argument("The tail BKeyList is null");
		}
		else if (!tail->isEmpty()) {
			throw std::invalid_argument("The tail BKeyList is not empty");
		}
		else if (currentSize < order) {
			throw std::logic_error("There are not enough keys in the BKeyList");
		}
		else {
			currentSize = order / 2;
			for (size_t i = order / 2 + 1; i < order; i++) {
				tail->insertByIndex(keys[i], i - (order / 2 + 1));
			}
		}
	}

	void mergeBKeyList(BKeyList* other) {
		for (size_t i = 0; i < other->currentSize; i++) {
			T currentKey = other->getKeyByIndex(i);
			insert(currentKey);
		}
	}

	const size_t getCurrentSize() const { 
		return currentSize; 
	}

	const T& getKeyByIndex(size_t index) const {
		if (index >= currentSize) {
			throw std::out_of_range("The index is out of range");
		}
		return keys[index];
	}

	const T& getSmallestKey(void) const {
		if (currentSize == 0) {
			throw EmptyBKeyList("The BKeyList is empty");
		}
		return keys[0];
	}

	const T& getLargestKey(void) const {
		if (currentSize == 0) {
			throw EmptyBKeyList("The BKeyList is empty");
		}
		return keys[currentSize - 1];
	}

	const bool isEmpty(void) const {
		return (currentSize == 0);
	}

private: // Private helper functions
	void insertByIndex(const T& key, size_t index) {
		if (currentSize >= order) {
			throw InsertionFailure("Failed to insert the key");
		}

		for (size_t i = currentSize; i > index; i--) {
			keys[i] = keys[i - 1];
		}

		keys[index] = key;
		currentSize++;
	}

	void removeByIndex(const T& key, size_t index) {
		if (index >= currentSize) {
			throw std::out_of_range("The index is out of range");
		}
		else if (keys[index] != key) {
			throw DeletionFailure("Failed to delete the key");
		}
		else {
			currentSize--;

			for (size_t i = index; i < currentSize; i++) {
				keys[i] = keys[i + 1];
			}
		}
	}
};

#endif // !__B_KEY_LIST__