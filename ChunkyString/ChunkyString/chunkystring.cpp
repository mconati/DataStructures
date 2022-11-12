/**
 * \file chunkystring.cpp
 * author: CS70 Starter Code
 *
 * \brief Contains implementations of ChunkyString member functions
 */

#include <iostream>
#include <string>

#include "chunkystring.hpp"


// Default Constructor
ChunkyString::ChunkyString() : size_{0}, chunks_{} {
    // nothing left to do
}

// Copy Constructor
ChunkyString::ChunkyString(const ChunkyString& other) {
    chunks_.clear();
    size_ = 0;
// Uses push_back to copy each element and increment size (deep copy)
    if (!other.isEmpty()) {
        for (ChunkyString::ConstIterator i = other.begin();
        i != other.end(); ++i) {
            push_back(*i);
        }
    }
}

// Assignment operator
ChunkyString& ChunkyString::operator=(const ChunkyString& other) {
    chunks_.clear();
    size_ = 0;
// Uses push_back to copy each element and increment size (deep copy)
    if (!other.isEmpty()) {
        for (ChunkyString::ConstIterator i = other.begin();
        i != other.end(); ++i) {
            push_back(*i);
        }
    }

    return *this;
}

// appends a character to the ChunkyString
void ChunkyString::push_back(char c) {
    // If a chunkystring is empty or the last chunk is full, make a new chunk
    // and add the character there
    if (size_ == 0 || ((chunks_.back().length_) == Chunk::CHUNKSIZE)) {
        Chunk newChunk;
        chunks_.push_back(newChunk);
        chunks_.back().chars_[0] = c;
        chunks_.back().length_++;

    // Otherwise, just add the character after the last element in the final
    // Chunk
    } else {
        size_t insertLocation = chunks_.back().length_;
        chunks_.back().chars_[insertLocation] = c;
        chunks_.back().length_++;
    }
    ++size_;
}

// Returns an iterator to the begining of the ChunkyString
ChunkyString::iterator ChunkyString::begin() {
    Iterator begin = Iterator{chunks_.begin(), 0};
    return begin;
}

// Returns an iterator to the end of the ChunkyString
ChunkyString::iterator ChunkyString::end() {
    return Iterator{chunks_.end(), 0};
}

// Returns a Constant iterator to the begining of the ChunkyString
ChunkyString::const_iterator ChunkyString::cbegin() const {
    ConstIterator begin = ConstIterator{chunks_.begin(), 0};
    return begin;
}

// Returns a Constant iterator to the end of the ChunkyString
ChunkyString::const_iterator ChunkyString::cend() const {
    return ConstIterator{chunks_.end(), 0};
}

// Append operator
ChunkyString& ChunkyString::operator+=(const ChunkyString& rhs) {
    ChunkyString copy = rhs;
// Makes a deep copy by pushing back each of the elements from rhs
    for (Iterator i = copy.begin(); i != copy.end(); ++i) {
        push_back(*i);
    }
    return *this;
}

// Equality operator
bool ChunkyString::operator==(const ChunkyString& rhs) const {
    // Checks that sizes are equivalent first
    if (size_ != rhs.size_) {
        return false;
    }
    // Checks the edge case of an empty ChunkyString
    if (size_ == 0) {
        return true;
    }
    // Since we know that sizes are equivalent, we now iterate through
    // Each ChunkyString and compare each element for equality
    const_iterator lhsIterator = cbegin();
    const_iterator rhsIterator = rhs.cbegin();
    for (size_t i = 0; i < size_; ++i) {
        if ((*lhsIterator) != (*rhsIterator)) return false;
        ++rhsIterator;
        ++lhsIterator;
    }
    return true;
}
// Uses our equality operator to check inequality
bool ChunkyString::operator!=(const ChunkyString& rhs) const {
    return !(*this == rhs);
}

// Loops through a ChunkyString, adding each element to a printable ostream&
std::ostream& ChunkyString::print(std::ostream& out) const {
    for (ChunkyString::ConstIterator i = cbegin(); i != cend(); ++i) {
        out << *i;
    }
    return out;
}

std::ostream& ChunkyString::printChunks(std::ostream& out) const {
    for (std::_List_const_iterator<ChunkyString::Chunk> i = chunks_.cbegin();
    i != chunks_.end(); ++i) {
        out << "Length: " << i->length_ << " - ";
        for (size_t j = 0; j < i->length_; ++j) {
            out << i->chars_[j];
        }
        out << std::endl;
    }
    return out;
}

