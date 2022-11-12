/*
 * \file intlist.cpp
 * \authors YOUR ALIASES HERE
 * \brief Implemenation of IntList and its private classes.
 */

#include <utility>

#include "intlist.hpp"
#include "testing-logger.hpp"

IntList::IntList() : back_(nullptr), front_(nullptr), size_(0) {
  affirm(consistent() && empty());
}

IntList::~IntList() {
  while (!empty()) {
    pop_front();
  }
}


IntList::IntList(const IntList& orig)
: back_(nullptr), front_(nullptr), size_(0) {
  for (IntList::iterator copyHelper = orig.begin();
  copyHelper != orig.end(); ++copyHelper) {
    push_back(*copyHelper);
  }
  affirm(consistent());
}

void IntList::swap(IntList& rhs) {
  // This is the canonical way to swap arbitrary types; this incantation
  // will call std::swap unless we're swapping a class that provides its
  // own swap function (like we do), in which case that swap is called.
  // Here we end up calling std::swap since the swapped parts are primitives.

  using std::swap;

  swap(back_, rhs.back_);
  swap(front_, rhs.front_);
  swap(size_, rhs.size_);
  affirm(consistent());
}

void swap(IntList& lhs, IntList& rhs) { lhs.swap(rhs); }

IntList& IntList::operator=(const IntList& rhs) {
  // Assignment is implemented idiomatically for C++, using "the swap trick"
  IntList copy = rhs;
  swap(copy);

  affirm(consistent());
  return *this;
}


size_t IntList::size() const { return size_; }

bool IntList::empty() const {
  if (size_ == 0 && front_ == nullptr && back_ == nullptr) {
    return true;
  }
  return false;
}


bool IntList::operator==(const IntList& rhs) const {
  if (size() == rhs.size()) {
    IntList::iterator rhsIterator = rhs.begin();
    for (IntList::iterator lhsIterator = begin(); lhsIterator !=
    end(); ++lhsIterator) {
      if (*rhsIterator != *lhsIterator) {
        return false;
      }
      ++rhsIterator;
    }
    return true;
  }
  return false;
}

bool IntList::operator!=(const IntList& rhs) const {
  // Idiomatic code: leverage == to implement !=
  return !operator==(rhs);
}


void IntList::push_front(int pushee) {
  if (empty()) {
    front_ = new Element(pushee, nullptr);
    back_ = front_;
  } else {
    Element* oldFront = front_;
    front_ = new Element(pushee, oldFront);
  }
  ++size_;
  affirm(consistent() && !empty());
}

int IntList::pop_front() {
  if (!empty()) {
    Element* currentFront = front_;
    int returnValue = currentFront->value_;
    if (size() > 1) {
      Element* newFront = currentFront->next_;
      front_ = newFront;
      delete currentFront;
    } else {
      delete currentFront;
      front_ = nullptr;
      back_ = nullptr;
    }
    --size_;
    return returnValue;
  }
  affirm(consistent());
  return 0;
}


void IntList::push_back(int pushee) {
  if (empty()) {
    push_front(pushee);
  } else {
    Element* newBack = new Element(pushee, nullptr);
    back_->next_ = newBack;
    back_ = newBack;
    ++size_;
  }
  affirm(consistent());
}


void IntList::insert_after(iterator where, int value) {
  Element* currentNext = where.current_->next_;
  Element* newElement = new Element(value, currentNext);
  where.current_->next_ = newElement;
  ++size_;
  affirm(consistent() && !empty());
}


IntList::iterator IntList::begin() const {
  return Iterator(front_);
}



IntList::iterator IntList::end() const {
  return Iterator(nullptr);
}


bool IntList::consistent() const {
  return ((front_ == nullptr) && (back_ == nullptr) && (size_ == 0)) ||
         ((front_ == back_) && (size_ == 1)) ||
         ((front_ != back_) && (size_ > 1));
}

// --------------------------------------
// Implementation of IntList::Element
// --------------------------------------

IntList::Element::Element(int value, Element* next)
    : value_(value), next_(next) {
  // Nothing else to do.
}

// --------------------------------------
// Implementation of IntList::Iterator
// --------------------------------------

IntList::Iterator::Iterator(Element* current) : current_(current) {
  // Nothing else to do.
}

IntList::Iterator& IntList::Iterator::operator++() {
  current_ = current_->next_;
  return *this;
}

int& IntList::Iterator::operator*() const {
  int& returnInt = current_->value_;
  return returnInt;
}



bool IntList::Iterator::operator==(const Iterator& rhs) const {
  return (rhs.current_ == current_);
}

bool IntList::Iterator::operator!=(const Iterator& rhs) const {
  // Idiomatic code: leverage == to implement !=
  return !(*this == rhs);
}
