#include "testing-logger.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>

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
    if (assertion) {
      return;
    }
    std::cerr << "FAILURE (with no logger): " << description << "\n";
    abort();
  }

  AssertInfo& test = currentLogger->assertions_[description];
  ++test.asserts_;
  if (assertion) {
    return;
  }
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
      if (verbose || assertion.second.failures_ > 0) {
        std::cerr << assertion.second.failures_ << "\t/ "
                  << assertion.second.asserts_ << "\t" << assertion.first
                  << "\n";
      }
    std::cerr << std::endl;
  }
  clear();
  return success;
}