// Takes advantage of the STL lexicographical compare to determine whether
// a ChunkyString is less than another.
bool ChunkyString::operator<(const ChunkyString& rhs) const {
    return std::lexicographical_compare(begin(), end(), rhs.begin(), rhs.end());
}

// Returns size
size_t ChunkyString::size() const {
    return size_;
}

// Default Chunk Constructor
ChunkyString::Chunk::Chunk() : length_{0} {
    // nothing left to do
}

// 1 Param Chunk Constructor
ChunkyString::Chunk::Chunk(size_t length) : length_{length} {
    // nothing left to do
}

/*
  Insert a character and return the index of the insertee. If the insertion
  is at the end of a chunk, just push back. Otherwise, if the chunk is full
  split the chunk into the current chunk and a new chunk. Push half the
  elements to the new chunk and fix length. 
  In any case, at this point, perform a regular insert. To do so,
  move all elements after the index down one and insert the insertee
  into the correct index. Fix size and return the iterator.
*/
ChunkyString::Iterator ChunkyString::insert(ChunkyString::Iterator i, char c) {
    // if the insertion is at the end, this should be equivalent to a push back
    if (i == end()) {
        push_back(c);
        iterator toReturn = end();
        --toReturn;
        return toReturn;
    }
    // If the chunk is full, make a new chunk
    if (i.chunkPointer_->length_ >= (Chunk::CHUNKSIZE)) {
        std::list<Chunk>::iterator nextChunk = i.chunkPointer_;
        ++nextChunk;
        Chunk newChunk = Chunk{0};
        nextChunk = chunks_.insert(nextChunk, newChunk);

        // push half the elements onto the new chunk and fix lengths
        for (size_t n = 0; n < Chunk::CHUNKSIZE/2; ++n) {
            nextChunk->chars_[n] =
            i.chunkPointer_->chars_[Chunk::CHUNKSIZE/2 + n];
            --(i.chunkPointer_->length_);
            ++(nextChunk->length_);
        }
        // fix the iterator if the index is now pointing to a different chunk
        if (i.index_ > Chunk::CHUNKSIZE/2) {
            i = iterator{nextChunk, i.index_ - Chunk::CHUNKSIZE/2};
        }
    }

    // now we have chunks that aren't full, so we can perform a regular insert
    for (size_t n = i.chunkPointer_->length_; n > i.index_; --n) {
        i.chunkPointer_->chars_[n] = i.chunkPointer_->chars_[n - 1];
    }
    // insert the new character
    i.chunkPointer_->chars_[i.index_] = c;
    // adjust length, size and return
    ++(i.chunkPointer_->length_);
    ++size_;
    return i;
}
/*
    Erases the character at the iterator and returns the index of the next char

    Case 1: If the chunk has one element, simply delete the chunk. 
    Case 2: If the erasee is last in a chunk, delete it and change the length
    Case 3: If the character is in the beginning or the middle of the a chunk,
            bump all of the elemtents up 1 and decrement length
    
    Finally, for cases 2 and 3 call reorder to optimize utlization.
*/
ChunkyString::Iterator ChunkyString::erase(ChunkyString::Iterator i) {
    // if the chunk has one element, delete the chunk!
    if (i.chunkPointer_->length_ == 1) {
        --size_;
        std::list<Chunk>::iterator deletedChunk =
        chunks_.erase(i.chunkPointer_);
        // return an iterator to the place of the deleted chunk
        return iterator{deletedChunk, 0};
    // otherwise, if the erasee is last in a chunk, delete it and change length
    } else {
        if (i.index_ == (i.chunkPointer_->length_ - 1)) {
            --(i.chunkPointer_->length_);
            i = Iterator{i.chunkPointer_, i.index_};
        // finally, if the character is at the beginning or in the middle of
        // a chunk, bump all other elements up 1 and decrement length
        } else {
            for (size_t n = i.index_; n < i.chunkPointer_->length_- 1; ++n) {
                i.chunkPointer_->chars_[n] = i.chunkPointer_->chars_[n + 1];
            }
            --(i.chunkPointer_->length_);
        }
        // decrement size
        --size_;
        // call this function to keep utilization low
        i = reorder(i);
    }
    return i;
}

