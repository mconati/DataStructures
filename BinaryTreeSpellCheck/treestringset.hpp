/**
 * \file treestringset.hpp
 * \authors macaw/owl
 * \brief A binary search tree of strings.
 *
 */

#ifndef TREESTRINGSET_HPP_INCLUDED
#define TREESTRINGSET_HPP_INCLUDED 1

#include <cstddef>
#include <iostream>

 /**
 * \class TreeStringSet
 *
 * \brief A binary search tree representing a set of strings
 *
 * \details Class allocates memory dynamically; thus can't use C++'s
 *          defaults for copy constructor, assignment operator and
 *          destructor.
 */
class TreeStringSet {
public:
    // Initializing Default Constructor and Destructor which we will write,
    // And deleting the assignment operator and copy constructor
    TreeStringSet();
    TreeStringSet(const TreeStringSet& orig) = delete;
    TreeStringSet& operator=(TreeStringSet& rhs) = delete;
    ~TreeStringSet();

    size_t size() const; ///< Size of set
    void insert(const std::string& insertee); ///< Adds string to set
    bool exists(const std::string& checker) const; ///< True if string in set
    int height() const; ///< returns height of set
    std::ostream& print(std::ostream& out) const; ///< prints all strings
    /// prints relevant stats about string set
    std::ostream& showStatistics(std::ostream& out) const;

private:
    // "struct" class to define our Nodes
    struct Node {
        std::string value_;
        Node* left_;
        Node* right_;
        size_t size_;

        Node(std::string value);
        Node(const Node&) = default;
        Node& operator=(const Node&) = default;
        ~Node();
    };

    // Each tree should have a reference to a root node
    // even if empty
    Node* root_;
    // These declare our helper functions which are privately called
    // from the public Class functions
    void insertHelper(Node*& nd, const std::string& s);
    static bool existsHelper(const Node* nd, const std::string& s);
    int heightHelper(Node* nd) const;
    void insertAtRoot(Node*& root, std::string insertee);
    void rotateRight(Node*& top);
    void rotateLeft(Node*& top);
    static std::ostream& printHelper(const Node* root, std::ostream& out);
    // Seed of our random number generator
    unsigned int seed_;
 };
 #endif //TREESTRINGSET_INCLUDED