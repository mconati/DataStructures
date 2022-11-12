/**
 * \file stringtest.cpp
 *
 * \brief Tests a ChunkyString for correctness.
 */

#include "testing-logger.hpp"
#ifndef LOAD_GENERIC_STRING
/* This test code can be used two ways.  It can either test a local
 * ChunkyString implementation, or it can dynamically load an implementation
 * at runtime and test that.  In the first homework, you'll dynamically load an
 * implementation that someone else used, but in later assignments, you'll
 * set the value to zero to use your own.
 */
#define LOAD_GENERIC_STRING 0  // 0 = Normal, 1 = Load Code Dynamically
#endif

#if LOAD_GENERIC_STRING
//#include "string-wrapper.hpp"  // Use dynamic loading magic! You're not sneeky here eithr
using TestingString = GenericString;
#else
#include "chunkystring.hpp"  // Just include and link as normal.
using TestingString = ChunkyString;
#endif

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "signal.h"
#include "unistd.h"

using std::cerr;
using std::endl;

static const size_t TESTING_CHUNKSIZE = 12;  // Assuming a chunksize of 12

// Helper functions
/**
 * \brief Assuming chunks are supposed to be at least an average of
 *        1/divisor full, checks for the lowest allowable utilization
 *        for the input string
 *
 * \remarks A helper function for affirming a TestingString's utilization
 *          is at least 1/divisor. E.g., to check for adherence to 1/2,
 *          divisor would be 2. The function does so by calculating the
 *          lowest allowable utilization for a string the length of the
 *          input string, including handling the edge cases of small strings.
 *          Since checkUtilization is not a test on its own, but rather
 *          a helper function to be used in other tests, it doesn't
 *          create its own TestingLogger object. Instead, its affirms
 *          will be associated with the TestingLogger of the calling
 *          function.
 *
 * \param test          TestingString to check
 * \param divisor       Expected fullness of chunk = 1/divisor
 */
void checkUtilization(const TestingString& test, size_t divisor) {
  double utilLimit = 0;

  if (test.size() > 0) {
    size_t chunks = 1;
    size_t size = test.size() - 1;
    divisor = TESTING_CHUNKSIZE / divisor;
    chunks += (size + divisor - 1) / divisor;
    utilLimit = static_cast<double>(size) /
                   static_cast<double>(chunks * TESTING_CHUNKSIZE);
    affirm(test.utilization() >= utilLimit);
  }
}

// Testing functions

