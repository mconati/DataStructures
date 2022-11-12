/**
 * \file chunkystring.hpp
 *
 * \authors CS 70 given code, with additions by ... your aliases here ...
 *
 * \brief Declares the ChunkyString class.
 */

#ifndef CHUNKYSTRING_HPP_INCLUDED
#define CHUNKYSTRING_HPP_INCLUDED 1

#include <cstddef>
#include <iostream>
#include <list>
#include <string>

/**
 * \class ChunkyString
 * \brief Efficiently represents strings where insert and erase are
 *    constant-time operations.
 *
 * \details This class is comparable to a linked-list of characters,
 *   but more space efficient.
 *
 * \remarks
 *   reverse_iterator and const_reverse_iterator aren't
 *   supported. Other than that, we use the STL container type declarations
 *   such that STL functions are compatible with ChunkyString.
 */
class ChunkyString {
 private:
  // Forward declaration of Iterator and ConstIterator
  class Iterator;
  class ConstIterator;

 public:
  // Standard STL container type definitions
  using value_type = char;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = Iterator;
  using const_iterator = ConstIterator;
  // reverse_iterator and const_reverse_iterator aren't supported

  /**
   * \brief Default constructor
   * \details Creates an empty string
   *
   * \note constant time
   */
  ChunkyString();
  ChunkyString(const ChunkyString& other);
  ChunkyString& operator= (const ChunkyString& other);
  ~ChunkyString() = default;

  /// Return an iterator to the first character in the ChunkyString.
  iterator begin();

  /// Return an iterator to "one past the end"
  iterator end();

  /// Return a const_iterator to the first character in the ChunkyString.
  const_iterator cbegin() const;

  /// Return a const_iterator to "one past the end"
  const_iterator cend() const;

  /**
   * \brief Old-style begin() for constant ChunkyStrings
   *
   * \note Surprise! We actually can put code in class definitions
   *       as in Java. But there are drawbacks (including mixing
   *       iterface with implementation), so we generally do it
   *       only for trivial, uninteresting definitions.
   */
  const_iterator begin() const { return cbegin(); }

  /// Old-style end() for constant ChunkyStrings
  const_iterator end() const { return cend(); }

  /**
   * \brief Insert a character at the end of the ChunkyString.
   *
   * \param c     Character to insert
   *
   * \note constant time
   */
  void push_back(char c);
  ChunkyString::Iterator reorder(iterator i);
  size_t size() const;  ///< String size \note constant time

  bool operator==(const ChunkyString& rhs) const;  ///< String equality
  bool operator!=(const ChunkyString& rhs) const;  ///< String inequality
  bool isEmpty() const;

  std::ostream& print(std::ostream& out) const;  ///< String printing
  std::ostream& printChunks(std::ostream& out) const;  ///< String printing

  void dump(const std::string& label) const;  ///< Debug printing

  /**
   * \brief String concatenation
   *
   * \warning invalidates all iterators
   */
  ChunkyString& operator+=(const ChunkyString& rhs);

  /**
   * \brief String comparison
   *
   * \remarks String comparison is lexicographic, which means:
   *    + Two strings `s1` and `s2` are compared character by character.
   *    + The first characters that aren't equal determine the order. If the
   *      character value from `s1` is smaller than the corresponding one from
   *      `s2`, then `s1` < `s2`, and vice-versa.
   *    + If `s1` is a prefix of `s2`, then `s1` < `s2`, and vice-versa.
   *    + If `s1` and `s2` have exactly the same character, then neither is
   *      less than the other.
   *    + An empty string is less than any other string, except the empty
   *      string.
   * \see http://www.cplusplus.com/reference/string/string/compare/
   * \see http://en.cppreference.com/w/cpp/algorithm/lexicographical_compare
   */
  bool operator<(const ChunkyString& rhs) const;

  /**
   * \brief Insert a character before the character at i.
   * \details
   *   What makes ChunkyString special is its ability to insert and
   *   erase characters quickly while remaining space efficient.
   *
   * \param i     iterator to specify insertion point
   * \param c     character to insert
   *
   * \returns an iterator pointing to the newly inserted character.
   *
   * \note constant time
   *
   * \warning invalidates all iterators except the returned iterator
   */
  iterator insert(iterator i, char c);

  /**
   * \brief Erase a character at i
   * \details
   *   What makes ChunkyString special is its ability to insert and
   *   erase characters quickly while remaining space efficient.
   *
   * \param i     iterator pointing to the character to erase
   *
   * \returns an iterator pointing to the character after the one
   *   that was deleted.
   *
   * \note constant time
   *
   * \warning invalidates all iterators except the returned iterator
   *
   * \warning erasing from an empty string is undefined behavior
   */
  iterator erase(iterator i);

  /**
   * \brief Average capacity of each chunk, as a fraction
   *
   * \details
   *   This function computes the fraction of the ChunkyString's character
   *   cells that are in use. It is defined as
   *
   *   \f[\frac{\mbox{number of characters in the string}}
   *           {\mbox{number of chunks}\times\mbox{CHUNKSIZE}}  \f]
   *
   *   For reasonably sized strings (i.e., those with more than one or two
   *   characters), utilization should never fall to near one character per
   *   chunk; otherwise the data structure would be wasting too much space.
   *
   *   The utilization for an empty string is undefined (i.e., any value is
   *   acceptable).
   *
   * \note constant time
   */
  double utilization() const;