// redorders the chunkystring to maximize utilization
ChunkyString::Iterator ChunkyString::reorder(ChunkyString::Iterator i) {
    // We call this when we erase. We have two alternatives
    // Set up iterators for the alternatives
    std::list<Chunk>::iterator previousChunk = i.chunkPointer_;
    if (i.chunkPointer_ != chunks_.begin()) {
        --previousChunk;
    }
    std::list<Chunk>::iterator nextChunk = i.chunkPointer_;
    ++nextChunk;

    // Option 1: the chunk we just erased from not at the beginning
    // and there is space for it to merge with the previous chunk
    if (i.chunkPointer_ != chunks_.begin() && (i.chunkPointer_->length_ +
     previousChunk->length_) <= Chunk::CHUNKSIZE) {
        // merge with the previous chunk
        for (size_t n = 0; n < i.chunkPointer_->length_; ++n) {
            previousChunk->chars_[previousChunk->length_ + n] =
            i.chunkPointer_->chars_[n];
        }
        // fix iterators and length.
        ChunkyString::Iterator toReturn = iterator{previousChunk,
        previousChunk->length_ + i.index_};
        previousChunk->length_ += i.chunkPointer_->length_;
        // remove the now empty chunk
        chunks_.erase(i.chunkPointer_);
        return toReturn;
    // Option 2: the chunk we just erased from is not at the end
    // and there is space for it to merge with the next chunk
    } else if (i.chunkPointer_ != (--chunks_.end()) &&
    (i.chunkPointer_->length_ + nextChunk->length_) <= Chunk::CHUNKSIZE) {
        // add the next chunk to this chunk
        for (size_t n = 0; n < nextChunk->length_; ++n) {
            i.chunkPointer_->chars_[i.chunkPointer_->length_ + n] =
            nextChunk->chars_[n];
        }
        // fix lengths
        i.chunkPointer_->length_ += nextChunk->length_;
        // remove the now empty chunk
        chunks_.erase(nextChunk);

    // Otherwise, do nothing
    }
    return i;
}

// returns the utilization of a chunkystring
double ChunkyString::utilization() const {
    double size = static_cast<double> (size_);
    double capacity = static_cast<double> (chunks_.size() * Chunk::CHUNKSIZE);
    return size / capacity;
}

// Returns true for an empty ChunkyString
bool ChunkyString::isEmpty() const {
    return (size_ == 0);
}
// Here's a free function that you can use for debugging.
//  If you have a test of a ChunkyString (TestingString) that fails,
//    it is often useful to add debugging output to the test code, e.g.,
//         std::cerr << "Before: " << s << std::endl;
//         s.push_back('*');
//         std::cerr << "After: " << s << std::endl;
//  But sometimes bugs occur only when a chunk is full, or becomes
//     empty, or has exactly 2 chars, and normal printing (by design)
//     just shows the characters, not the chunk structure, making
//     these bugs hard to diagnose
//  The solution in such cases is to call the dump method, e.g.,
//         s.dump("Before: ");
//         s.push_back('*');
//         s.dump("After: ");
//  Then you can tell whether the ChunkyString "abc" is really
//         |3:abc|         -- 3 chars in one chunk
//         |2:ab|1:c|      -- 2 chars in 1st chunk, 1 char in 2nd
//         |1:a|1:b|1:c|   -- 1 char in each of 3 chunks
//  or whether the problem is that s is just corrumpted, e.g.,
//         |2:ab|0:|0:|1:c|   -- some empty chunks in the middle
//         |3:abc|-9:|        -- a chunk with negative length_ !?
//
//  Alternatively, rather than adding output to the tests, you might
//     (temporarily!) add debugging output to your member functions.
//     For example, if you think there's a problem in operator+=
//     you can change the code for that member function to:
//
//          ChunkyString& operator+=(const ChunkyString& rhs) {
//              dump("LHS before append: ");
//              rhs.dump("RHS before append: ");
//              ...code to do the append...
//              dump("LHS after append : ");
//              return *this;
//          }
//
void ChunkyString::dump(const std::string& label) const {
    // Print the label so that we know where this output came from
    std::cerr << label << "|";
    // print size and contents of each chunk.
    for (const Chunk& chunk : chunks_) {
        std::cerr << chunk.length_ << ":";
        for (size_t i = 0; i < chunk.length_; ++i) {
            std::cerr << chunk.chars_[i];
        }
        std::cerr << "|";
    }
    std::cerr << "\n";
}

// Add code for all the other member functions mentioned
// in the header file...
