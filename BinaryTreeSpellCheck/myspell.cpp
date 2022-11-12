/**
 * File: myspell.cpp
 *
 * Description: Implements a spelling checker
 */

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include "treestringset.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::string;


/**
* Helper function that processes input words by making lower case
* and removing any leading or trailing non-alphabetical chars
*/
std::string processing(std::string word) {
  // get rid of any leading and trailing punctuation
  while (!word.empty() && !isalpha(word[0])) {
    word.erase(0, 1);
  }
  while (!word.empty() && !isalpha(word.back())) {
    word.pop_back();
  }
  if (!word.empty()) {
    for (size_t i = 0; i < word.length(); ++i) {
      if (isalpha(word[i])) {
        word[i] = tolower(word[i]);
      }
    }
  }
  return word;
}

/**
* Helper function that takes in a word that is not in the dictionary
* and looks for words that are really similar and are in the dictionary
* by looping through each letter in the word and changing it to see if
* it makes a valid word.
*/
std::string correcting(std::string word, const TreeStringSet& myDict) {
  char insertee = 'a';
  std::string correctedWords = word;
  correctedWords += ":";
  for (size_t i = 0; i < word.length(); ++i) {
    for (int j = 0; j < 26; ++j) {
      std::string tempWord = word;
      tempWord[i] = (insertee + j);
      if (myDict.exists(tempWord)) {
        correctedWords += " " + tempWord;
      }
    }
  }
  return correctedWords;
}

/**
 * This function processes input from cin and compares against a dictionary
 *        If the word is not found and has not been seen before, spelling
 *        corrections are printed to standard output.
 *
 * Parameters: dictfile - Name of dictionary file.
 *             debug    - Boolean corresponding to the -d flag
 */
void spellcheck(const string& dictfile, bool debug) {
  // make our dictionary in tree form
  TreeStringSet myDict;
  std::string allCorrections;
  std::string word;
  std::ifstream inputFile(dictfile);
  while (inputFile >> word) {
    myDict.insert(word);
  }
  if (debug == true) {
    myDict.showStatistics(std::cerr);
  }

  // Process the words to eliminate punctuation and then
  // if not in the dictionary, build the list of corrections
  // that are possible for the word.
  std::string inputWord;
  while (std::cin >> inputWord) {
    std::string processedWord = processing(inputWord);
    if (!processedWord.empty() && !myDict.exists(processedWord)) {
      std::string correctedWords = correcting(processedWord, myDict);
      allCorrections += correctedWords + "\n";
    }
  }
  // Print the corrected words
  std::cout << allCorrections;
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