 private:
  // NOTE: You can choose to change Chunk to be a class rather than a struct
  /**
   * \struct Chunk
   * \brief Holds part of a ChunkyString
   */
  struct Chunk {
    /**
     * \brief Maximum size of a chunk
     *
     * \remarks Although we set the value of CHUNKSIZE here to be 12,
     *          that's an implementation detail. We're allowed to change
     *          it, and user's code (as well as our own implementation code)
     *          shouldn't depend on CHUNKSIZE having a particular value.
     */
    static const size_t CHUNKSIZE = 12;
    size_t length_;  ///< Number of characters occupying this chunk
    char chars_[CHUNKSIZE];  ///< Contents of this chunk
    Chunk();
    Chunk(size_t length);
    ~Chunk() = default;
  };

  /**
   * \class Iterator
   * \brief STL-style iterator for ChunkyString.
   *
   * \details Synthesized copy constructor, destructor, and assignment
   *          operator are okay.
   *
   *          The five typedefs and the member functions are such that
   *          the iterator works properly with STL functions (e.g., copy).
   *
   *          Since this is a bidirectional_iterator, `operator--`
   *          is provided and meaningful for all iterators except
   *          ChunkyString::begin.
   */
  class Iterator {
   public:
    /// Default constructor, to be STL-compliant; creates invalid iterator
    Iterator();


    // TODO: Do we need more constuctors? A destructor?
    //       An assignment operator?

    // Make Iterator STL-friendly with these typedefs:
    using difference_type   = ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = char;
    using const_reference = const value_type&;
    using reference = value_type&;
    using pointer = value_type*;
    /**
     *  The `chunk_iter_t` type below might be helpful for some
     *  implementations. If your implementation ever needs to declare
     *  a variable whose type is an iterator for a chunk list, you
     *  should use `chunk_iter_t` for the type name.
     */
    using chunk_iter_t = std::list<Chunk>::iterator;

    Iterator(chunk_iter_t chunkPtr, size_t i);
    Iterator& operator=(const Iterator& other);
    ~Iterator() = default;

    // Operations
    Iterator& operator++();
    Iterator& operator--();
    reference operator*() const;
    bool operator==(const Iterator& rhs) const;
    bool operator!=(const Iterator& rhs) const;


    chunk_iter_t chunkPointer_;
    size_t index_;

   private:
    friend class ChunkyString;

    // TODO: Add private member functions, including constructors, as needed
  };

  /**
   * \class ConstIterator
   * \brief STL-style constant iterator for ChunkyString.
   *
   * \details Synthesized copy constructor, destructor, and assignment
   *          operator are okay.
   *
   *          The five typedefs and the member functions are such that
   *          the iterator works properly with STL functions (e.g., copy).
   *
   *          Since this is a bidirectional_iterator, `operator--`
   *          is provided and meaningful for all iterators except
   *          ChunkyString::begin.
   *
   */
  class ConstIterator {
   public:
    /// Default constructor, to be STL-compliant; creates invalid iterator
    ConstIterator();

    // TODO: Do we need more constuctors? A destructor?
    //       An assignment operator?

    /// Convert a non-const iterator to a const-iterator, if necessary
    ConstIterator(const Iterator& i);
    ConstIterator& operator=(const ConstIterator& other);

    // Make Iterator STL-friendly with these typedefs:
    using difference_type = ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = char;
    using const_reference = const value_type&;
    using reference = const value_type&;
    using pointer = const value_type*;

    /**
     *  The `chunk_iter_t` type below might be helpful for some
     *  implementations. If your implementation ever needs to declare
     *  a variable whose type is an iterator for a chunk list, you
     *  should use `chunk_iter_t` for the type name.
     */
    using chunk_iter_t = std::list<Chunk>::const_iterator;

    ConstIterator(chunk_iter_t chunkPtr, size_t i);
    chunk_iter_t chunkPointer_;
    size_t index_;

    // Operations
    ConstIterator& operator++();
    ConstIterator& operator--();
    reference operator*() const;
    bool operator==(const ConstIterator& rhs) const;
    bool operator!=(const ConstIterator& rhs) const;

   private:
    friend class ChunkyString;

    // TODO: Add private data members, for encoding
    // TODO: Add private member functions, including constructors, as needed
  };

  // ChunkyString data members
  size_t size_;              ///< Length of the string
  std::list<Chunk> chunks_;  ///< Linked list of chunks
};



/**
 * \brief Print operator: displays a ChunkyString on the given stream
 *
 * \param out   the display stream
 * \param text  a ChunkyString to display
 *
 * \returns the display stream
 *
 * \remarks
 *   This "global function" makes it possible for ChunkyString users to write
 *   `cout << s` (where `s` is a ChunkyString). The implementation of this
 *   function just calls ChunkyString's `print` member function. We use the C++
 *   keyword `inline` because we're making the rare choice to put the
 *   implementation of a function in the header file.
 */
inline std::ostream& operator<<(std::ostream& out, const ChunkyString& text) {
  return text.print(out);
}

#endif  // CHUNKYSTRING_HPP_INCLUDED
