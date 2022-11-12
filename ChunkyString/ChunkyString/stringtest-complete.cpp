/**
 * \file stringtest-complete.cpp
 * author: CS70 Sample Solution
 *
 * \brief Tests a ChunkyString for correctness.
 *
 * \remarks This file is essentially the solution file for last week's tests.
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
#include "string-wrapper.hpp"  // Use dynamic loading magic!
using TestingString = GenericString;
#else
#include "chunkystring.hpp"  // Just include and link as normal.
using TestingString = ChunkyString;
#endif

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <initializer_list>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "signal.h"
#include "unistd.h"

using std::advance;
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::vector;

static const size_t TESTING_CHUNKSIZE = 12;  // Assuming a chunksize of 12

/******************************************************************************
 * Helper functions
 *****************************************************************************/
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
 * \param divisor       Fullness of chunk = 1/divisor
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

/**
 * \brief Converts any type that operator << can write out into a string.
 *
 * \param thing         thing to convert
 * \returns             string representation of thing
 * \throws std::invalid_argument  if conversion fails
 */
template <typename T>
string stringFrom(const T& thing) {
  stringstream stream;
  stream << thing;

  if (!stream.fail()) {
    return stream.str();
  } else {
    throw std::invalid_argument("Conversion to string failed");
  }
}

enum randomness_t { MIN_VALUE, MAX_VALUE, MID_VALUE, RANDOM_VALUE };

/**
 * Returns a value n, where 0 < n <= maximum.  Depending on the choice
 * parameter, n will either be at the midpoint, one of the extreme ends
 * or at a random position.
 */
static size_t maybeRandomSize(size_t maximum, randomness_t choice) {
  switch (choice) {
    case MIN_VALUE:
      return 0;

    case MAX_VALUE:
      return maximum;

    case MID_VALUE:
      return (maximum + 1) / 2;

    case RANDOM_VALUE:
    default:
      // We take rather unnecessary care here to avoid skewing the
      // distribution.  Only really necessary if maximum is very
      // large.
      ++maximum;
      size_t divisor = RAND_MAX / maximum;
      size_t candidate;

      do {
        candidate = random() / divisor;
      } while (candidate >= maximum);

      return candidate;
  }
}

/// Returns a random character
static char randomChar() {
  return maybeRandomSize(26, RANDOM_VALUE) + 'a';
}

/// Tests comparison for two string-like objects
template <class LHSType, class RHSType>
void testComparison(const LHSType& string1, const RHSType& string2) {
  string s1 = stringFrom(string1);
  string s2 = stringFrom(string2);

  affirm((s1 == s2) == (s2 == s1));
  affirm((s1 != s2) == (s2 != s1));
  if (s1 != s2) {
    affirm((s1 < s2) == !(s2 < s1));
  } else {
    affirm((s1 < s2) == (s2 < s1));
  }
}

/**
 * \brief Compare a TestingString and an iterator against a string and
 *        its iterator
 *
 * \details Tests size, iterator comparison operators, and all operations
 *          tested by testComparison(test, control).
 *
 * \param testingString         TestingString to check
 * \param controlString         string to use as control value
 * \param testingIterator       an iterator for the TestingString
 * \param controlIterator       an iterator for the control string
 */
void checkWithControl(const TestingString& testingString,
                      const string& controlString,
                      const TestingString::const_iterator& testingIterator,
                      const string::const_iterator controlIterator) {
  // Check that the sizes are the same
  affirm(testingString.size() == controlString.size());

  // Check that the testing string matches the control
  affirm(stringFrom(testingString) == controlString);

  // Check all comparison operations
  testComparison(testingString, controlString);

  // Check that iterators match
  affirm((testingIterator == testingString.begin()) ==
         (controlIterator == controlString.begin()));
  affirm((testingIterator == testingString.end()) ==
         (controlIterator == controlString.end()));
}

