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


using std::cerr;
using std::cout;
using std::endl;
using std::string;

/**
 * This function processes input from cin and compares against a dictionary
 *        If the word is not found and has not been seen before, spelling
 *        corrections are printed to standard output.
 *
 * Parameters: dictfile - Name of dictionary file.
 *             debug    - Boolean corresponding to the -d flag
 */
void spellcheck(const string& dictfile, bool debug) {
  // read in dictionary file
  string line;
  std::ifstream dictionary(dictfile);
  HashSet<string> dict;

  if (dictionary.is_open()) {
    while (getline(dictionary, line)) {
      dict.insert(line);
    }
    dictionary.close();
  }

  // if debug, showStatistics
  if (debug) {
    dict.showStatistics(cerr);
  }

  const char letters[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
    'z'};
  HashSet<string> misspelledWords;

  std::string word;
  while (std::cin >> word) {
    // process the word
    if (!isalpha(word.front())) {
      word.erase(0, 1);
    }
    if (!isalpha(word.back())) {
      word.pop_back();
    }
    for (size_t i = 0; i < word.length(); ++i) {
      if (isalpha(word[i])) {
        word[i] = tolower(word[i]);
      }
    }
    // check if word is in dictionary
    if (!dict.exists(word) && !misspelledWords.exists(word)
      && word.compare("") != 0) {
      misspelledWords.insert(word);
      cout << word << ": ";

      // then, generate set of permutations of word that are in the dictionary
      for (size_t j = 0; j < word.length(); ++j) {
        string temp = word;
        for (size_t i = 0; i < 26; ++i) {
          temp[j] = letters[i];
          if (dict.exists(temp)) {
            // print those words
            cout << temp << " ";
          }
        }
      }
      cout << endl;
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
  if (argc == 2 && *argv == string("-d")) {
    debug = true;
    --argc;
    ++argv;
  }

  // For correct usage, there should be just one command-line option left
  // If not, print an error to tell the user how they should use this program
  if (argc != 1) {
    cerr << "Usage: ./myspell [-d] dict" << endl;
    exit(2);
  }

  // Check that spelling! Run spellcheck
  try {
    spellcheck(argv[0], debug);
  } catch (std::runtime_error err) {
    cerr << err.what() << endl;
    return 1;
  }

  return 0;
}
