// our-tests.cpp
// Sean Hoerger and Marco Conati

// Don't put any includes before this one!  (Otherwise it won't be as good a
// test of whether the hashset is self sufficient.)  If you make another
// test file, it'll be okay to add other includes, like stringhash.hpp, but
// don't do that here.


#include <cstddef>  // For size_t
#include <ostream>  // For std::ostream

#include "signal.h"  // For signal
#include "unistd.h"  // For alarm


constexpr size_t HASH_MULTIPLIER = 37;
/// Hash function for string
size_t myhash(const std::string& str) {
  unsigned long hv = 0;
  for (unsigned char c : str) {
    hv = hv * HASH_MULTIPLIER + c;
  }
  return hv; 
}
#include "hashset.hpp"
// Explicitly instantiate the entire templated class (normally C++ is lazy
// and only instantiates the things it needs as it needs them, here we make
// it do everything.)

template class HashSet<std::string>;

/*****************************************************
 * Functionality Tests
 *
 *      Functionality tests for HashSet
 * ***************************************************/

#include "testing-logger.hpp"
// check exist, size, maximal, and insert
bool ourTest() {
  // Set up the TestingLogger object
  TestingLogger log{"size test"};

  std::string bessie = "bessie";
  std::string mooody = "mooody";
  std::string paco = "paco";
  HashSet<std::string> cowNamesSet;

  // make sure it is empty
  affirm(!cowNamesSet.exists(bessie));
  affirm(!cowNamesSet.exists(mooody));
  affirm(!cowNamesSet.exists(paco));

  // make sure size increases as we insert cows
  cowNamesSet.insert(bessie);
  affirm(cowNamesSet.size() == 1);
  cowNamesSet.insert(mooody);
  affirm(cowNamesSet.size() == 2);
  cowNamesSet.insert(paco);
  affirm(cowNamesSet.size() == 3);

  // make sure the cows were inserted
  affirm(cowNamesSet.exists(bessie));
  affirm(cowNamesSet.exists(mooody));
  affirm(cowNamesSet.exists(paco));

  // affirm that the largest bucket has at least one cow
  affirm(cowNamesSet.maximal() >= 1);
  
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
  affirm(ourTest());

  if (alltests.summarize(true)) {
    return 0;  // Error code of 0 == Success!
  } else {
    return 2;  // Arbitrarily chosen exit code of 2 means tests failed.
  }
}
