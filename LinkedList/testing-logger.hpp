#ifndef TESTING_LOGGER_HPP_INCLUDED
#define TESTING_LOGGER_HPP_INCLUDED 1

#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

/*****
 * Affirm Logger
 *
 * Affirm is like assert, but it doesn't halt the
 * program. Instead, it counts how many times the affirmation fails.
 *****/

class TestingLogger {
public:
    TestingLogger(std::string name);
    ~TestingLogger();
    static void check(bool assertion, std::string description);
    
    // This version is like check, but takes a lambda function that
    // returns a bool.  It'll call the function in an environment where any
    // exceptions are caught.  This code is unusual in a number of ways.
    template <typename Function>
    static void checkSafely(Function assertionFn, std::string description)
    {
        try {
            check(assertionFn(), description);
        }
        catch (...) {
            check(false, description);
            throw;
        }
    }
    
    bool summarize(bool verbose = false);
    void clear();
    void abortOnFail();
    
    static TestingLogger* currentLogger;    

private:
    struct AssertInfo {
        int asserts_ = 0;
        int failures_ = 0;
    };

    std::map<std::string, AssertInfo> assertions_;
    using const_iter = std::map<std::string, AssertInfo>::const_iterator;
    using iter       = std::map<std::string, AssertInfo>::iterator;
    std::string testName_;
    bool failedSome_;
    bool abortOnFail_;
    TestingLogger* previousLogger_;
};

// C has a global macro called assert, here we create our own called affirm.

#define affirm(isTrue) \
        TestingLogger::checkSafely([&]{ return isTrue; }, \
            std::string(__FILE__) + ":" + std::to_string(__LINE__) \
                + ":\t" + #isTrue)

#endif // TESTING_LOGGER_HPP_INCLUDED
