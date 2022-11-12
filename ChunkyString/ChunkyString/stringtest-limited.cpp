/**
 * \file stringtest-limited.cpp
 * author: CS70 Sample Solution
 *
 * \brief Tests a ChunkyString for correctness, omitting
 * tests for erase and insert operations.
 */

#include "testing-logger.hpp"
#ifndef LOAD_GENERIC_STRING
/* This test code can be used two ways.  It can either test a local
 * ChunkyString implementation, or it can dynamically load an implementation
 * at runtime and test that.  In the first homework, you'll load an
 * implementation that someone else used, but in later assignments, you'll
 * set the value to zero to use your own.
 */
#define LOAD_GENERIC_STRING 0  // 0 = Normal, 1 = Load Code Dynamically
#endif

#if LOAD_GENERIC_STRING
//#include "string-wrapper.hpp"  Use dynamic loading magic! You'r not sneky
using TestingString = GenericString;
#else
#include "chunkystring.hpp"  // Just include and link as normal.
using TestingString = ChunkyString;
#endif

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <string>

#include "signal.h"
#include "unistd.h"

using std::advance;
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::stringstream;

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

enum randomness_t { MIN_VALUE, MAX_VALUE, MID_VALUE, RANDOM_VALUE };

/**
 * Returns a value n, where 0 < n < maximum.  Depending on the choice
 * parameter, n will either be at the midpoint, one of the extreme ends
 * or at a random position.
 */
static int maybeRandomInt(int maximum, randomness_t choice) {
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
      int candidate;

      do {
        candidate = random() / divisor;
      } while (candidate >= maximum);

      return candidate;
  }
}

/// Returns a random printable character, using the maybeRandomInt function.
static char randomChar() { return 33 + maybeRandomInt(95, RANDOM_VALUE); }

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

/**
 * \brief Make sure that equivalent TestingStrings are not actually using
 *        the same data (i.e. not a shallow copy)
 *
 * \param test          TestingString that will be modified in this test
 * \param copy          TestingString that will remain constant in this test
 *
 * \remark  The first input will be modified during the test, but is returned
 *          to its original value before the test returns.
 */
void checkDeepCopy(TestingString& test, const TestingString& copy) {
  // Don't deal with empty strings
  if (test.size() == 0) {
    return;
  }

  // Select random character to change
  size_t pos = maybeRandomInt(test.size() - 1, RANDOM_VALUE);
  TestingString::iterator tIter = test.begin();
  std::advance(tIter, pos);
  TestingString::const_iterator cIter = copy.begin();
  std::advance(cIter, pos);

  // Select a random character that is not the same as the existing one
  char c;

  do {
    c = randomChar();
  } while (c == *tIter);

  // Modify test string and see that copy is unchanged
  affirm(test == copy);
  affirm(*cIter == *tIter);
  *tIter = c;
  affirm(c != *cIter);
  affirm(*cIter != *tIter);
  affirm(test != copy);

  // Return them to equality
  *tIter = *cIter;
  affirm(test == copy);
}

/**
 * \brief Compare the TestingString to an expected value.
 *
 * \details Tests size, operator<<, begin, end, forward iteration, and
 *          edge case comparisons.
 *
 * \param test          TestingString to check
 * \param control       Expected value of the TestingString
 */
