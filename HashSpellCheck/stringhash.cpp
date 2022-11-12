/**
 * \file stringhash.hpp
 * \author What are your CS70 aliases?
 *
 * \brief Hash function for strings
 */

#include "stringhash.hpp"

using std::string;

// Hash Function Gallery
//
// These are the hash functions our group has examined, one of them is used
// as myhash (see later code in the file)
//
// All the code below is in an anonymous namespace, so these function names
// are hidden from other code.
namespace {

/**
* A minimally acceptable hash function from class that takes advantage of
 * a prime number to assign buckets. Hope that the user doesn't choose
 * a number of buckets that is easily divisible by 37
 */
constexpr unsigned long HASH_MULTIPLIER = 37;

unsigned long hash1(const string& str) {
  unsigned long hv = 0;
  for (unsigned char c : str) {
    hv = hv * HASH_MULTIPLIER + c;
  }
  return hv;
}

  /**
  * sdbm - this algroithm was created for sdbm, a database library
  * It is a good general hash function and uses the magic, prime constant
  * 6599. This implementation uses bitshifting for efficency.
  */
  unsigned long hash2(const string& str) {
    unsigned long hash = 0;

    for (unsigned char c : str) {
      hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
  }

  /**
  * A hash function by Dan Bernstein that uses bitshifting and the magic 
  * number 33
  */
  unsigned long hash3(const string& str) {
  unsigned long hash = 5381;

  for (unsigned char c : str) {
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }
  return hash;
  }
}  // end of anonymous namespace

size_t myhash(const string& str) {
  // FIXME: Make this code call your favorite of the ones you've defined
  //        above.
  return hash1(str);
}

// You don't have to fully understand this code, but it is used to provide a
// table used by stringhash-test.cpp, all you need to do is list the name
// of your hash function (for printing) and the actual function name from
// above.
std::initializer_list<HashFunctionInfo> hashInfo = {
    {"InClass", hash1},
    {"djb2", hash3},
    {"sdbm", hash2}  // No comma for last one
};
