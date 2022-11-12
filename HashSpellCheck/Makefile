
# Makefile for CS 70 Spell Checking Assignment
#

###############################################################################
# MAKEFILE VARIABLES
###############################################################################

# CXX is the name of the compiler we are using (clang++)

CXX = clang++

# CXXFLAGS are the flags we will be passing each compile

CXXFLAGS = -g -std=c++1z -Wall -Wextra -pedantic

# TARGETS is the list of all programs created when we do "make all"
#   (and which should be deleted when we do "make clean")

TARGETS = stringhash-test hashset-cow-test our-tests myspell

###############################################################################
# "PHONY" (BUT USEFUL) MAKEFILE TARGETS
###############################################################################


# "make all" brings all programs up-to-date (recursively)
#     and then runs no commands.

all: $(TARGETS)

# "make clean" brings nothing up to date, but always
#      runs commands to delete all created files

clean:
	rm -f $(TARGETS)
	rm -rf *.o

# "make test" brings all the testing programs up-to-date (recursively)
#       and then runs them.
# You should probably use a more realistic number of buckets than 10
#       for stringhash-test, but the exact number is up to you.

test: stringhash-test hashset-cow-test
	./stringhash-test 10 /usr/share/dict/web2
	./hashset-cow-test

doxygen: doxygen.config
	doxygen doxygen.config

###############################################################################
# CREATING PROGRAMS
###############################################################################


# In a command run by the makefile, $^ stands for the things the target
#      depends on (in this case, the list stringhash-test.o stringhash.o")

stringhash-test: stringhash-test.o stringhash.o
	$(CXX) -o stringhash-test $^


hashset-cow-test: hashset-cow-test.o testing-logger.o
	$(CXX) -o hashset-cow-test $^

our-tests: our-tests.o testing-logger.o
	$(CXX) -o our-tests $^

myspell: myspell.o stringhash.o 
	$(CXX) -o myspell $^

###############################################################################
# GENERATING OBJECT FILES
###############################################################################

# The .o files depend on the corresponding .cpp file and the .hpp
#      files that C++ code includes.
#      In each case, the command to generate the .o file uses
#      our C++ compiler to compile the .cpp file, with the -c flag.

hashset-cow-test.o: hashset-cow-test.cpp hashset.hpp hashset-private.hpp testing-logger.hpp
	$(CXX) $(CXXFLAGS) -c hashset-cow-test.cpp

our-tests.o: our-tests.cpp hashset.hpp hashset-private.hpp testing-logger.hpp
	$(CXX) $(CXXFLAGS) -c our-tests.cpp

# Or we can take advantage of the implicit .cpp-to-.o rule
stringhash.o: stringhash.cpp stringhash.hpp
stringhash-test.o: stringhash-test.cpp stringhash.hpp
testing-logger.o: testing-logger.cpp testing-logger.hpp
myspell.o: myspell.cpp stringhash.hpp hashset.hpp hashset-private.hpp
