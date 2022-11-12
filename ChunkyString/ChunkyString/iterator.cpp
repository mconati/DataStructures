/**
 * \file iterator.cpp
 * author: CS70 Starter Code
 *
 * \brief Contains function stubs for the implementation of
 * ChunkyString::Iterator  and ChunkyString::ConstIterator
 */

#include <stdexcept>
#include "chunkystring.hpp"

// ----------------------- Iterator stubs: ----------------------

// default constructor
ChunkyString::Iterator::Iterator() : chunkPointer_{}, index_{0} {
    // nothing to do here
}

// 2 Parameter contructor : chunk_iter_t and size_t
ChunkyString::Iterator::Iterator(chunk_iter_t chunkPtr, size_t i)
    : chunkPointer_{}, index_{0} {
    chunkPointer_ = chunkPtr;
    index_ = i;
}

// Assignment Operator
ChunkyString::Iterator& ChunkyString::Iterator::
operator=(const Iterator& other) {
    chunkPointer_ = other.chunkPointer_;
    index_ = other.index_;

    return *this;
}

// Iterator increment
ChunkyString::Iterator& ChunkyString::Iterator::operator++() {
    // if iterator is not at the last element in a chunk,
    // increment index
    if (index_ < chunkPointer_->length_- 1) {
        ++index_;
    } else {
        // index is at the last element of the chuck, so move iterator
        // to point at the first element of the next chunk
        index_ = 0;
        ++chunkPointer_;
    }
    return *this;
}

// Iterator decrement
ChunkyString::Iterator& ChunkyString::Iterator::operator--() {
    // if iterator not at the first element in a chunk,
    // decrement index
    if (index_ > 0) {
        --index_;
    } else {
        // iterator is at the first element, so decrement chunkPointer
        // and set index to be the length -1 of that chunk's length
        --chunkPointer_;
        index_ = chunkPointer_->length_ - 1;
    }
    return *this;
}

// Iterator dereference
ChunkyString::Iterator::reference ChunkyString::Iterator::operator*() const {
    // return the element referenced by the iterator
    return (chunkPointer_->chars_[index_]);
}

// Iterator equals
bool ChunkyString::Iterator::operator==(const Iterator& rhs) const {
    // see if both data members of both iterators are equal
    if (this->chunkPointer_ == rhs.chunkPointer_
        && this->index_ == rhs.index_) {
        return true;
    }
    return false;
}

// Iterator not equals
bool ChunkyString::Iterator::operator!=(const Iterator& rhs) const {
    // return !=
    return !(*this == rhs);
}

// ----------------------- ConstIterator stubs: ----------------------


ChunkyString::ConstIterator::ConstIterator() : chunkPointer_{}, index_{0} {
    // nothing to do here
}
// 1 Parameter contructor : Interator
ChunkyString::ConstIterator::ConstIterator(const Iterator& i) :
    chunkPointer_{i.chunkPointer_}, index_{i.index_} {
}

// 2 Parameter contructor : chunk_iter_t and size_t
ChunkyString::ConstIterator::ConstIterator(chunk_iter_t chunkPtr, size_t i)
: chunkPointer_{}, index_{0} {
    chunkPointer_ = chunkPtr;
    index_ = i;
}

// Assignment Operator
ChunkyString::ConstIterator& ChunkyString::ConstIterator::
operator=(const ConstIterator& other) {
    chunkPointer_ = other.chunkPointer_;
    index_ = other.index_;

    return *this;
}

// Iterator increment
ChunkyString::ConstIterator& ChunkyString::ConstIterator::operator++() {
    // if iterator is not at the last element in a chunk,
    // increment index
    if (index_ < chunkPointer_->length_- 1) {
        ++index_;
    } else {
        // index is at the last element of the chuck, so move iterator
        // to point at the first element of the next chunk
        index_ = 0;
        ++chunkPointer_;
    }
    return *this;
}

// Iterator decrement
ChunkyString::ConstIterator& ChunkyString::ConstIterator::operator--() {
    // if iterator not at the first element in a chunk,
    // decrement index
    if (index_ > 0) {
        --index_;
    } else {
        // iterator is at the first element, so decrement chunkPointer
        // and set index to be the length -1 of that chunk's length
        --chunkPointer_;
        index_ = chunkPointer_->length_ - 1;
    }
    return *this;
}

// Iterator dereference
ChunkyString::ConstIterator::reference ChunkyString::ConstIterator::
operator*() const {
    return (chunkPointer_->chars_[index_]);
}

// Iterator equals
bool ChunkyString::ConstIterator::operator==(const ConstIterator& rhs) const {
    // see if both data members of both iterators are equal
    if (this->chunkPointer_ == rhs.chunkPointer_
        && this->index_ == rhs.index_) {
        return true;
    }
    return false;
}

// Iterator not equals
bool ChunkyString::ConstIterator::operator!=(const ConstIterator& rhs) const {
    // return !=
    return !(*this == rhs);
}