bool defaultConstructorTest() {
  // Set up the TestingLogger object
  TestingLogger log("Default Constructor test");
  
  TestingString s1;

  // Testing that default constructed TestingString is valid
  affirm(s1.end() == s1.begin());

  affirm(s1.size() == 0);

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

bool equalsTest() {
  // Set up the TestingLogger object
  
  TestingLogger log("Equals test");
  TestingString s1;
  TestingString s2;
  TestingString s3;
  TestingString s4;
  s3.push_back('a');
  s4.push_back('a');

  affirm(s1 == s2);  // check empty
  affirm(s3 == s4);  // check non empty
  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

bool notEqualsTest() {
  // Set up the TestingLogger object
  TestingLogger log("Not equals test");

  TestingString s1;
  TestingString s3;
  TestingString s4;

  s3.push_back('a');
  s4.push_back('a');
  s4.push_back('b');

  affirm(s1 != s3);  // check empty vs non empty
  affirm(s3 != s4);  // check non empty vs non empty

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

bool pushBackTest() {
  // Set up the TestingLogger object
  
  TestingLogger log("Push back test");
  TestingString s1;
  TestingString s2;

  s1.push_back('a');
  s2.push_back('a');

  // make sure size increments when pushing back on empty TestingString
  affirm(s1.size() == 1);
  affirm(s2.size() == 1);
  s2.push_back('b');

  // make sure size increments when pushing back on a nonempty object
  affirm(s2.size() == 2);
  s2.push_back('c');

  // make sure size increments when pushing back on a nonempty object
  // of size greater than 1.
  affirm(s2.size() == 3);

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

bool sizeTest() {
  // Set up the TestingLogger object
  TestingLogger log("Size test");

  TestingString s1;
  TestingString s2;

  // confirm size is zero when empty
  affirm(s1.size() == 0);
  affirm(s2.size() == 0);
  s1.push_back('a');
  s2.push_back('a');

  // ensure size is consistent after pushing back on empty list
  affirm(s1.size() == 1);
  affirm(s2.size() == 1);
  s2.push_back('b');

  // confirm size remains consistent after pushing back on non empty list
  affirm(s2.size() == 2);
  s2.push_back('c');

  // ensuring size is consistent after pushing back on list of size greater
  // than 1.
  affirm(s2.size() == 3);

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

bool plusEqualsTest() {
  // Set up the TestingLogger object
  TestingLogger log("+= test");

  TestingString s1;
  TestingString s2;

  // non empty test
  s1.push_back('a');
  s1.push_back('b');
  s1.push_back('c');
  s2.push_back('a');
  s2.push_back('b');
  s2.push_back('c');

  s1 += s2;

  affirm(s1.size() == 6);  // make sure s1 changed
  affirm(s1 != s2);
  affirm(s2.size() == 3);  // make sure s2 didn't change

  s2.push_back('a');
  s2.push_back('b');
  s2.push_back('c');

  affirm(s1.size() == 6);
  affirm(s2.size() == 6);
  affirm(s1 == s2);  // make sure s1 got the correct characters

  // empty test
  TestingString s3;
  TestingString s4;

  s3 += s3;

  affirm(s3.size() == 0);  // make sure s3 stays empty
  affirm(s4 == s3);

  s3.push_back('a');
  s4.push_back('a');
  s4.push_back('a');

  s3 += s3;  // test += on non empty self

  affirm(s3.size() == 2);
  affirm(s3 == s4);

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

bool beginTest() {
  // Set up the TestingLogger object
  TestingLogger log("Begin test");

  TestingString s1;
  affirm(s1.begin() == s1.end());
  s1.push_back('a');
  affirm((*(s1.begin())) == 'a');  // check begin references the correct char
  affirm(s1.begin() != s1.end());
  s1.push_back('b');
  affirm((*(s1.begin())) == 'a');  // check begin references the correct char
  affirm(s1.begin() != s1.end());

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

bool endTest() {
  // Set up the TestingLogger object
  TestingLogger log("End test");

  TestingString s1;
  affirm(s1.begin() == s1.end());
  s1.push_back('a');
  affirm(s1.begin() != s1.end());
  affirm(*(--s1.end()) == 'a');  // check end references the correct char
  s1.push_back('b');
  affirm(s1.begin() != s1.end());
  affirm(*(--s1.end()) == 'b');  // check end references the correct char

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

bool printTest() {
  // Set up the TestingLogger object
  TestingLogger log("Print test");

  // empty test
  TestingString s1;
  std::ostringstream empty;
  empty << s1;
  std::string output = empty.str();
  affirm(output == "");

  // non empty test
  s1.push_back('a');
  s1.push_back('b');
  s1.push_back('c');

  std::ostringstream buffer;
  buffer << s1;
  output = buffer.str();
  affirm(output == "abc");

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

bool lessThanTest() {
  // Set up the TestingLogger object
  TestingLogger log("< test");

  TestingString s1;
  TestingString s2;
  TestingString s3;
  TestingString s4;

  s1.push_back('a');
  s2.push_back('b');
  affirm(s1 < s2);  // compare a with b
  affirm(s3 < s1);  // compare to empty string

  s3.push_back('a');
  s3.push_back('b');
  affirm(s1 < s3);  // compare a with ab (prefix test)

  s1.push_back('a');
  affirm(s1 < s3);  // compare aa with ab

  s4.push_back('a');
  s4.push_back('a');
  s4.push_back('a');
  s4.push_back('a');
  s1.push_back('a');
  s1.push_back('a');
  s1.push_back('a');

  affirm(s4 < s1);  // compare aaaa with aaaaa

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

bool insertTest() {
  // Set up the TestingLogger object
  TestingLogger log("Insert test");

  TestingString begin;
  TestingString middle;
  TestingString end;
  TestingString compare;

  begin.push_back('b');
  middle.push_back('a');
  end.push_back('a');
  compare.push_back('a');
  begin.push_back('c');
  middle.push_back('c');
  end.push_back('b');
  compare.push_back('b');
  begin.push_back('d');
  middle.push_back('d');
  end.push_back('c');
  compare.push_back('c');
  compare.push_back('d');

  begin.insert(begin.begin(), 'a');
  middle.insert(++middle.begin(), 'b');
  end.insert(end.end(), 'd');

  /*
  std::cout << "Compare: "; 
  compare.printChunks(std::cout);
  std::cout << std::endl;
  std::cout << "Begin: "; 
  begin.printChunks(std::cout);
  std::cout << std::endl;
  std::cout << "Middle: "; 
  middle.printChunks(std::cout);
  std::cout << std::endl;
  std::cout << "End: "; 
  end.printChunks(std::cout);
  std::cout << std::endl;
  */

  // tests for inserting at the beginning, middle, and end cases
  affirm(compare == begin);
  affirm(compare == middle);
  affirm(compare == end);

  // tests that inserting at end is the same as push_back and that
  // if the TestingString is empty, inserting at the beginning is the
  // same as inserting at the end.
  TestingString s1;
  TestingString s2;
  TestingString s3;

  s1.insert(s1.begin(), 'a');
  s2.push_back('a');
  s3.insert(s3.end(), 'a');

  affirm(s1 == s2);
  affirm(s2 == s3);

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

bool eraseTest() {
  // Set up the TestingLogger object
  TestingLogger log("Erase test");

  TestingString begin;
  TestingString middle;
  TestingString end;
  TestingString compare;

  begin.push_back('a');
  begin.push_back('a');
  middle.push_back('a');
  end.push_back('a');
  compare.push_back('a');
  begin.push_back('b');
  middle.push_back('b');
  middle.push_back('b');
  end.push_back('b');
  compare.push_back('b');
  begin.push_back('c');
  middle.push_back('c');
  end.push_back('c');
  end.push_back('c');
  compare.push_back('c');

  begin.erase(begin.begin());
  middle.erase(++middle.begin());
  end.erase(--end.end());

  // tests for erasing at the beginning, middle, and end cases
  affirm(compare == begin);
  affirm(compare == middle);
  affirm(compare == end);

  TestingString s1;
  TestingString s2;

  // test erasing to form an empty TestingString
  s1.push_back('a');
  s1.erase(s1.begin());

  affirm(s1 == s2);

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

bool utilizationTest() {
  // Set up the TestingLogger object
  TestingLogger log("Utilization test");

  TestingString s1;

  s1.push_back('a');
  s1.push_back('a');
  s1.push_back('a');
  s1.push_back('a');
  s1.push_back('a');
  s1.push_back('a');

  // testing for utilization after filling a chunk halfway (6/12)
  checkUtilization(s1, 2);

  s1.erase(s1.begin());
  s1.push_back('a');
  s1.erase(s1.begin());
  s1.push_back('a');
  s1.erase(s1.begin());
  s1.erase(s1.begin());
  s1.erase(s1.begin());

  // testing for 1/4 utilization after calling erase and push_back multiple
  // times.
  checkUtilization(s1, 4);

  return log.summarize();
}

bool loopIteratorTest() {
  // Set up the TestingLogger object
  TestingLogger log("Loop: Insert + erase test");

  TestingString s1;
  TestingString::iterator a = s1.begin();

  char c = 'x';

  // check that insert returns a valid iterator everytime
  for (size_t i = 0; i < 24; ++i) {
    std::cout << "Insert target: ";
    std::cout << c << std::endl;
    a = s1.insert(a, c);
    s1.printChunks(std::cout);
    std::cout << "Iterator returned: " << *a << std::endl;
    std::cout << std::endl; 
    // if (*a == 'd') *a = 'a';
    --c;
    
  }

  affirm(s1.size() == 24);
  /*
  std::cout << "Size: " << s1.size() << ":" << std::endl;
  s1.printChunks(std::cout);
  std::cout << endl;
  a = s1.begin();
  // check that erase returns a valid iterator everytime
  for (size_t i = 0; i < 24; ++i) {
    std::cout << "Erase Target:" << *a << std::endl;
    a = s1.erase(a);
    std::cout << "Size: " << s1.size() << ":" << std::endl;
    s1.printChunks(std::cout);
    std::cout << endl;
  }

  affirm(s1.size() == 0);
  */

  return log.summarize();
}

bool customTest() {
  // Set up the TestingLogger object
  
  TestingLogger log("CustomTests");
  
  TestingString s3;

  // tests the funcionality of many push_backs

  s3.push_back('0');
  s3.push_back('1');
  s3.push_back('2');
  s3.push_back('3');
  s3.push_back('4');
  s3.push_back('5');
  s3.push_back('6');
  s3.push_back('7');
  s3.push_back('8');
  s3.push_back('9');
  s3.push_back('a');
  s3.push_back('b');
  s3.push_back('0');
  s3.push_back('1');
  s3.push_back('2');
  s3.push_back('3');
  s3.push_back('4');
  s3.push_back('5');
  s3.push_back('6');
  s3.push_back('7');
  s3.push_back('8');
  s3.push_back('9');
  s3.push_back('a');
  s3.push_back('b');
  s3.push_back('0');
  s3.push_back('1');
  s3.push_back('2');
  s3.push_back('3');
  s3.push_back('4');
  s3.push_back('5');
  s3.push_back('6');
  s3.push_back('7');
  s3.push_back('8');
  s3.push_back('9');
  s3.push_back('a');
  s3.push_back('b');
  TestingString s2 = s3;
  affirm(s2 == s3);

  // tests deep copy ability
  char c = 'c';
  TestingString::iterator k = s3.begin();
  for (TestingString::iterator i = s2.begin(); i != s2.end(); ++i) {
      affirm(*k == *i);
      *k = c;
      affirm(c != *i);
      affirm(*i != *k);
      ++k;
  }
  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

//--------------------------------------------------
//           RUNNING THE TESTS
//--------------------------------------------------

// Called if the test runs too long.
static void timeout_handler(int) {
  // We go super-low-level here, because we can't trust anything in
  // the C/C++ library to really be working right.
  write(STDERR_FILENO, "Timeout occurred!\n", 18);
  abort();
}

/// Run tests
int main(int argc, char** argv) {
  // Initalize testing environment
  TestingLogger alltests{"All tests"};

#if LOAD_GENERIC_STRING
  // Load the desired string implementation, if that's what we're doing.

  if (argc != 2) {
    cerr << "Usage ./stringtest plugin" << endl;
    exit(0);
  }

  GenericString::loadImplementation(argv[1]);
#else
  // No setup to do if we're using ChunkyString directly
#endif

  signal(SIGALRM, timeout_handler);  // Call this when the timer expires
  alarm(10);                         // set the timer at 10 seconds

  // Add calls to your tests here...
  affirm(defaultConstructorTest());
  affirm(equalsTest());
  affirm(notEqualsTest());
  affirm(pushBackTest());
  affirm(sizeTest());
  affirm(plusEqualsTest());
  affirm(beginTest());
  affirm(endTest());
  affirm(printTest());
  affirm(lessThanTest());
  affirm(insertTest());
  affirm(eraseTest());
  affirm(utilizationTest());
  affirm(loopIteratorTest());
  affirm(customTest());

  if (alltests.summarize(true)) {
    return 0;  // Error code of 0 == Success!
  } else {
    return 2;  // Arbitrarily chosen exit code of 2 means tests failed.
  }
}
