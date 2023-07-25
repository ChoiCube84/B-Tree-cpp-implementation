#ifndef __B_TREE_ERRORS_H__
#define __B_TREE_ERRORS_H__

#include <string>
#include <stdexcept>

class InvalidOrder : public std::invalid_argument {
public:
	InvalidOrder(const std::string& message) : invalid_argument(message) {
	}
};

class InsertionFailure : public std::logic_error {
public:
	InsertionFailure(const std::string& message) : logic_error(message) {
	}
};

class DeletionFailure : public std::logic_error {
public:
	DeletionFailure(const std::string& message) : logic_error(message) {
	}
};

#endif // !__B_TREE_ERRORS_H__