void checkWithControl(const TestingString& test, const string& control) {
  // Make sure controls and tests match
  affirm(control.size() == test.size());
  affirm(control.size() == stringFrom(control).size());
  affirm(stringFrom(control) == stringFrom(test));
  affirm(control == stringFrom(test));

  TestingString::const_iterator tIter = test.begin();
  string::const_iterator cIter = control.begin();

  // Walk forward
  for (; cIter != control.end(); ++cIter, ++tIter) {
    affirm(*cIter == *tIter);
  }

  affirm(tIter == test.end());

  // Walk backward
  for (; cIter != control.begin();) {
    --cIter;
    --tIter;
    affirm(*cIter == *tIter);
  }

  affirm(tIter == test.begin());

  // Test edge case characters
  TestingString chunkyChr0;
  string checkChr0;
  chunkyChr0.push_back(0);  // Null character
  checkChr0.push_back(0);

  affirm((control == checkChr0) == (test == chunkyChr0));
  affirm((checkChr0 == control) == (chunkyChr0 == test));
  affirm((control < checkChr0) == (test < chunkyChr0));
  affirm((checkChr0 < control) == (chunkyChr0 < test));

  TestingString chunkyChr127;
  string checkChr127;
  chunkyChr127.push_back(127);  // Delete character
  checkChr127.push_back(127);

  affirm((control == checkChr127) == (test == chunkyChr127));
  affirm((checkChr127 == control) == (chunkyChr127 == test));
  affirm((control < checkChr127) == (test < chunkyChr127));
  affirm((checkChr127 < control) == (chunkyChr127 < test));
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
  checkWithControl(first, fControl);
  checkWithControl(second, sControl);

  // Test equality and inequality
  affirm((fControl == sControl) == (first == second));
  affirm((sControl == fControl) == (second == first));
  affirm((fControl != sControl) == (first != second));
  affirm((sControl != fControl) == (second != first));
  affirm((fControl < sControl) == (first < second));
  affirm((sControl < fControl) == (second < first));
}

/**
 *  Compares two TestingStrings
 */
void checkBothIdentical(const TestingString& first,
                        const TestingString& second) {
  checkTwoWithControl(first, second, stringFrom(first), stringFrom(second));
}

/**
 * \brief Compare a TestingString and a TestingString iterator with expected
 *        values.
 *
 * \details Tests iterator correctness and all functions tested by
 *          checkWithControl(test, control).
 *
 * \param test          TestingString to check
 * \param control       Expected value of test
 * \param tIter         TestingString iterator to check
 * \param cIter         Expected value of tIter
 */
void checkIterWithControl(const TestingString& test, const string& control,
                          const TestingString::const_iterator& tIter,
                          const string::const_iterator& cIter) {
  affirm((cIter == control.begin()) == (tIter == test.begin()));
  affirm((cIter == control.end()) == (tIter == test.end()));

  if (tIter != test.end()) {
    affirm(*cIter == *tIter);
  }

  // Check this AFTER iterator checks, so iterators are not invalidated
  checkWithControl(test, control);
}

//--------------------------------------------------
//           TEST FUNCTIONS
//--------------------------------------------------

