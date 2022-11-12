/**
 * \file intlist.hpp
 * \authors YOUR ALIASES HERE
 * \brief A list of ints.
 *
 */

#ifndef INTLIST_HPP_INCLUDED
#define INTLIST_HPP_INCLUDED 1

#include <cstddef>
#include <iostream>

/**
 * \class IntList
 *
 * \brief A linked list of many Element objects which hold \c ints.
 *
 * \details Class allocates memory dynamically; thus can't use C++'s
 *          defaults for copy constructor, assignment operator and
 *          destructor.
 */
class IntList {

private:
    // Forward declaration of private class.
    class Iterator;

public:
    IntList();
    IntList(const IntList& orig);
    IntList& operator=(const IntList& rhs);
    ~IntList();
    void swap(IntList& rhs);

    void push_front(int pushee); ///< Push onto head of list
    void push_back(int pushee);  ///< Push onto tail of list
    int pop_front();             ///< Drop & return the head element

    size_t size() const; ///< Size of the list
    bool empty() const;  ///< true if the list is empty

    bool operator==(const IntList& rhs) const;
    bool operator!=(const IntList& rhs) const;

    // Allow clients to iterate over the contents of the list.
    using iterator = Iterator;
    iterator begin() const; ///< An iterator that refers to the first element
    iterator end() const;   ///< An invalid / "past-the-end" iterator

    /**
     * Insert a value after a given iterator
     *
     * \details The iterator cannot be end()
     */
    void insert_after(iterator where, int value);

private:
    /***
     * \struct Element
     *
     * \brief The list is stored as a linked list of Elements.
     *        The class is private so only IntList knows about it.
     *        A constructor is provided for convenience.
     *
     * \details The Copy Constructor and Assignment operator are
     *          disabled.
     */
    struct Element {
        // Data members
        int value_;
        Element* next_;

        // Convenience constructor
        Element(int value, Element* next);

        // Disabled functions
        Element() = delete;
        Element(const Element&) = delete;
        Element& operator=(const Element&) = delete;
        ~Element() = default; // If you don't like this, you can change it.
    };

    Element* back_;  ///< Current tail of list
    Element* front_; ///< Current head of list
    size_t size_;    ///< Current size of list

    bool consistent() const;

    /***
     * \class Iterator
     * \brief STL-style iterator for IntList.
     */
    class Iterator {
    public:
        // Definitions that are required for this class to be a well-behaved
        // STL-style iterator that moves forward through a collection of ints.
        using value_type        = int;
        using reference         = value_type&;
        using pointer           = value_type*;
        using difference_type   = ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        // Provide all the usual operations for a forward iterator

        Iterator() = default;
        Iterator(const Iterator&) = default;
        Iterator& operator=(const Iterator&) = default;
        ~Iterator() = default;

        Iterator& operator++();
        int& operator*() const;
        bool operator==(const Iterator& rhs) const;
        bool operator!=(const Iterator& rhs) const;

    private:
        friend class IntList;
        Iterator(Element* current); ///< Friends create non-default iterators
        Element* current_;          ///< The current list node
    };
};

/// Provide a non-member version of swap to allow standard swap(x,y) usage.
void swap(IntList& lhs, IntList& rhs);

#endif // INTLIST_HPP_INCLUDED