/**
 * \brief Compare two TestingStrings to an expected values and each other.
 *
 * \details Tests operator==, operator!=, operator<. and all functions
 *          tested by checkWithControl(test, control).
 *
 * \param first         TestingString to check
 * \param second        TestingString to check
 * \param fControl      Expected value of 'first'
 * \param sControl      Expected value of 'second'
 */
void checkTwoWithControl(const TestingString& first,
                         const TestingString& second, const string& fControl,
                         const string& sControl) {
  // Check validity of each TestingString
  checkWithControl(first, fControl, first.begin(), fControl.begin());
  checkWithControl(second, sControl, second.begin(), sControl.begin());

  // Test equality and inequality
  affirm((fControl == sControl) == (first == second));
  affirm((sControl == fControl) == (second == first));
  affirm((fControl != sControl) == (first != second));
  affirm((sControl != fControl) == (second != first));
  affirm((fControl < sControl) == (first < second));
  affirm((sControl < fControl) == (second < first));
}

/// Compares two TestingStrings, to see if they're identical
void checkBothIdentical(const TestingString& first,
                        const TestingString& second) {
  checkTwoWithControl(first, second, stringFrom(first), stringFrom(second));
}

/******************************************************************************
 * Testing data
 *****************************************************************************/

struct TestPair {
  const TestingString testingString;  /// the object under test
  const string controlString;         /// the intended value of the object
  const size_t size;                  /// the intended length of the string

  TestPair(const TestingString& test, const string& control, size_t s)
      : testingString(test), controlString(control), size(s) {
    // Nothing (else) to do
  }
};

/**
 * \brief A struct that holds a bunch of strings for testing
 *
 * \remarks Uses the ChunkyString default constructor, assignment operator,
 *          push_back, and destructor
 */
struct TestData {
  vector<TestPair> data;

  static TestPair createRandomStringPairs(size_t size) {
    // the new testing and control strings
    TestingString testingString;
    string controlString;

    // add the same random character(s) to the end of each string
    for (size_t i = 0; i < size; ++i) {
      char c = randomChar();
      testingString.push_back(c);
      controlString.push_back(c);
    }

    // add the pair to the test data
    TestPair testPair(testingString, controlString, size);

    return testPair;
  }

  TestData() {
    const size_t LARGE_NUMBER = 10000;

    // empty string
    // (already handled by default constructors)

    // string of size 1
    data.push_back(createRandomStringPairs(1));

    // string of size 2
    data.push_back(createRandomStringPairs(2));

    // string of size CHUNKSIZE
    data.push_back(createRandomStringPairs(TESTING_CHUNKSIZE));

    // string of size CHUNKSIZE + 1
    data.push_back(createRandomStringPairs(TESTING_CHUNKSIZE + 1));

    // string of size 2 * CHUNKSIZE
    data.push_back(createRandomStringPairs(2 * TESTING_CHUNKSIZE));

    // string of size 2 * CHUNKSIZE + 1
    data.push_back(createRandomStringPairs(2 * TESTING_CHUNKSIZE + 1));

    // string of size 3 * CHUNKSIZE
    data.push_back(createRandomStringPairs(3 * TESTING_CHUNKSIZE));

    // string of size 3 * CHUNKSIZE + 1
    data.push_back(createRandomStringPairs(3 * TESTING_CHUNKSIZE + 1));

    // string of random size
    size_t randomSize = maybeRandomSize(LARGE_NUMBER, RANDOM_VALUE);
    data.push_back(createRandomStringPairs(randomSize));

    // large string
    data.push_back(createRandomStringPairs(LARGE_NUMBER));
  }
};

/******************************************************************************
 * Default constructor tests
 *****************************************************************************/

