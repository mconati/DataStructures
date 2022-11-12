/**
 * \file tree-test.cpp
 * \authors macaw/owl
 * \brief The program unit tests the TreeStringSet class.
 *
 * \details Part of the CS 70 TreeStringSet assignment.
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

#include "testing-logger.hpp"
#include "treestringset.hpp"

///////////////////////////////////////////////////////////
//  TESTING
///////////////////////////////////////////////////////////

/* Default Constructor Testing:
* Should create an empty tree of size zero and height -1
*/
bool defaultConstructorTest() {
  // Set up the TestingLogger object
  TestingLogger log("default constructor");

  // Should set up an empty tree
  TreeStringSet emptyTest;
  affirm(emptyTest.size() == 0);
  affirm(emptyTest.height() == -1);

  // Prints result of testing
  return log.summarize();
}

/* Testing size and insert functions
* First, we'll test the edge case of creating an empty
* tree and making sure that size = 0
* Insert a single element, check that size = 1
* Insert multiple elements in a loop, then check size after each iteration.
*/
bool sizeAndInsertTest() {
  // Set up the TestingLogger object
  TestingLogger log("empty");

  // Set up for your test
  TreeStringSet myTree;
  std::string alphabet[26] = { "a", "b", "c", "d", "e", "f",
  "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r",
  "s", "t", "u", "v", "w", "x", "y", "z" };
  // Testing pushing one integer
  affirm(myTree.size() == 0);

  myTree.insert("string");
  affirm(myTree.size() == 1);

  for (size_t i = 0; i < 10; ++i) {
    myTree.insert(alphabet[i]);
    affirm(myTree.size() == (i + 2));
  }
  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}
/* Checks if an element exists in an empty tree as an edge case
* Then checks if an element exists that is the root of the tree
* and then checks if an Element exists that is a child somewhere
* in that tree!
*/
bool existsTest() {
  // Set up the TestingLogger object
  TestingLogger log("empty");

  // Set up for your test
  TreeStringSet myTree;

  // Testing pushing one integer
  affirm(myTree.exists("root") == false);
  // Insert an element and check that it exists
  // make sure that elements we didn't add don't exist
  myTree.insert("element");
  affirm(myTree.exists("element") == true);
  affirm(myTree.exists("root") == false);

  // Add more elements and make sure they exist
  myTree.insert("otherElement");
  myTree.insert("evenMoreElement");
  affirm(myTree.exists("element") == true);
  affirm(myTree.exists("otherElement") == true);
  affirm(myTree.exists("evenMoreElement") == true);
  affirm(myTree.exists("root") == false);

  // Print a summary of the all the affirmations and return true
  // if they were all successful.
  return log.summarize();
}
/* Tests the height function by making sure that height is
* bounded by a constant factor(we chose 3) times log2(n).
* We can't test an exact height since the tree is 
* randomly created. */
bool heightTest() {
  TestingLogger log("empty");
  std::string alphabet[26] = { "a", "b", "c", "d", "e", "f",
  "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r",
  "s", "t", "u", "v", "w", "x", "y", "z" };
  TreeStringSet myTree;
  for (size_t i = 0; i < 26; ++i) {
      myTree.insert(alphabet[i]);
  }
  // 12 since 3 * log2(26) is about 12 and it should be better
  // than a constant times log2(n)
  affirm(myTree.height() <= 12);
  affirm(myTree.height() > 0);
  std::cout << myTree.height() << std::endl;
  myTree.showStatistics(std::cout);
return log.summarize();
}

int main(int, char**) {
  TestingLogger alltests("All tests");
  affirm(defaultConstructorTest());

  affirm(heightTest());

  affirm(sizeAndInsertTest());

  affirm(existsTest());
  if (alltests.summarize(true)) {
    return 0;  // Error code of 0 == Success!
  } else {
    return 2;  // Arbitrarily chosen exit code of 2 means tests failed.
  }
}
