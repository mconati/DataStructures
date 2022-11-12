/**
 * File: myspell.cpp
 *
 * Description: Implements a spelling checker
 */

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

// Because of C++ template rules, the string-hashing functions must
//   be known to the compiler *before* it learns about the HashSet
//   template class -- even though that means the headers aren't
//   in alphabetical order.
#include "stringhash.hpp"
#include "hashset.hpp"

/**
 * This function processes input from cin and compares against a dictionary
 *        If the word is not found and has not been seen before, spelling
 *        corrections are printed to standard output.
 *
 * Parameters: dictfile - Name of dictionary file.
 *             debug    - Boolean corresponding to the -d flag
 */
void spellcheck(const std::string& dictfile, bool debug) {
  // Open the dictionary file.
  std::ifstream inputFile(dictfile);
  if (!inputFile.good()) {
    std::cerr << "Error: Unable to open input file: " << dictfile << "\n";
    return;
  }

  // Load the dictionary words into a hash table.
  HashSet<std::string> correctWords;
  std::string dictWord;
  while (inputFile >> dictWord) {
    // Adds the words to a set
    correctWords.insert(dictWord);
  }

  // Optionally show statistics
  if (debug) correctWords.showStatistics(std::cerr);

  // Do the spell checking of words from standard input.
  std::string checkWord;
  HashSet<std::string> misspellingsSeen;
  while (std::cin >> checkWord) {
    // If a novel misspelled word is found, report it.
    if (!correctWords.exists(checkWord) &&
                    !misspellingsSeen.exists(checkWord)) {
      misspellingsSeen.insert(checkWord);
      std::cout << checkWord << " unrecognized";
      std::cout << std::endl;
    }
  }
}

// Main function: processes input and runs the spellcheck function
int main(int argc, const char** argv) {
  bool debug = false;

  // Process command-line options
  --argc;  // Skip past 0th arg (program name)
  ++argv;

  // If the user specified debug mode with the -d flag, turn on debugging
  if (argc == 2 && *argv == std::string("-d")) {
    debug = true;
    --argc;
    ++argv;
  }

  // For correct usage, there should be just one command-line option left
  // If not, print an error to tell the user how they should use this program
  if (argc != 1) {
    std::cerr << "Usage: ./myspell [-d] dict" << std::endl;
    exit(2);
  }

  // Check that spelling! Run spellcheck
  try {
    spellcheck(argv[0], debug);
  } catch (std::runtime_error err) {
    std::cerr << err.what() << std::endl;
    return 1;
  }

  return 0;
}
