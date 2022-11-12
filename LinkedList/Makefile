#
# Makefile for CS 70 IntList Assignment
#
# As usual, you can build this software by typing
#
#     make
#

all: intlist-test

intlist-test: intlist-test.o intlist.o testing-logger.o
	clang++ -o intlist-test intlist-test.o intlist.o testing-logger.o

intlist.o: intlist.cpp intlist.hpp testing-logger.hpp
	clang++ -c -g -std=c++1z -Wall -Wextra -pedantic intlist.cpp

testing-logger.o: testing-logger.cpp testing-logger.hpp
	clang++ -c -g -std=c++1z -Wall -Wextra -pedantic testing-logger.cpp

intlist-test.o: intlist-test.cpp intlist.hpp testing-logger.hpp
	clang++ -c -g -std=c++1z -Wall -Wextra -pedantic intlist-test.cpp

clean:
	rm -rf intlist-test *.o *.dSYM
