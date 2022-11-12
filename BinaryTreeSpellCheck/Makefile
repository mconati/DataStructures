#
# Makefile for CS 70 TreeStringSet Assignment
#


all: tree-test myspell

tree-test: tree-test.o treestringset.o testing-logger.o
	clang++ -o tree-test tree-test.o treestringset.o testing-logger.o

treestringset.o: treestringset.cpp treestringset.hpp testing-logger.hpp
	clang++ -c -g -std=c++1z -Wall -Wextra -pedantic treestringset.cpp

testing-logger.o: testing-logger.cpp testing-logger.hpp
	clang++ -c -g -std=c++1z -Wall -Wextra -pedantic testing-logger.cpp

tree-test.o: tree-test.cpp treestringset.hpp testing-logger.hpp
	clang++ -c -g -std=c++1z -Wall -Wextra -pedantic tree-test.cpp

myspell.o: myspell.cpp treestringset.hpp
	clang++ -c -g -std=c++1z -Wall -Wextra -pedantic myspell.cpp

myspell: myspell.o treestringset.o
	clang++ -o myspell myspell.o treestringset.o

clean:
	rm -rf tree-test *.o *.dSYM
