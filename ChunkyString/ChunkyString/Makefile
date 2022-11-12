# NEW THIS WEEK: MAKEFILE VARIABLES
#  Why bother typing -g -std=c++1z -Wall -Wextra -pedantic for every compile?
#     (and adding a -O2 optimization flags means changing every rule)
#  Why hardcode our Makefile to use clang++ instead of some other C++ compiler?
#  Let's define variables once, and then use those variables in the rules below

CXX = clang++
CXXFLAGS = -g -std=c++1z -Wall -Wextra -pedantic

all: stringtest-ours stringtest-limited  stringtest-complete messagepasser

chunkystring.o: chunkystring.hpp chunkystring.cpp
	$(CXX) -c $(CXXFLAGS) chunkystring.cpp

iterator.o: chunkystring.hpp iterator.cpp
	$(CXX) -c $(CXXFLAGS) iterator.cpp

testing-logger.o: testing-logger.cpp testing-logger.hpp
	$(CXX) -c $(CXXFLAGS) testing-logger.cpp

stringtest-ours.o: stringtest-ours.cpp chunkystring.hpp
	$(CXX) -c $(CXXFLAGS) stringtest-ours.cpp

stringtest-ours: stringtest-ours.o testing-logger.o chunkystring.o iterator.o
	$(CXX) -o stringtest-ours stringtest-ours.o testing-logger.o chunkystring.o iterator.o


# NEW THIS WEEK: IMPLICIT RULES
#   If you say that a .o file depends on a .cpp file (and maybe other things), 
#   Then make automatically assumes that you want to run the command
#       $(CXX) $(CXXFLAGS) -c -o <name of .o file>  <name of .cpp file>
#   unless you specify a different command. Since we have cleverly defined those
#   two Makefile variables above, this means we can get away with writing
#   the dependencies for any simple C++ compile steps, and nothing else.
#   (Yes, we could have done this for the four compile rules above as well.)
#   
#   Note: we *do* have to specify the command to run for the linking step.
#   make does have an implicit rule it can use for turning a bunch of .o
#      files into an executable, but that rule uses uses the *C* compiler
#      and links in the *C* standard library; we need to use the *C++* compiler
#      so that it links in the *C++* standard library.

noisy-transmission.o: noisy-transmission.cpp noisy-transmission.hpp chunkystring.hpp

message-passer.o: message-passer.cpp noisy-transmission.hpp chunkystring.hpp

messagepasser: chunkystring.o message-passer.o noisy-transmission.o
	$(CXX) -o messagepasser chunkystring.o message-passer.o noisy-transmission.o


# NEW THIS WEEK: AUTOMATIC VARIABLES
#   Why bother listing a bunch of .o files as dependencies for a linking rule,
#      and then type the same list of .o files in the linking command?
#   When writing the command(s) in a makefile rule, you can use the following
#   automatically defined variables
#         $@ is a variable containing the name of the target (file being built)
#         $< is a variable containing the first dependency
#         $^ is a variable containing all the dependencies (space-separated)
#         ...etc...

stringtest-limited.o: stringtest-limited.cpp chunkystring.hpp

stringtest-limited: stringtest-limited.o testing-logger.o chunkystring.o iterator.o
	$(CXX) -o $@ $^



# Other targets

doxygen:
	doxygen doxygen.config

test: stringtest-limited stringtest-ours stringtest-complete
	./stringtest-ours && ./stringtest-limited && ./stringtest-complete

clean:
	rm -f stringtest-limited stringtest-ours stringtest-complete messagepasser *.o 

