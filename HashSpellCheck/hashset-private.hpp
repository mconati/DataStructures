
/**
 * \file hashset-private.hpp
 *
 * \author What are your CS70 aliases?
 *
 * \brief Implements HashSet<T>, a hash-table class template
 *
 * \remark There is no include-guard for this file, because it is
 *         only #included by hashset.hpp, inside hashset.hpp's
 *         own include guard.
 */

// Includes required for your templated code go here
#include "hashset.hpp"
#include <iostream>
#include <vector>

// Templated code for member functions goes here

// default constructor
template <typename T>
HashSet<T>::HashSet() : hashTable_{new std::vector<T>[INITIAL_BUCKETS]} {
    // sets initial values
    size_ = 0;
    buckets_ = INITIAL_BUCKETS;
    reallocations_ = 0;
    collisions_ = 0;
    maximal_ = 0;
}

// desctructor
template <typename T>
HashSet<T>::~HashSet() {
    delete[] hashTable_;
}

// returns size_ which is the number of elements
template <typename T>
size_t HashSet<T>::size() const {
    return size_;
}

// inserts an element into the hash table
template <typename T>
void HashSet<T>::insert(const T& x) {
    // if the element exists, behavior is undefined. We just return
    if (exists(x)) {
        return;
    }
    // If the hash table is too full(determined by our load factor), make
    // a new table using reallocate and increment reallocations
    if (static_cast<double>(size_)/static_cast<double>(buckets_) >= MAX_LOAD) {
        ++reallocations_;
        reallocate();
    }
    // initialize indexes and lengths for later
    size_t hv = myhash(x);
    size_t index = hv % buckets();
    size_t length = hashTable_[index].size();
    // increment collisions if you insert into a bucket with an element
    if (length != 0) {
        ++collisions_;
    }
    // insert the element and increment bucket length. Increment maximal if
    // it is a new longest bucket.
    hashTable_[index].push_back(x);
    ++length;
    if (length > maximal_) {
        ++maximal_;
    }
    ++size_;
}

// reallocate - doubles the size of the hashtable
template <typename T>
void HashSet<T>::reallocate() {
    // double the number of buckets and reset the other data members
    buckets_ = buckets_ * 2;
    collisions_ = 0;
    size_ = 0;
    maximal_ = 0;
    // create a copy of the original hash table
    auto tempTable = hashTable_;
    // create a new hash table
    hashTable_ = new std::vector<T>[buckets_];
    // insert all the elements into the new hashtable,
    // thus also updating the data members
    for (size_t i = 0; i < buckets_ / 2; ++i) {
        for (size_t j = 0; j < tempTable[i].size(); ++j) {
            insert(tempTable[i][j]);
        }
    }
}


// check if the element is already in the hash table
template <typename T>
bool HashSet<T>::exists(const T& x) const {
    // hash the element
    size_t hv = myhash(x);
    size_t index = hv % buckets();
    // check to see if that element is in the table
    for (size_t j = 0; j < hashTable_[index].size(); ++j) {
        if (x == hashTable_[index][j]) {
            return true;
        }
    }
    return false;
}

// return the number of buckets in the table
template <typename T>
size_t HashSet<T>::buckets() const {
    return buckets_;
}

// return the the of times the hashtable has been resized
template <typename T>
size_t HashSet<T>::reallocations() const {
    return reallocations_;
}

// return the recorded number of collisions
template <typename T>
size_t HashSet<T>::collisions() const {
    return collisions_;
}

// return the legnth of the largest chain
template <typename T>
size_t HashSet<T>::maximal() const {
    return maximal_;
}

// displays the statistics of the hash table
template <typename T>
std::ostream& HashSet<T>::showStatistics(std::ostream& out) const {
    out << "Statistics:" << std::endl;
    out << "             Size: " << size() << std::endl;
    out << "          Buckets: " << buckets() << std::endl;
    out << "    Reallocations: " << reallocations() << std::endl;
    out << "       Collisions: " << collisions() << std::endl;
    out << "Max Bucket length: " << maximal() << std::endl;
    return out;
}