/// Check all known conditions of a single default constructed object
bool defaultConstructorTest() {
  // Set up the TestingLogger object
  TestingLogger log("default constructor test");

  TestingString test;
  string control;

  checkWithControl(test, control);
  affirm(test.begin() == test.end());

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

/// Test the copy constructor
bool copyConstructorTest() {
  // Set up the TestingLogger object
  TestingLogger log("copy constructor test");

  // Copy an empty TestingString
  TestingString original;
  TestingString empty(original);
  checkBothIdentical(original, empty);

  // Copy a small TestingString
  original.push_back('e');
  TestingString tiny(original);
  checkBothIdentical(original, tiny);
  checkDeepCopy(original, tiny);

  // Make sure previous copy is not changed
  TestingString secondEmpty;
  checkBothIdentical(empty, secondEmpty);
  affirm(original != empty);

  // Copy a larger string
  for (char alpha = 'A'; alpha < 'z'; ++alpha) {
    original.push_back(alpha);
  }

  TestingString large(original);
  checkBothIdentical(original, large);
  checkDeepCopy(original, large);

  // Still different from previous copies
  affirm(original != empty);
  affirm(original != tiny);

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

/**
 * \brief Assign one TestingString to another, then verify the assignment
 *
 * \remarks This relies on a correct copy constructor, since the inputs
 *          are copied for repeated testing.
 * \param lhs          The left side of assignment
 * \param rhs          The right side of assignment
 */
/*void assignTest(const TestingString& lhs, const TestingString& rhs)
{
    // Basic equality
    TestingString lhsBasic(lhs);
    lhsBasic = rhs;
    checkBothIdentical(lhsBasic, rhs);

    // Check deep copy by iterators
    checkDeepCopy(lhsBasic, rhs);

    // Deep copy by push_back
    TestingString lhsPush(lhs);
    lhsPush = rhs;
    lhsPush.push_back('r');
    affirm(lhsPush != rhs);
    affirm(rhs < lhsPush);

    // Deep copy by insert
    TestingString lhsInsert(lhs);
    lhsInsert = rhs;
    lhsInsert.insert(lhsInsert.begin(), 'c');
    affirm(lhsPush != rhs);
}*/

const size_t NUM_HELPERS = 8;  ///< Size of helpers array

/// Setup for functions requiring two arguments.
void setUpTwoArguments(TestingString* helpers) {
  // 0 = empty
  // 1 = one char
  // 2 = different one char
  // 3 = one full chunk
  // 4 = different full chunk
  // 5 = several chunks
  // 6 = different several chunks
  // 7 = very long string
  for (size_t i = 0; i < TESTING_CHUNKSIZE * 10 + 5; ++i) {
    // Decide which strings still need filling
    size_t j = 1;

    if (i >= 3 * TESTING_CHUNKSIZE + 1) {
      j = 7;
    } else if (i >= TESTING_CHUNKSIZE) {
      j = 5;
    } else if (i >= 1) {
      j = 3;
    }

    for (; j < NUM_HELPERS; ++j) {
      helpers[j].push_back(randomChar());
    }
  }

  // Make sure single char strings are different
  if (*helpers[1].begin() == *helpers[2].begin()) {
    *helpers[1].begin() = *helpers[1].begin() + 1;
  }
}

/// Test Assignment in as many combinations as possible
/*bool assignmentTest()
{
    // Set up the TestingLogger object
    TestingLogger log("assignment test");

    TestingString helpers[NUM_HELPERS];  ///< Array of useful TestingStrings
    setUpTwoArguments(helpers);

    for (size_t i = 0; i < NUM_HELPERS; ++i) {
        // Assign makes copies, so lets try self-assignment directly
        size_t preSize = helpers[i].size();
        string value = stringFrom(helpers[i]);
        helpers[i] = helpers[i];
        affirm(preSize == helpers[i].size());
        affirm(value == stringFrom(helpers[i]));

        // Assign each helper_ to each other helper_, to cover all cases
        // of small and large TestingStrings being assigned to each other.
        for (size_t j = 0; j < NUM_HELPERS; ++j) {
            assignTest(helpers[i], helpers[j]);
        }
    }

    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();
}*/

/// Test += in as many combinations as possible
bool appendTest() {
  // Set up the TestingLogger object
  TestingLogger log("append test");

  TestingString helpers[NUM_HELPERS];  ///< Array of useful TestingStrings
  setUpTwoArguments(helpers);

  for (size_t i = 0; i < NUM_HELPERS; ++i) {
    TestingString temp = helpers[i];
    string control = stringFrom(temp);

    // Check for self-append
    temp += temp;
    control += control;
    checkWithControl(temp, control);

    // Check all pairs
    for (size_t j = 0; j < NUM_HELPERS; ++j) {
      temp = helpers[i];
      control = stringFrom(temp);

      temp += helpers[j];
      control += stringFrom(helpers[j]);

      checkWithControl(temp, control);
    }
  }

  // Make sure they didn't directly link to the other object
  TestingString* heapString = new TestingString();
  heapString->push_back('a');
  heapString->push_back('b');
  // Use a full chunk for our lhs to encourage bad chunk combining behavior
  TestingString stackString;

  for (size_t i = 0; i < TESTING_CHUNKSIZE; ++i) {
    stackString.push_back('z');
  }

  string control = stringFrom(stackString);
  stackString += *heapString;
  control += stringFrom(*heapString);

  // Ensure deep copy
  TestingString::iterator it = heapString->begin();
  *it = 'e';
  checkWithControl(stackString, control);

  // Check for memory isses
  delete heapString;
  checkWithControl(stackString, control);

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

/// Test equality and inequality
/*bool equalityTest()
{
    // Set up the TestingLogger object
    TestingLogger log("equality test");

    TestingString helpers[NUM_HELPERS];  ///< Array of useful TestingStrings
    setUpTwoArguments(helpers);

    // The variety of arguments in helpers assure we have all special
    // cases covered:
    // - empty, single chunk, multi chunk compared in all combos
    // - self equality
    // - same size inequality
    // - different size inequality
    for (size_t i = 0; i < NUM_HELPERS; ++i) {
        for (size_t j = 0; j < NUM_HELPERS; ++j) {
            string sI = stringFrom(i);
            string sJ = stringFrom(j);
            checkTwoWithControl(helpers[i], helpers[j],
            stringFrom(helpers[i]), stringFrom(helpers[j]));
        }
    }

    TestingString pushBack;
    TestingString insert;

    // Same string constructed different ways
    for (size_t i = 0; i < 128; ++i) {
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
}*/

/// Test push_back on a variety of characters
bool pushBackTest() {
  // Set up the TestingLogger object
  TestingLogger log("pushback test");

  TestingString test;
  string control;

  // Add random characters
  for (size_t i = 0; i < TESTING_CHUNKSIZE * 5; ++i) {
    checkWithControl(test, control);
    char next = randomChar();
    test.push_back(next);
    control.push_back(next);
  }

  // Use weird characters
  TestingString test2;
  string control2;

  for (int i = 20; i >= 0; --i) {
    test2.push_back(i);
    control2.push_back(i);
    checkWithControl(test2, control2);
  }

//   for (int i = 245; i < 256; ++i) {
//     test2.push_back(i);
//     control2.push_back(i);
//     checkWithControl(test2, control2);
//   }

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

/// Basic iteration tests
bool iterateTest() {
  // Set up the TestingLogger object
  TestingLogger log("iterate test");

  TestingString test;
  TestingString::iterator iter;  // Test default constructor

  // Empty string iterators
  iter = test.begin();
  affirm(iter == test.end());

  // Add a few items
  test.push_back('a');
  iter = test.begin();
  affirm(iter != test.end());
  affirm('a' == *iter);

  // Forward iteration
  test.push_back('b');
  iter = test.begin();
  ++iter;
  affirm('b' == *iter);
  ++iter;
  affirm(iter == test.end());

  // Reverse iteration
  --iter;
  affirm('b' == *iter);
  --iter;
  affirm('a' == *iter);
  affirm(iter == test.begin());

  // Already used assignment, but lets check it just to be sure
  iter = test.end();
  affirm(iter == test.end());
  TestingString::iterator iter2 = iter;
  affirm(iter2 == iter);
  affirm(iter == iter2);

  // Point both iterators at different characters
  iter2 = test.begin();
  --iter;
  affirm(iter2 != iter);
  affirm(iter != iter2);

  // Assignment!
  iter2 = iter;
  affirm(*iter == *iter2);

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

/**
 * \brief Create a long string using insert.
 *
 * \param action        Method for inserting. MIN_VALUE = begin(),
 *                      MAX_VALUE = end(), RANDOM_VALUE = MID_VALUE = random
 *                      spot in the string.
 */
/*void basicInsertTest(randomness_t action)
{
    TestingString test;
    string        control;
    TestingString::iterator tIter;
    string::iterator        cIter;

    for (size_t i = 0; i < 500; ++i) {
        int r;

        switch (action) {
        case MAX_VALUE:
            tIter = test.end();
            cIter = control.end();
            break;

        case MID_VALUE:
        case RANDOM_VALUE:
            r = maybeRandomInt(test.size(), action);
            tIter = test.begin();
            std::advance(tIter, r);
            cIter = control.begin();
            std::advance(cIter, r);
            break;

        case MIN_VALUE:
        default:
            tIter = test.begin();
            cIter = control.begin();
            break;
        }

        char c = randomChar();
        tIter = test.insert(tIter, c);
        cIter = control.insert(cIter, c);
        checkIterWithControl(test, control, tIter, cIter);
        // We expect half full chunks when inserting only
        checkUtilization(test, 2);
    }
}*/

/// Basic insert tests (with iterator checks)
/*bool insertTest()
{
    // Set up the TestingLogger object
    TestingLogger log("insert test");

    TestingString test;
    string control;

    TestingString::iterator tIter;
    string::iterator cIter;

    // Insert at begin()
    basicInsertTest(MIN_VALUE);

    // Insert at end()
    basicInsertTest(MAX_VALUE);

    // Insert at midpoint
    basicInsertTest(MID_VALUE);

    // Insert randomly
    basicInsertTest(RANDOM_VALUE);

    // Insert at chunk boundary
    for (size_t i = 0; i < 2 * TESTING_CHUNKSIZE; ++i) {
        char c = randomChar();
        test.push_back(c);
        control.push_back(c);
    }

    affirm(test.size() > TESTING_CHUNKSIZE);
    tIter = test.begin();
    std::advance(tIter, TESTING_CHUNKSIZE);
    cIter = control.begin();
    std::advance(cIter, TESTING_CHUNKSIZE);
    tIter = test.insert(tIter, 'z');
    cIter = control.insert(cIter, 'z');
    checkIterWithControl(test, control, tIter, cIter);
    tIter = test.begin();
    std::advance(tIter, TESTING_CHUNKSIZE + 1);
    cIter = control.begin();
    std::advance(cIter, TESTING_CHUNKSIZE + 1);
    tIter = test.insert(tIter, 'D');
    cIter = control.insert(cIter, 'D');
    checkIterWithControl(test, control, tIter, cIter);

    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();
}*/

const size_t LONG_STRING_SIZE = 500;

// Test setup for tests that need a long string at the start.
void setUpLongString(TestingString& testString, string& controlString) {
  for (size_t i = 0; i < LONG_STRING_SIZE; ++i) {
    char c = randomChar();
    testString.push_back(c);
    controlString.push_back(c);
  }
}

/**
 * \brief Erase a long string using erase.
 *
 * \param test          String to erase
 * \param control       Expected value
 * \param action        Method for erasing. MIN_VALUE = begin(),
 *                      MAX_VALUE = end(), RANDOM_VALUE = MID_VALUE = random
 *                      spot in the string.
 */
/*void eraseAllTest(TestingString& test, string& control, randomness_t action)
{
    TestingString::iterator tIter;
    string::iterator cIter;

    while (test.begin() != test.end()) {
        int r;

        switch (action) {
        case MAX_VALUE:
            tIter = --test.end();
            cIter = --control.end();

        case RANDOM_VALUE:
        case MID_VALUE:
            r = maybeRandomInt(test.size() - 1, action);
            tIter = test.begin();
            cIter = control.begin();
            std::advance(tIter, r);
            std::advance(cIter, r);
            break;

        case MIN_VALUE:
        default:
            tIter = test.begin();
            cIter = control.begin();
            break;
        }

        tIter = test.erase(tIter);
        cIter = control.erase(cIter);
        checkIterWithControl(test, control, tIter, cIter);
        // We expect 1/4 full chunks when erasing
        checkUtilization(test, 4);
    }

}*/

/// Basic erase tests (with iterator checks)
/*bool eraseLongStringTest()
{
    TestingString testString;          ///< TestingString created
    string controlString;              ///< Expected value of testString
    setUpLongString(testString, controlString);

    // Set up the TestingLogger object
    TestingLogger log("erase long string test");

    // Make copies of our large strings
    TestingString test = testString;
    string control = controlString;

    TestingString::iterator tIter;
    string::iterator cIter;

    // Erase first character
    eraseAllTest(test, control, MIN_VALUE);

    // Erase last character
    test = testString;
    control = controlString;
    eraseAllTest(test, control, MAX_VALUE);

    // Erase randomly
    test = testString;
    control = controlString;
    eraseAllTest(test, control, RANDOM_VALUE);

    // Make sure push_back still works after erasing
    test.push_back(' ');
    control.push_back(' ');
    checkWithControl(test, control);

    // Erase from chunk boundary
    test    = testString;
    control = controlString;
    affirm(test.size() > TESTING_CHUNKSIZE);
    tIter = test.begin();
    std::advance(tIter, TESTING_CHUNKSIZE - 1);
    cIter = control.begin();
    std::advance(cIter, TESTING_CHUNKSIZE - 1);
    tIter = test.erase(tIter);
    cIter = control.erase(cIter);
    checkIterWithControl(test, control, tIter, cIter);

    affirm(test.size() > TESTING_CHUNKSIZE + 1);
    tIter = test.begin();
    std::advance(tIter, TESTING_CHUNKSIZE);
    cIter = control.begin();
    std::advance(cIter, TESTING_CHUNKSIZE);
    tIter = test.erase(tIter);
    cIter = control.erase(cIter);
    checkIterWithControl(test, control, tIter, cIter);

    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();
}*/

/// Basic iteration tests
bool iterateLongStringTest() {
  TestingString testString;  ///< TestingString created
  string controlString;      ///< Expected value of testString
  setUpLongString(testString, controlString);

  // Set up the TestingLogger object
  TestingLogger log("iterate long string test");

  // Basic forward iteration
  TestingString::iterator tIter = testString.begin();
  string::iterator cIter = controlString.begin();

  for (size_t i = 0; i < LONG_STRING_SIZE; ++i) {
    affirm(*cIter == *tIter);
    ++tIter;
    ++cIter;
  }

  affirm(tIter == testString.end());

  // Basic backward iteration
  tIter = testString.end();
  cIter = controlString.end();

  for (size_t i = 0; i < LONG_STRING_SIZE; ++i) {
    --tIter;
    --cIter;
    affirm(*cIter == *tIter);
  }

  affirm(tIter == testString.begin());

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
    checkWithControl(test, control);
  }

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

/// Advances iter by dist or until it hits end, whichever comes first
void myAdvance(TestingString::iterator& iter,
               const TestingString::iterator& end, size_t dist) {
  while (iter != end && dist > 0) {
    ++iter;
    --dist;
  }
}

/// Try to break the utilization of the string using insert and erase.
/// Strategy: force overflow of each chunk, then erase full chunks.
/*bool utilizationOverflowLongStringTest()
{
    TestingString testString;          ///< TestingString created
    string controlString;              ///< Expected value of testString
    setUpLongString(testString, controlString);

    // Set up the TestingLogger object
    TestingLogger log("utilization overflow long string test");

    // Overflow each chunk
    TestingString::iterator iter = testString.begin();
    advance(iter, TESTING_CHUNKSIZE - 1); // Jump to end of chunk

    for ( ; iter != testString.end();
            // advance by TESTING_CHUNKSIZE + 1 since we are inserting
            myAdvance(iter, testString.end(), TESTING_CHUNKSIZE + 1)) {
        iter = testString.insert(iter, 'x');
        checkUtilization(testString, 2);
    }

    // Erase the full chunks
    for (iter = testString.begin(); iter != testString.end(); ++iter) {
        for (size_t j = 0; j < TESTING_CHUNKSIZE; ++j) {
            if (iter == testString.end()) {
                break;
            }

            iter = testString.erase(iter);
        }

        checkUtilization(testString, 4);

        // Don't ++iter if we are at the end
        if (iter == testString.end()) {
            break;
        }
    }

    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();
}*/

enum insertpoint_t { FRONT, BACK };
enum usepush_t { INSERT_ONLY, PUSH_AND_INSERT };

/**
 * Test insert.  Inserts characters into a string within a locus of 'n' from
 * the front or back (depending on whether where is FRONT or BACK), exactly
 * how the locus is defined depends on randomize and the number of characters
 * currently in the string.
 */
/*void randomInsertTest(size_t n, insertpoint_t where,
          randomness_t randomize, usepush_t method)
{
    // Test data
    static const string TEST_DATA("ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                  "abcdefghijklmnopqrstuvwxyz" "123456789");
    static const size_t TEST_DATA_SIZE = TEST_DATA.size();

    string control;
    TestingString test;

    for (size_t i = 0; i < TEST_DATA_SIZE;  ++i) {
        char c = TEST_DATA[i];

        int m; // numeric index of point where we wish to insert

        if (where == FRONT) {
            m = n > i ? i : maybeRandomInt(n, randomize);
        } else {
            m = n > i ? 0 : i - maybeRandomInt(n, randomize);
        }

        // Find desired insertion point
        TestingString::iterator p = test.begin();
        advance(p, m);

        TestingString::iterator tIter =
            (method == PUSH_AND_INSERT && (p == test.end()))
            ? (test.push_back(c), --test.end())
            : test.insert(p, c);

        string::iterator mIter = control.insert(control.begin() + m, c);

        // Biggest deal, does the string look the way it should
        checkIterWithControl(test, control, tIter, mIter);

        // Utilization okay?
        checkUtilization(test, 4);
    }
}*/

/**
 * Test erase.  Erases characters into a string within a locus of 'n' from
 * the front or back (depending on whether where is FRONT or BACK), exactly
 * how the locus is defined depends on randomize and the number of characters
 * currently in the string.
 */

/*void randomEraseTest(size_t n, insertpoint_t where, randomness_t randomize)
{
    // Test data
    static const string TEST_DATA("ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                  "abcdefghijklmnopqrstuvwxyz" "123456789");
    static const size_t TEST_DATA_SIZE = TEST_DATA.size();

    string control(TEST_DATA);

    TestingString test;

    for (string::const_iterator i = TEST_DATA.begin();
            i != TEST_DATA.end();
            ++i) {
        test.push_back(*i);
    }

    for (size_t i = TEST_DATA_SIZE; i > 0; --i) {
        // The string is not yet empty
        affirm(!(test == TestingString()));

        int m;

        if (where == FRONT) {
            m = n >= i ? i - 1 : maybeRandomInt(n, randomize);
        } else {
            m = n >= i ? 0 : i - 1 - maybeRandomInt(n, randomize);
        }

        TestingString::iterator p = test.begin();
        advance(p, m);

        TestingString::iterator tIter = test.erase(p);
        string::iterator cIter = control.erase(control.begin() + m);

        checkIterWithControl(test, control, tIter, cIter);
        if (i > 0) {
            checkUtilization(test, 4);
        }
    }

    // The string is now empty
    affirm(test == TestingString());
}*/

/// Try to break utilization by pure insertion
/*bool insertionUtilizationTest()
{
    // Set up the TestingLogger object
    TestingLogger log("insertion utilization test");

    for (randomness_t r : { MIN_VALUE, MAX_VALUE, MID_VALUE, RANDOM_VALUE }) {
        for (int i = 0; i < 25; ++i) {
            randomInsertTest(i, FRONT, r, INSERT_ONLY);
        }

        for (int i = 0; i < 25; ++i) {
            randomInsertTest(i, BACK, r, INSERT_ONLY);
        }
    }

    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();
}*/

/// Try to break utilization by a combo of insertion and push_back
/*bool pushAndInsertUtilizationTest()
{
    // Set up the TestingLogger object
    TestingLogger log("push and insert utilization test");

    for (randomness_t r : { MIN_VALUE, MAX_VALUE, MID_VALUE, RANDOM_VALUE }) {
        for (int i = 0; i < 25; ++i) {
            randomInsertTest(i, FRONT, r, PUSH_AND_INSERT);
        }

        for (int i = 0; i < 25; ++i) {
            randomInsertTest(i, BACK, r, PUSH_AND_INSERT);
        }
    }

    // Print a summary of the all the affirmations and return true
    // if they were all successful.
    return log.summarize();
}*/

/// Try to break utilization by pure erasing
/*bool eraseUtilizationTest()
{
    // Set up the TestingLogger object
    TestingLogger log("erase utilization test");

    for (randomness_t r : { MIN_VALUE, MAX_VALUE, MID_VALUE, RANDOM_VALUE }) {
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
}*/

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
  alarm(60);                         // set the timer at 60 seconds

  // Add calls to your tests here...
  affirm(defaultConstructorTest());
  affirm(copyConstructorTest());
  // affirm(assignmentTest());
  affirm(appendTest());
  // affirm(equalityTest());
  affirm(pushBackTest());
  affirm(iterateTest());
  // affirm(insertTest());
  // affirm(eraseLongStringTest());
  affirm(iterateLongStringTest());
  affirm(appendUtilizationTest());
  // affirm(utilizationOverflowLongStringTest());
  // affirm(insertionUtilizationTest());
  // affirm(pushAndInsertUtilizationTest());
  // affirm(eraseUtilizationTest());

  if (alltests.summarize(true)) {
    return 0;  // Error code of 0 == Success!
  } else {
    return 2;  // Arbitrarily chosen exit code of 2 means tests failed.
  }
}
