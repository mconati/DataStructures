#include "testing-logger.hpp"

#include <cxxabi.h>

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

TestingLogger* TestingLogger::currentLogger = nullptr;

TestingLogger::TestingLogger(std::string name)
    : testName_{name},
      failedSome_{false},
      abortOnFail_{false},
      previousLogger_{currentLogger} {
  currentLogger = this;
}

TestingLogger::~TestingLogger() {
  currentLogger = previousLogger_;
  if (failedSome_) {
    std::cerr << "[automatic summary before information is lost]\n";
    summarize();
  }
}

void TestingLogger::abortOnFail() { abortOnFail_ = true; }

void TestingLogger::check(bool assertion, std::string description) {
  if (!currentLogger) {
    if (assertion) return;
    std::cerr << "FAILURE (with no logger): " << description << "\n";
    abort();
  }

  AssertInfo& test = currentLogger->assertions_[description];
  ++test.asserts_;
  if (assertion) return;
  ++test.failures_;
  currentLogger->failedSome_ = true;
  if (test.failures_ == 1) {
    std::cerr << "FAILURE (after " << (test.asserts_ - test.failures_)
              << " passes): " << description << "\n";
  }
  if (currentLogger->abortOnFail_) {
    currentLogger->summarize(true);
    abort();
  }
}

void TestingLogger::clear() {
  assertions_.clear();
  failedSome_ = false;
}

bool TestingLogger::summarize(bool verbose) {
  bool success = !failedSome_;
  if (success && !verbose) {
    std::cerr << testName_ << " passed!\n";
  } else {
    if (success) {
      std::cerr << "\n"
                << testName_ << " passed! "
                << "Summary of affirmations:\n";
    } else {
      std::cerr << "\nSummary of affirmation failures for " << testName_
                << "\n";
    }
    std::cerr << "----\n"
              << "Fails\t/ Total\tIssue\n";
    for (const auto& assertion : assertions_)
      if (verbose || assertion.second.failures_ > 0)
        std::cerr << assertion.second.failures_ << "\t/ "
                  << assertion.second.asserts_ << "\t" << assertion.first
                  << "\n";
    std::cerr << std::endl;
  }
  clear();
  return success;
}

// based on http://stackoverflow.com/a/24997351/847987

static std::string demangle(const char* mangled) {
  int status = 0;
  std::string result;
  char* buffer = abi::__cxa_demangle(mangled, nullptr, nullptr, &status);
  if (status == 0) {
    result = buffer;
    std::free(buffer);
    return result;
  } else {
    result = mangled;
    if (status == -1) {
      result += " [can't demangle, memory allocation failure]";
    } else if (status == -2) {
      result += " [can't demangle, invalid name]";
    } else {
      result += " [can't demangle, other error]";
    }
    return result;
  }
}

void TestingLogger::exn_fail(std::exception* exn, std::string what,
                             std::string description) {
  std::string exception_details = demangle(
      exn ? typeid(*exn).name() : abi::__cxa_current_exception_type()->name());
  if (what.size() > 0) {
    exception_details += " -- " + what;
  }

  std::cerr << "!! Unexpected exception: " << exception_details << std::endl;
  check(false, description);
}
