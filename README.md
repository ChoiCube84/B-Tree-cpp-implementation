# B Tree Implementation in C++
This repository contains a basic implementation of a B Tree data structure in C++. Born out of a sense of revenge following a failed assignment during a Data Structure class, this project aims to provide a clean, simple, and understandable implementation of a B Tree.

# Revenge Story
The initial inspiration for this project came from a Data Structures assignment, which involved implementing a B Tree. Unfortunately, the original assignment didn't go as planned, but failure isn't the end. It's a chance to learn and grow. Hence, this project is a testament to the spirit of learning and improvement.

# Project Overview
B Trees are a type of self-balancing search tree that maintains sorted data and allows for efficient insertion, deletion, and search operations. They're widely used in systems that read and write large blocks of data, like databases and file systems.

This project is implemented using Object-Oriented Programming (OOP) in C++, ensuring the code is modular, easy to understand, and flexible to changes.

# Features
The B Tree implementation supports the following operations:

1. Insertion: Add keys into the B Tree while maintaining its properties.
2. Deletion: Remove keys from the B Tree while maintaining its properties.
3. Search: Find if a key exists in the B Tree.
4. Traversal: Traverse through the keys in the B Tree.

# Constraints for Type Usage with this B-Tree
This B-Tree implementation is designed to handle a variety of data types. However, for correct operation and to prevent unexpected errors, the types you intend to use with this B-Tree must satisfy the following requirements:

1. Comparison Operators: Your data type should support the `<`, `>`, `==`, and `!=` comparison operators. These operators are essential for maintaining the B-Tree's ordering properties and for performing operations like search and deletion.
2. Deep Copying with Assignment Operator: Your data type should ensure deep copying when the `=` assignment operator is used. Shallow copies can lead to unexpected behaviors and bugs, especially when dealing with complex types.

Please ensure your custom types fulfill these requirements before using them with this B-Tree implementation.

# Getting Started
To compile the project, you need to have a suitable C++ compiler installed on your system that supports C++20. 

(If you are planning to use the Python bindings in the future, please stay tuned for additional instructions on that.)

Then, navigate to the project directory and run the following command:

```bash
g++ test.cpp -o test
```

To run the compiled program:

```bash
./test
```

# Usage Example
Below is an example of how to use the B Tree implementation with integers:

```cpp
#include "b_tree.h"

int main() {
    BTree<int> tree(3); // Create a B Tree with order 3
    
    // Insert keys
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    
    // Search for a key
    bool found = tree.search(2); // Returns true
    
    return 0;
}
```

# Future Work
I am planning to extend the utility of this B-Tree implementation by enabling Python bindings. This would allow the high-speed benefits of C++ to be utilized directly in Python environments. Details on this integration and usage will be provided as the development progresses.
