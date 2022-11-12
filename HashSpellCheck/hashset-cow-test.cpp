/**
 * \file hashset-cow-test.cpp
 * \author CS 70 Provided Code -- don't add to this file, make a new one.
 *
 * \brief Tests a hashset to ensure that it is properly generic and doesn't
 *        contain any hidden dependencies on being a hashtable of strings.
 *        Does not extensively test HashSet functionality.
 */

/*****************************************************
 * Compatability Tests
 *
 *      Test for any hidden dependencies of HashSet on
 *      functions of the template type or included
 *      files.
 * ***************************************************/

// Don't put any includes before this one!  (Otherwise it won't be as good a
// test of whether the hashset is self sufficient.)  If you make another
// test file, it'll be okay to add other includes, like stringhash.hpp, but
// don't do that here.
#include "hashset.hpp"

#include <cstddef>  // For size_t
#include <ostream>  // For std::ostream

#include "signal.h"  // For signal
#include "unistd.h"  // For alarm

/**
 * \class Cow
 * \brief
 *   Test class which implements the bare minimum to be usable with a HashSet.
 * \remarks
 *   As this class is very brief and intended purely for testing
 *   purposes, we keep it completely contained within this file. Doing
 *   so on production level code is problematic.
 */
class Cow {
 public:
  /// Non-default constructor.
  // (Classes used with hash tables aren't required to have a default
  //  constructor.)
  explicit Cow(size_t numHats);

 private:
  size_t numHats_;  ///< How many hats does our Cow have?

  // Give external functions friend access
  friend std::ostream& operator<<(std::ostream& out, const Cow& printMe);
  friend bool operator==(const Cow& lhs, const Cow& rhs);
};

// But classes used with hash tables must also have an equality operator.
// Here, I've defined it as a non-member function.

/// Equality for Cows
bool operator==(const Cow& lhs, const Cow& rhs) {
  return lhs.numHats_ == rhs.numHats_;
}

// As an extra, we define an output operator for the Cow type.
//
// NOTE: Cow isn't *required* to have an output operator, but it is reasonable
// for the hash table to have a (non-required) debugging function that prints
// out the hash table.  To *use* such a function Cows need to have printing
// defined.

/// Output a Cow in visual form
std::ostream& operator<<(std::ostream& out, const Cow& printMe) {
  for (size_t i = 0; i < printMe.numHats_; ++i) out << "        _====_\n";

  out << "         (__)\n"   // Shamelessly taken from Debian's (& Ubuntu's)
      << "         (oo)\n"   //    apt-get moo
      << "   /------\\/ \n"  // (apt has "super cow" powers)
      << "  / |    ||  \n"
      << " *  /\\---/\\  \n"
      << "    ~~   ~~  \n";
  return out;
}

Cow::Cow(size_t numHats) : numHats_(numHats) {
  // Nothing (else) to do.
}

/// Hash function for Cows
size_t myhash(const Cow&) {
  return 42;  // Really bad hash function. ;-)
}

// Explicitly instantiate the entire templated class (normally C++ is lazy
// and only instantiates the things it needs as it needs them, here we make
// it do everything.)

template class HashSet<Cow>;

/*****************************************************
 * Functionality Tests
 *
 *      Functionality tests for HashSet
 * ***************************************************/

#include "testing-logger.hpp"

/// Test insert and exists for cows
bool cowTestSuite() {
  // Set up the TestingLogger object
  TestingLogger log{"example test"};

  Cow mabel(3);
  HashSet<Cow> cowSet;

  affirm(!cowSet.exists(mabel));

  cowSet.insert(mabel);
  affirm(cowSet.exists(mabel));

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

/****************************
 * Boilerplate main function
 * **************************/

// Called if the test runs too long.
static void timeout_handler(int) {
  // We go super-low-level here, because we can't trust anything in
  // the C/C++ library to really be working right.
  write(STDERR_FILENO, "Timeout occurred!\n", 18);
  abort();
}

/// Run tests
int main() {
  // Initalize testing environment
  TestingLogger alltests{"All tests"};

  signal(SIGALRM, timeout_handler);  // Call this when the timer expires
  alarm(10);                         // set the timer at 10 seconds

  // Add calls to your tests here...
  affirm(cowTestSuite());
  if (alltests.summarize(true)) {
    return 0;  // Error code of 0 == Success!
  } else {
    return 2;  // Arbitrarily chosen exit code of 2 means tests failed.
  }
}