/// test that the default constructor makes an empty string
bool defaultConstructorTest() {
  // Set up the TestingLogger object
  TestingLogger log("default constructor test");

  TestingString s;
  affirm(s.size() == 0);

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

void defaultConstructorTests() { affirm(defaultConstructorTest()); }

/******************************************************************************
 * Copy constructor tests
 *****************************************************************************/

/// test that the default constructor makes an empty string
bool copyConstructorTest(const TestPair& testPair) {
  // Set up the TestingLogger object
  TestingLogger log("copy constructor test for size" +
                    std::to_string(testPair.size));

  TestingString testingCopy = testPair.testingString;
  checkBothIdentical(testPair.testingString, testingCopy);

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

void copyConstructorTests(const TestData& testData) {
  for (TestPair testPair : testData.data) {
    affirm(copyConstructorTest(testPair));
  }
}

/******************************************************************************
 * size tests
 *****************************************************************************/

/// test that string with n random characters has size n
bool testSpecificSize(const TestPair& testPair) {
  size_t size = testPair.size;

  // Set up the TestingLogger object
  TestingLogger log("size " + std::to_string(size) + " test");

  affirm(testPair.testingString.size() == size);

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

void sizeTests(const TestData& testData) {
  for (TestPair testPair : testData.data) {
    affirm(testSpecificSize(testPair));
  }
}

/******************************************************************************
 * utilization tests
 *****************************************************************************/

/// test that the utilization for a testing string is good
bool testUtilizationSpecificSize(const TestPair& testPair) {
  // Set up the TestingLogger object
  TestingLogger log("utilization test for size " +
                       std::to_string(testPair.size));

  checkUtilization(testPair.testingString, 2);

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

void utilizationTests(const TestData& testData) {
  for (TestPair testPair : testData.data) {
    affirm(testUtilizationSpecificSize(testPair));
  }
}

/******************************************************************************
 * default iterator constructor tests
 *****************************************************************************/

/// test default constructor for iterator and const_iterator
template <bool is_const>
bool defaultIteratorTest() {
  string iteratorKind = is_const ? "const_iterator" : "iterator";

  // Set up the TestingLogger object
  TestingLogger log("default " + iteratorKind);

  using iter_type =
      typename std::conditional<is_const, TestingString::const_iterator,
                                TestingString::iterator>::type;

  iter_type i1;
  iter_type i2;
  affirm(i1 == i2);
  affirm(!(i1 != i2));

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

/// test the iterators for the empty string
void iteratorDefaultConstructorTests() {
  affirm(defaultIteratorTest<false>());
  affirm(defaultIteratorTest<true>());
}

/******************************************************************************
 * begin, end, ++, --, and dereference tests
 *****************************************************************************/

/**
 * \brief Checks that the ith character of s is c
 *
 * \warning If i is greater than the length of the string, this function
 *          has undefined behavior.
 */
void checkCharacterMatch(const TestingString& s, size_t i, char c) {
  // Test using ++
  TestingString::const_iterator iter = s.begin();
  std::advance(iter, i);
  affirm(*iter == c);

  // Test using --
  iter = s.end();
  size_t offset = s.size() - i;
  std::advance(iter, -offset);
  affirm(*iter == c);
}

/// test that string with n random characters has size n
bool testIteratorSpecificSize(const TestPair& testPair) {
  size_t size = testPair.size;
  const TestingString& testingString = testPair.testingString;
  const string& controlString = testPair.controlString;

  // Set up the TestingLogger object
  TestingLogger log("iterator test for size " + std::to_string(size));

  affirm(testingString.size() == controlString.size());

  // If the string is empty, we can only test that begin and end match
  if (size == 0) {
    affirm(testingString.begin() == testingString.end());
  } else {
    // Otherwise, we can test lots of things.

    affirm(testingString.begin() != testingString.end());

    // make sure the first character is correct
    checkCharacterMatch(testingString, 0, controlString[0]);

    // make sure the last character is correct
    checkCharacterMatch(testingString, size - 1, controlString[size - 1]);

    // make sure the middle character is correct
    size_t middle = size / 2;
    checkCharacterMatch(testingString, middle, controlString[middle]);

    // make sure a random character is correct
    size_t randomIndex = maybeRandomSize(size - 1, RANDOM_VALUE);
    checkCharacterMatch(testingString, randomIndex, controlString[randomIndex]);
  }

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

void iteratorTests(const TestData& testData) {
  for (TestPair testPair : testData.data) {
    affirm(testIteratorSpecificSize(testPair));
  }
}

/******************************************************************************
 * print tests
 *****************************************************************************/

/// test that string with n random characters prints correctly
bool testPrintSpecificSize(const TestPair& testPair) {
  size_t size = testPair.size;
  const TestingString& testingString = testPair.testingString;
  const string& controlString = testPair.controlString;

  // Set up the TestingLogger object
  TestingLogger log("print test for size " + std::to_string(size));

  affirm(stringFrom(testingString) == controlString);

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

void printTests(const TestData& testData) {
  for (TestPair testPair : testData.data) {
    affirm(testPrintSpecificSize(testPair));
  }
}

/******************************************************************************
 * append tests
 *****************************************************************************/

/**
 * \brief Test that appending two strings gives the desired result
 *
 * \remarks Uses the TestingString copy constructor
 */
bool testAppendSpecificSize(const TestPair& testPair1,
                            const TestPair& testPair2) {
  // Set up the TestingLogger object
  TestingLogger log("appending size " + std::to_string(testPair1.size) +
                    " to " + std::to_string(testPair2.size));

  string controlResult = testPair1.controlString + testPair2.controlString;
  TestingString lhsCopy = testPair1.testingString;
  TestingString rhsCopy = testPair2.testingString;
  lhsCopy += rhsCopy;

  affirm(stringFrom(lhsCopy) == controlResult);
  checkUtilization(lhsCopy, 2);

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

/**
 * \brief Tests appending a string to itself
 *
 * \remarks Uses the TestingString default constructor, push_back, and ==
 */
bool testSelfAppend() {
  // Set up the TestingLogger object
  TestingLogger log("self-append");

  size_t randomSize = maybeRandomSize(1000, RANDOM_VALUE);
  TestPair testPair = TestData::createRandomStringPairs(randomSize);
  TestingString testingString = testPair.testingString;
  string controlString = testPair.controlString;
  testingString += testingString;
  controlString += controlString;

  affirm(stringFrom(testingString) == controlString);
  checkUtilization(testingString, 2);

  TestingString emptyString;
  testingString += emptyString;

  affirm(stringFrom(testingString) == controlString);
  checkUtilization(testingString, 2);

  emptyString += testingString;

  affirm(stringFrom(emptyString) == controlString);
  checkUtilization(emptyString, 2);

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

/// Create a low-utilization string by repeated appending
bool appendUtilizationTest() {
  // Set up the TestingLogger object
  TestingLogger log("append utilization test");

  TestingString test;
  string control;

  for (size_t i = 0; i < 550; ++i) {
    TestingString append;
    char c = randomChar();
    append.push_back(c);
    append.push_back(c);  // Two char chunk, in case 1 char is a special case
    test += append;
    control.push_back(c);
    control.push_back(c);

    checkUtilization(test, 2);
    checkWithControl(test, control, test.begin(), control.begin());
  }

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

void appendTests(const TestData& testData) {
  for (TestPair firstPair : testData.data) {
    for (TestPair secondPair : testData.data) {
      affirm(testAppendSpecificSize(firstPair, secondPair));
    }
  }

  affirm(testSelfAppend());
  affirm(appendUtilizationTest());
}

/******************************************************************************
 * (in)equality tests
 *****************************************************************************/

bool testEquality(const TestingString& string1, const TestingString& string2) {
  // Set up the TestingLogger object
  TestingLogger log("inequality test");

  affirm(string1 == string2);
  affirm(string2 == string1);

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

bool testInequality(const TestingString& string1,
                    const TestingString& string2) {
  // Set up the TestingLogger object
  TestingLogger log("inequality test");

  affirm(string1 != string2);
  affirm(string2 != string1);

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

/// tests inequality for testing strings
void testInequalityDifferentSize(const TestData& testData) {
  size_t newSize = 0;
  for (TestPair testPair : testData.data) {
    newSize += testPair.size;
  }

  TestPair newPair = TestData::createRandomStringPairs(newSize);
  TestingString newTestingString = newPair.testingString;
  for (TestPair testPair : testData.data) {
    const TestingString& testingString = testPair.testingString;
    affirm(testInequality(testingString, newTestingString));
  }
}

/// tests inequality for testing strings
void testInequalitySameSize(const TestData& testData) {
  for (TestPair testPair : testData.data) {
    const TestingString& testingString = testPair.testingString;
    TestPair newPair = TestData::createRandomStringPairs(testPair.size);
    const TestingString& newTestingString = newPair.testingString;
    affirm(testInequality(testingString, newTestingString));
  }
}

void equalityTests(const TestData& testData) {
  for (TestPair testPair : testData.data) {
    const TestingString& testingString = testPair.testingString;
    affirm(testEquality(testingString, testingString));
  }
}

void inequalityTests(const TestData& testData) {
  testInequalityDifferentSize(testData);
  testInequalitySameSize(testData);
}

/// Test equality and inequality
bool equalityTestWithInsert(const TestingString& testingString1,
                            const TestingString& testingString2) {
  // Set up the TestingLogger object
  TestingLogger log("equality test with insert");

  checkTwoWithControl(testingString1, testingString2,
                      stringFrom(testingString1), stringFrom(testingString2));

  TestingString pushBack;
  TestingString insert;

  // Same string constructed different ways
  for (unsigned i = 0; i < 128; ++i) {
    pushBack.push_back(i);
  }

  for (int i = 127; i >= 0; i -= 2) {
    insert.insert(insert.begin(), i);
  }

  TestingString::iterator it = insert.end();

  for (int i = 126; i >= 0; i -= 2) {
    --it;
    it = insert.insert(it, i);
  }

  checkBothIdentical(pushBack, insert);

  // Almost the same, but one is longer
  pushBack.push_back('a');
  checkTwoWithControl(pushBack, insert, stringFrom(pushBack),
                      stringFrom(insert));

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

void equalityTestsWithInsert(const TestData& testData) {
  for (TestPair testPair : testData.data) {
    const TestingString& testingString = testPair.testingString;
    affirm(equalityTestWithInsert(testingString, testingString));
  }
}

/******************************************************************************
 * comparison tests
 *****************************************************************************/

/// compare two strings whose order is unknown
bool testLessThanUnknown(const TestingString& string1,
                         const TestingString& string2) {
  // Set up the TestingLogger object
  TestingLogger log("less-than unknown");

  if (string1 == string2) {
    affirm((string1 < string2) == (string2 < string1));
  } else {
    affirm((string1 < string2) == !(string2 < string1));
  }

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

/// test that string1 is less than string 2
bool testDefinitelyLessThan(const TestingString& string1,
                            const TestingString& string2) {
  // Set up the TestingLogger object
  TestingLogger log("definitely less-than");

  affirm(string1 < string2);

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

/// tests less than (and == and !=) for testing strings
void lessThanTests(const TestData& testData) {
  const TestingString empty;

  for (TestPair testPair1 : testData.data) {
    const TestingString& testingString1 = testPair1.testingString;

    // test that the empty string is less than every non-empty string
    if (testingString1.size() != 0) {
      affirm(testDefinitelyLessThan(empty, testingString1));
    }

    // test that appending a character to a string makes
    // the original string less-than
    TestingString copy = testingString1;
    copy.push_back('z');
    affirm(testDefinitelyLessThan(testingString1, copy));

    // test that pushing back NUL doesn't affect less-than
    /*copy = testingString1;
    copy.push_back(0);  // NUL
    affirm(testDefinitelyLessThan(testingString1, copy));

    // test that pushing back DEL doesn't affect less-than
    copy = testingString1;
    copy.push_back(127);  // DEL
    affirm(testDefinitelyLessThan(testingString1, copy));*/

    // compare strings to one another
    for (TestPair testPair2 : testData.data) {
      const TestingString& testingString2 = testPair2.testingString;
      affirm(testLessThanUnknown(testingString1, testingString2));
    }
  }
}

/******************************************************************************
 * insert and erase tests
 *****************************************************************************/

enum modification_type_t { INSERT, ERASE, ERASE_ALL };
enum iterator_location_t { FRONT, MIDDLE, BACK, CHUNK, RANDOM };

string modificationDescription(modification_type_t modificationType) {
  switch (modificationType) {
    case INSERT:
      return "insert";
    case ERASE:
      return "erase";
    case ERASE_ALL:
      return "erase all";
  }
}

string iteratorDescription(iterator_location_t iteratorLocation) {
  switch (iteratorLocation) {
    case FRONT:
      return "front";
    case MIDDLE:
      return "middle";
    case BACK:
      return "back";
    case CHUNK:
      return "chunk boundary";
    case RANDOM:
      return "random location";
  }
}

/// compute a place to either insert or erase
size_t getIteratorLocation(size_t size, iterator_location_t iteratorLocation,
                           modification_type_t modificationType) {
  if (size == 0) {
    return 0;
  }

  // compute index for modification location
  size_t result = 0;
  switch (iteratorLocation) {
    case FRONT:
      result = 0;
      break;
    case MIDDLE:
      result = size / 2;
      break;
    case BACK:
      result = size;
      break;
    case CHUNK:
      if (size <= TESTING_CHUNKSIZE) {
        result = size;
      } else {
        result = TESTING_CHUNKSIZE;
      }
      break;
    case RANDOM:
      result = maybeRandomSize(size, RANDOM_VALUE);
      break;
  }

  // can't erase from the end
  if ((modificationType != INSERT) && (result == size)) {
    --result;
  }

  return result;
}

/// test insert or erase, from a particular location in the string
bool modifyIteratorTest(const TestPair& testPair,
                        iterator_location_t iteratorLocation,
                        modification_type_t modificationType) {
  // Set up the TestingLogger object
  string description = modificationDescription(modificationType) + " " +
                       iteratorDescription(iteratorLocation) + ", size " +
                       std::to_string(testPair.size);
  TestingLogger log(description);

  // make copies of the strings, to modify
  TestingString testingString = testPair.testingString;
  string controlString = testPair.controlString;

  // save sizes
  size_t initialSize = testingString.size();

  // get iterators for modification location
  size_t offset =
      getIteratorLocation(initialSize, iteratorLocation, modificationType);
  TestingString::iterator testingIterator = testingString.begin();
  string::iterator controlIterator = controlString.begin();
  advance(testingIterator, offset);
  advance(controlIterator, offset);

  // Modify both strings
  size_t newSize = initialSize;
  char newChar = *testingIterator;

  // Inserting a single character at the provided location
  if (modificationType == INSERT) {
    newChar = randomChar();
    testingIterator = testingString.insert(testingIterator, newChar);
    controlIterator = controlString.insert(controlIterator, newChar);
    ++newSize;
  } else if (modificationType == ERASE) {
    // Erasing a single character from the provided location
    testingIterator = testingString.erase(testingIterator);
    controlIterator = controlString.erase(controlIterator);
    --newSize;
    if (controlIterator != controlString.end()) {
      newChar = *controlIterator;
    }
  } else if (modificationType == ERASE_ALL) {
    // Erasing all the characters

    // As long as there are characters in the string,
    // erase a character from the provided location.
    while (testingString.begin() != testingString.end()) {
      testingIterator = testingString.begin();
      controlIterator = controlString.begin();
      size_t location = getIteratorLocation(testingString.size(),
                                            iteratorLocation, modificationType);
      advance(testingIterator, location);
      advance(controlIterator, location);
      testingIterator = testingString.erase(testingIterator);
      controlIterator = controlString.erase(controlIterator);
    }
    newSize = 0;
  }

  // Check that the new testing string has the correct size
  affirm(newSize == testingString.size());

  // Check that the iterator "points" to the correct character
  if ((testingString.size()) > 0 && (testingIterator != testingString.end())) {
    affirm(*testingIterator == newChar);
  }

  // Check utilization
  size_t utilizationLimit = (modificationType == ERASE) ? 4 : 2;
  checkUtilization(testingString, utilizationLimit);

  checkWithControl(testingString, controlString, testingIterator,
                   controlIterator);

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

/// test insert or erase from various locations in the string
void modificationTests(const TestData& testData,
                       modification_type_t modificationType) {
  for (TestPair testPair : testData.data) {
    affirm(modifyIteratorTest(testPair, FRONT, modificationType));
    affirm(modifyIteratorTest(testPair, MIDDLE, modificationType));
    affirm(modifyIteratorTest(testPair, BACK, modificationType));
    if (testPair.size >= TESTING_CHUNKSIZE) {
      affirm(modifyIteratorTest(testPair, CHUNK, modificationType));
    }
    affirm(modifyIteratorTest(testPair, RANDOM, modificationType));
  }
}

/// erases `amount` characters from the beginning
void eraseFromBeginning(TestingString& testingString, string& controlString,
                        size_t amount) {
  for (size_t i = 0; i < amount; ++i) {
    testingString.erase(testingString.begin());
    controlString.erase(controlString.begin());
  }

  checkWithControl(testingString, controlString, testingString.end(),
                   controlString.end());
  checkUtilization(testingString, 4);
}

/// erases `amount` characters from the end
void eraseFromEnd(TestingString& testingString, string& controlString,
                  size_t amount) {
  for (size_t i = 0; i < amount; ++i) {
    testingString.erase(--(testingString.end()));
    controlString.erase(--(controlString.end()));
  }
  checkWithControl(testingString, controlString, testingString.end(),
                   controlString.end());
  checkUtilization(testingString, 4);
}

/// erase from beginning and end of string
bool alternatingErasesLongString() {
  // Set up the TestingLogger object
  TestingLogger log("erase from different places in long string");

  // Create some test data
  const size_t STRING_SIZE = TESTING_CHUNKSIZE * 3;
  TestPair testPair = TestData::createRandomStringPairs(STRING_SIZE);
  TestingString testingString = testPair.testingString;
  string controlString = testPair.controlString;

  const size_t BIG_MOD_SIZE = TESTING_CHUNKSIZE / 2;
  const size_t SMALL_MOD_SIZE = TESTING_CHUNKSIZE / 3;

  // erase from the end of the string
  eraseFromEnd(testingString, controlString, BIG_MOD_SIZE);

  // erase from the beginning of the string
  eraseFromBeginning(testingString, controlString, BIG_MOD_SIZE);

  // erase from the end of the string
  eraseFromEnd(testingString, controlString, SMALL_MOD_SIZE);

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

/**
 * Test erase.  Erases characters into a string within a locus of 'n' from
 * the front or back (depending on whether where is FRONT or BACK), exactly
 * how the locus is defined depends on randomize and the number of characters
 * currently in the string.
 *
 * \remarks Copied from old test cases
 */
void randomEraseTest(size_t n, iterator_location_t where,
                     randomness_t randomize) {
  // Test data
  static const string TEST_DATA(
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz"
      "123456789");
  static const size_t TEST_DATA_SIZE = TEST_DATA.size();

  string control(TEST_DATA);

  TestingString test;

  for (string::const_iterator i = TEST_DATA.begin(); i != TEST_DATA.end();
       ++i) {
    test.push_back(*i);
  }

  for (size_t i = TEST_DATA_SIZE; i > 0; --i) {
    // The string is not yet empty
    affirm(!(test == TestingString()));

    int m;

    if (where == FRONT) {
      // Note: you'll get a compiler warning here:
      //     maybeRandomSize returns an unsigned value (size_t)
      m = n >= i ? i - 1 : maybeRandomSize(n, randomize);
    } else {
      m = n >= i ? 0 : i - 1 - maybeRandomSize(n, randomize);
    }

    TestingString::iterator p = test.begin();
    advance(p, m);

    TestingString::iterator tIter = test.erase(p);
    string::iterator cIter = control.erase(control.begin() + m);

    checkWithControl(test, control, tIter, cIter);
    if (i > 0) {
      checkUtilization(test, 4);
    }
  }

  // The string is now empty
  affirm(test == TestingString());
}

/// Try to break utilization by pure erasing
bool eraseUtilizationTest() {
  // Set up the TestingLogger object
  TestingLogger log("erase utilization test");

  for (randomness_t r : {MIN_VALUE, MAX_VALUE, MID_VALUE, RANDOM_VALUE}) {
    for (int i = 0; i < 25; ++i) {
      randomEraseTest(i, FRONT, r);
    }

    for (int i = 0; i < 25; ++i) {
      randomEraseTest(i, BACK, r);
    }
  }

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

void insertionTests(const TestData& testData) {
  modificationTests(testData, INSERT);
}

void eraseTests(const TestData& testData) {
  modificationTests(testData, ERASE);
  modificationTests(testData, ERASE_ALL);
  affirm(alternatingErasesLongString());
  affirm(eraseUtilizationTest());
}

/// check that pushing "weird" chars (NUL and DEL) work just like normal chars
bool checkWeirdCharacters(const TestPair& testPair) {
  // Set up the TestingLogger object
  TestingLogger log("check weird characters");

  const TestingString& testingString = testPair.testingString;
  const string& controlString = testPair.controlString;

  // Test edge case characters
  TestingString chunkyChr0;
  string checkChr0;
  chunkyChr0.push_back(0);  // NUL character
  checkChr0.push_back(0);

  affirm((controlString == checkChr0) == (testingString == chunkyChr0));
  affirm((checkChr0 == controlString) == (chunkyChr0 == testingString));
  affirm((controlString < checkChr0) == (testingString < chunkyChr0));
  affirm((checkChr0 < controlString) == (chunkyChr0 < testingString));

  TestingString chunkyChr127;
  string checkChr127;
  chunkyChr127.push_back(127);  // DEL character
  checkChr127.push_back(127);

  affirm((controlString == checkChr127) == (testingString == chunkyChr127));
  affirm((checkChr127 == controlString) == (chunkyChr127 == testingString));
  affirm((controlString < checkChr127) == (testingString < chunkyChr127));
  affirm((checkChr127 < controlString) == (chunkyChr127 < testingString));

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

void weirdCharacterTests(const TestData& testData) {
  for (TestPair testPair : testData.data) {
    affirm(checkWeirdCharacters(testPair));
  }
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
  TestingLogger alltests("All tests");

#if LOAD_GENERIC_STRING
  // Load the desired string implementation, if that's what we're doing.

  if (argc != 2) {
    cerr << "Usage ./stringtest plugin" << endl;
    exit(0);
  }

  GenericString::loadImplementation(argv[1]);
#else
  // No setup to do if we're using ChunkyString directly
  (void)argc;  // turn off
  (void)argv;  //   unused variable warnings
#endif

  signal(SIGALRM, timeout_handler);  // Call this when the timer expires
  alarm(60);                         // set the timer at 10 seconds

  // Add calls to your tests here...
  TestData testData;

  defaultConstructorTests();
  copyConstructorTests(testData);
  sizeTests(testData);
  utilizationTests(testData);
  iteratorDefaultConstructorTests();
  iteratorTests(testData);
  printTests(testData);
  equalityTests(testData);
  inequalityTests(testData);
  appendTests(testData);
  lessThanTests(testData);
  weirdCharacterTests(testData);
  insertionTests(testData);
  eraseTests(testData);
  equalityTestsWithInsert(testData);

  if (alltests.summarize(true)) {
    return 0;  // Error code of 0 == Success!
  } else {
    return 2;  // Arbitrarily chosen exit code of 2 means tests failed.
  }
}
