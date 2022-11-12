/* These are the headers that this code needs, in addition to your hashset
 * header.
 */

#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>

/* If you need prime numbers close to a power of two, add this
   it to your HashSet class as a private
   *static* member function (i.e., declared static in the header file
   that declares the HashSet class template).  It is "static" because
   it does not need or use the "this" data for a HashSet object.

   It *must* be part of the HashSet class because otherwise everyone
   who uses your hash tables would have to remember to link in
   this code as an extra .o file, which would be incredibly annoyting.
 */

template <typename T>
size_t HashSet<T>::primeAbove(size_t power) {
  // Table of differences between nearest prime and power of two. We
  // use unsigned char (i.e., an 8-bit unsigned int) to make the
  // table tiny.
  static unsigned char offset[] = {
    0, 0, 1, 3, 1, 5, 3, 3, 1, 9, 7, 5, 3, 17, 27, 3, 1,
    29, 3, 21, 7, 17, 15, 9, 43, 35, 15, 29, 3, 11, 3, 11,
    15, 17, 25, 53, 31, 9, 7, 23, 15, 27, 15, 29, 7, 59, 15,
    5, 21, 69, 55, 21, 21, 5, 159, 3, 81, 9, 69, 131, 33, 15, 135, 29
  };
  assert(power > 0 && power < 64);
  return (static_cast<size_t>(1) << power) + offset[power];
}
