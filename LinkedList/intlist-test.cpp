/**
 * \file intlist-test.cpp
 * \authors CS 70 Starter Code
 * \brief The program unit tests the IntList class.
 *
 * \details Part of the CS 70 IntList assignment.
 */

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "intlist.hpp"
#include "testing-logger.hpp"

///////////////////////////////////////////////////////////
//  TESTING
///////////////////////////////////////////////////////////

// ISSUE 2 TESTS

/** \brief Test default constructor for IntLists
 */
bool defaultConstructorTest() {
  // Set up the TestingLogger object
  TestingLogger log("default constructor");

  // Set up for your test
  IntList myList;

  // Add checks for things that should be true.
  affirm(myList.size() == 0);

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

// ISSUE 3 TESTS -------------------------------------------------
// Add tests here for empty, push_front, and pop_front

/** \brief Test empty
 *  \detail Tests that empty is true for a default intlist, and that
 *          it is false for a non-empty intlist.
 */
bool emptyTest() {
  // Set up the TestingLogger object
  TestingLogger log("empty");

  // Set up for your test
  IntList myList;

  affirm(myList.empty());

  affirm(myList.size() == 0);

  myList.push_front(1);
  affirm(!myList.empty());
  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

// Following the above model, add tests for push/pop front here:
/** \Test push_front
 *  \detail Tests that push_front adds a new element--
 *  size should be incremented.
 */
bool push_frontTest() {
  // Set up the TestingLogger object
  TestingLogger log("empty");

  // Set up for your test
  IntList myList;

  // Testing pushing one integer
  myList.push_front(4);
  affirm(myList.size() == 1);

  myList.push_front(4);
  myList.push_front(4);
  myList.push_front(4);
  affirm(myList.size() == 4);
  affirm(!myList.empty());
  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}

/** \Test pop_front
 *  \detail Tests that pop_front removes the front element
 *     and returns the int value stored in said element.
 *     Check that size has been decremented each time, and ints
 *     match each other.
 */
bool pop_frontTest() {
  // Set up the TestingLogger object
  TestingLogger log("empty");

  // Set up for your test
  IntList myList;
  // Testing that pushing and popping is done in the correct order
  // for just one push and one pop!
  myList.push_front(1);
  affirm(myList.pop_front() == 1);
  affirm(myList.empty());

  // Testing that pushing and popping is done in the correct order
  // for multiple pops!
  for (int i=0; i < 11; ++i) {
    myList.push_front(i);
  }
  for (int i=0; i < 11; ++i) {
    affirm(myList.pop_front() == (10-i));
    affirm(myList.size() == size_t((10-i)));
  }
  affirm(myList.empty());
  return log.summarize();
}
// ...
// ...

// ISSUE 4 TESTS -------------------------------------------------
// Do not write tests that call the IntList destructor!
// Instead, be sure to run valgrind

// ISSUE 5 TESTS -------------------------------------------------
// Add tests here for begin, end; and for Iterator *, ++ and ==
bool iteratorTests() {
  // Set up the TestingLogger object
  TestingLogger log("empty");

  // Set up for your test
  IntList iteratorList;
  // test begin, end, and == on an empty list
  affirm(iteratorList.begin() == iteratorList.end());
  // make a big list
  for (int i=0; i < 101; ++i) {
    iteratorList.push_front(i);
  }
  int i = 100;
  // check begin, end, ++, and * by iterating through the long
  // list and checking the value of each
  for (IntList::iterator checkingIterator = iteratorList.begin();
    checkingIterator != iteratorList.end(); ++checkingIterator) {
          affirm(*checkingIterator == i);
          --i;
  }
  return log.summarize();
}



// ISSUE 6 TESTS -------------------------------------------------
bool equalityOperatorTest() {
  // Set up the TestingLogger object
  TestingLogger log("empty");

  // Set up for the test
  IntList lhs;
  IntList rhs;
  // Check for empty IntLists
  affirm(rhs == lhs);
  // fill up the lists
  for (int i=0; i < 101; ++i) {
    lhs.push_front(i);
    rhs.push_front(i);
  }
  // Check for now full IntLists
  affirm(rhs == lhs);
  lhs.pop_front();
  // Check for now different length lists
  affirm(rhs != lhs);
  lhs.push_front(10000);
  // Check for same length but different lists
  affirm(rhs != lhs);
  return log.summarize();
}
// ISSUE 7 TESTS -------------------------------------------------
// Add tests here for push_back and insert_after
bool pushBackTest() {
  // Set up the TestingLogger object
  TestingLogger log("empty");

  // Set up for the test
  IntList testList;
  IntList compareList;
  // Set up the lists to both have value 1 in back_ and 2 in front_
  compareList.push_front(1);
  compareList.push_front(2);

  testList.push_back(2);
  // make sure it adjusts size
  affirm(testList.size() == 1);
  testList.push_back(1);
  affirm(testList == compareList);

  return log.summarize();
}

bool insertAfterTest() {
  // Set up the TestingLogger object
  TestingLogger log("empty");

  // Set up for the test
  IntList testList;

  for (int i = 0; i < 3; ++i) {
    testList.push_back(i);
  }

  IntList::iterator where = testList.begin();
  affirm(testList.size() == 3);

  ++where;
  testList.insert_after(where, 1);
  affirm(testList.size() == 4);
  ++where;
  affirm(*where == 1);
  ++where;
  affirm(*where == 2);

  return log.summarize();
}

bool copyConstructorTest() {
  // Set up the TestingLogger object
  TestingLogger log("empty");

// Set up a list and a copy
  IntList myList;
  for (size_t i = 0; i < 101; ++i) {
    myList.push_front(i);
  }
  affirm(myList.size() == 101);

  IntList copyList = myList;

// Affirm that copying actually copied every element
  IntList::iterator copyTester = copyList.begin();
  for (IntList::iterator tester = myList.begin();
  tester != myList.end(); ++tester) {
    affirm(*copyTester == *tester);
    ++copyTester;
  }
  return log.summarize();
}
/*
 * Test the IntList!
 */
int main(int, char**) {
  TestingLogger alltests("All tests");

  affirm(defaultConstructorTest());

  // To do: add your new tests as new affirms here...

  affirm(emptyTest());

  // Print a summary of the all the affirmations and exit the program.
  affirm(push_frontTest());

  affirm(pop_frontTest());

  affirm(iteratorTests());

  affirm(equalityOperatorTest());

  affirm(pushBackTest());

  affirm(insertAfterTest());

  affirm(copyConstructorTest());

  if (alltests.summarize(true)) {
    return 0;  // Error code of 0 == Success!
  } else {
    return 2;  // Arbitrarily chosen exit code of 2 means tests failed.
  }
}
