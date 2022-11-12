/**
 * File: myspell.cpp
 *
 * Description: Implements a spelling checker
 */

#include <iostream>
#include <stdexcept>
#include <string>

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
  // TODO(you): spellcheck all the words in standard input.
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
