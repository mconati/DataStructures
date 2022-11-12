/**
 * \file hashset.hpp
 *
 * \author What are your CS70 aliases?
 *
 * \brief Provides HashSet<T>, a set class template, using hash tables
 */

#ifndef HASHSET_HPP_INCLUDED
#define HASHSET_HPP_INCLUDED 1

// Any header files that are needed to typecheck the class
// declaration should be #included here.

// Templated interfaces (e.g., the HashSet class declarations) go here

// Most users of this header file would be interested in interfaces: how does
// one use a HashSet<T>? What member functions does it provide? What parameters
// do they take?
//
// Because the hashset code is templated, it must appear in this header file
// instead of a normal .cpp file. We do this by moving the nasty implementation
// details into hashset-private.hpp, and recursively including that here.  This
// way, readers don't see the actual code unless they want to, and aren't forced
// to manually include two separate header files every time they want to use
// this hash table.


#include <iostream>
#include <vector>

template <typename T>
class HashSet{
 public:
    // Default Constructor and destructor
    HashSet();
    ~HashSet();
    // No assignment operator or copy constructor
    HashSet(const HashSet&) = delete;
    HashSet operator=(const HashSet&) = delete;

    // returns the number of items stored in the table
    size_t size() const;

    // inserts an element into the hash table. Undefined if the insertee
    // has already been inserted
    void insert(const T&);

    // resizes the hashtable
    void reallocate();

    // returns true if the element is present in the hash table
    bool exists(const T&) const;

    // returns the number of buckets
    size_t buckets() const;

    // returns the number of times the hash table has resized
    size_t reallocations() const;

    // returns the number of times an insert has caused a collision
    size_t collisions() const;

    // returns the length of the longest chain(we are using separate chaining)
    size_t maximal() const;

    // prints all the relevant statistics about the hash table
    std::ostream& showStatistics(std::ostream& out) const;

 private:
    // private variables
    static const size_t INITIAL_BUCKETS = 10;
    const double MAX_LOAD = 0.5;

    size_t size_;           // size of hash table
    size_t buckets_;        // number of buckets
    size_t reallocations_;  // number of resizes
    size_t collisions_;     // number of collsions
    size_t maximal_;        // maximum chain length
    std::vector<T>* hashTable_;
};
#include "hashset-private.hpp"

#endif
