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
 * This is a length-based hash function, a hash function based purely on
 * string length.  It's deliberately terrible.
 *
 * /remarks CS 70 Note: This is a dumb hash function as a placeholder
 *          for one you might actually find.  You should replace it.
 *
 */
size_t simpleLengthHash(const string& str) { return str.length(); }

/**
 * This is a simple addition hash function.  It's deliberately terrible.
 *
 * /remarks CS 70 Note: This is a dumb hash function as a placeholder
 *          for one you might actually find.  You should replace it.
 *
 */
size_t simpleAddHash(const string& str) {
  size_t hash = 0;

  for (unsigned char c : str) hash = hash + c;

  return hash;
}

/**
 * This is a simple multiplication hash function.  It's deliberately bad.
 *
 * /remarks CS 70 Note: This is a dumb hash function as a placeholder
 *          for one you might actually find.  You should replace it.
 *
 */
size_t simpleMultHash(const string& str) {
  size_t hash = 1;

  for (unsigned char c : str) hash = hash * c;

  return hash;
}

}  // end of anonymous namespace

size_t myhash(const string& str) {
  // FIXME: Make this code call your favorite of the ones you've defined
  //        above.
  return simpleMultHash(str);
}

// You don't have to fully understand this code, but it is used to provide a
// table used by stringhash-test.cpp, all you need to do is list the name
// of your hash function (for printing) and the actual function name from
// above.
std::initializer_list<HashFunctionInfo> hashInfo = {
    {"Simple Length (Dummy)", simpleLengthHash},
    {"Simple Add (Dummy)", simpleAddHash},
    {"Simple Mult (Dummy)", simpleMultHash}  // No comma for last one
};
