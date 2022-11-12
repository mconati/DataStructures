/*
 * \file treestringset.cpp
 * \authors Macaw and Owl
 * \brief Implemenation of Treestringset and its private class (Node).
 */

#include <stdlib.h>
#include <algorithm>
#include <ctime>
#include <string>

#include "testing-logger.hpp"
#include "treestringset.hpp"

/* Default constructor
* Constructs an empty treestringset with nullpointers in nodes
*/
TreeStringSet::TreeStringSet() : root_{nullptr},
seed_{static_cast<unsigned int>(time(nullptr))} {
}

TreeStringSet::~TreeStringSet() {
    delete root_;
}


// Returns the size of the treestringset
size_t TreeStringSet::size() const {
    if (root_ == nullptr) {
        return 0;
    } else {
        return root_->size_ + 1;
    }
}

/////////////////////////////
// Node default constructor
/////////////////////////////
TreeStringSet::Node::Node(std::string value) {
    value_ = value;
    left_ = nullptr;
    right_ = nullptr;
    size_ = 0;
}

TreeStringSet::Node::~Node() {
    if (left_ != nullptr) {
        delete left_;
    }
    if (right_ != nullptr) {
        delete right_;
    }
}


// Insert Inserts In certain random place in a randomized order
// binary search tree by reordering the whole tree
void TreeStringSet::insert(const std::string& insertee) {
    insertHelper(root_, insertee);
}


void TreeStringSet::insertHelper(Node*& nd, const std::string& insertee) {
    if (nd == nullptr) {
        insertAtRoot(nd, insertee);
    } else if (rand_r(&seed_) % (nd->size_ + 1) == 0) {
        insertAtRoot(nd, insertee);
    } else if (insertee < nd->value_) {
        ++nd->size_;
        insertHelper(nd->left_, insertee);
    } else {
        ++nd->size_;
        insertHelper(nd->right_, insertee);
    }
}

// Inserts and increments size
void TreeStringSet::insertAtRoot(Node*& rootNode, std::string insertee) {
    if (rootNode == nullptr) {
        rootNode = new Node{insertee};
    } else if (insertee < rootNode->value_) {
        ++rootNode->size_;
        insertAtRoot(rootNode->left_, insertee);
        rotateRight(rootNode);
    } else if (insertee > rootNode->value_) {
        ++rootNode->size_;
        insertAtRoot(rootNode->right_, insertee);
        rotateLeft(rootNode);
    }
}


// Our expanded rotateRight rotates right around node top
// and adjusts sizes accordingly
void TreeStringSet::rotateRight(Node*& top) {
    Node* b = top->left_;
    size_t elemLeftSize;
    size_t prevRootSize = top->size_;
// Adjusts the size of the node that used to be the top
// Depending on its new children
    if (b->left_ == nullptr) {
        elemLeftSize = 0;
    } else {
        elemLeftSize = b->left_->size_;
    }
    top->left_ = b->right_;
// Changes the size of the new top to be the size of the old one
    b->size_ = prevRootSize;
    b->right_ = top;
    top->size_ = prevRootSize - (1 + elemLeftSize);
    top = b;
}
// Our expanded rotateLeft rotates left around node top
// and adjusts sizes accordingly
void TreeStringSet::rotateLeft(Node*& top) {
    Node* d = top->right_;
    size_t prevRootSize = top->size_;
    size_t elemRightSize = 0;
// Adjusts the size of the node that used to be the top
// Depending on its new children
    if (d->right_ == nullptr) {
        elemRightSize = 0;
    } else {
        elemRightSize = d->right_->size_;
    }
    top->right_ = d->left_;
// Changes the size of the new top to be the size of the old one
    d->size_ = prevRootSize;
    d->left_ = top;
    top->size_ = prevRootSize - (1 + elemRightSize);
    top = d;
}
// Checks throughout a tree to see if an element exists
// by starting at the root and going in the correct direction
// depending on a comparison (<, >)
bool TreeStringSet::exists(const std::string& checker) const {
    if (root_ == nullptr) {
        return false;
    }
    return existsHelper(root_, checker);
}

// Helper function that does all of the work of checking for exists
bool TreeStringSet::existsHelper(const Node* nd, const std::string& s) {
    if (nd->value_ == s) {
        return true;
    } else if (nd->value_ > s) {
        if (nd->left_ != nullptr) {
            return existsHelper(nd->left_, s);
        }
    } else {
        if (nd->right_ != nullptr) {
            return existsHelper(nd->right_, s);
        }
    }
    return false;
}

// Checks all of the paths in a tree and returns
// the height of the longest boi.
int TreeStringSet::height() const {
    if (size() == 0) {
        return -1;
    } else {
        return heightHelper(root_);
    }
}
// Helper function that does all of the work in finding
// the height of paths through the tree
int TreeStringSet::heightHelper(Node* nd) const {
    if (nd->left_ == nullptr && nd->right_ == nullptr) {
        return 0;
    } else if (nd->left_ != nullptr && nd->right_ == nullptr) {
        return 1 + heightHelper(nd->left_);
    } else if (nd->left_ == nullptr && nd->right_ != nullptr) {
        return 1 + heightHelper(nd->right_);
    } else {
        return 1 + std::max(heightHelper(nd->left_),
        heightHelper(nd->right_));
    }
}
// Recursively prints out the treestringset in the form:
// :(left subtree, node value, right subtree)
std::ostream& TreeStringSet::print(std::ostream& out) const {
    return printHelper(root_, out);
}

std::ostream& TreeStringSet::printHelper(const Node* root, std::ostream& out) {
    // First check if tree is empty
    // std::string output = '';
    if (root == nullptr) {
        out << "-";
    } else {
        out << root->size_;
        out << ":(";
        printHelper(root->left_, out);
        out << ", ";
        out << root->value_;
        out << ", ";
        printHelper(root->right_, out);
        out << ")";
    }
    return out;
}


std::ostream& TreeStringSet::showStatistics(std::ostream& out) const {
    out << "Here's some stagtistics" << std::endl;
    out << "Root Value: " << root_->value_ << std::endl;
    out << "Height: " << height() << std::endl;
    out << "Size: " << size() << std::endl;
    return out;
}
