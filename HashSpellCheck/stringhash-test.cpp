/**
 * \file stringhash-test.cpp
 * \author CS 70 Provided Code.
 *
 * \brief Tests a hash function (by default, myhash) to ensure that it has good
 *        properties.
 */

// Nothing should go above this #include
#if !TEST_STD_HASH
#include "stringhash.hpp"
#endif

#include <chrono>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <forward_list>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <random>
#include <string>
#include <unordered_map>

using std::cerr;
using std::cout;
using std::endl;
using std::forward_list;
using std::ifstream;
using std::string;
using std::unordered_map;
using std::vector;

/**
 * \brief Count the number of bits set to 1 in a size_t
 *
 */
unsigned int countBits(size_t n) {
  // Based on code Published in 1988, the C Programming Language 2nd Ed.
  // (by Brian W. Kernighan and Dennis M. Ritchie) mentions this in
  // exercise 2-9. On April 19, 2006 Don Knuth pointed out to him that
  // this method "was first published by Peter Wegner in CACM 3 (1960),
  // 322. (Also discovered independently by Derrick Lehmer and published
  // in 1964 in a book edited by Beckenbach.)"

  unsigned int count = 0;  // c accumulates the total bits set in v

  while (n != 0) {
    n &= n - 1;  // clear the least significant bit set
    ++count;
  }

  return count;
}

/**
 * \brief Actually do the testing
 *
 */
void doTests(const char* filename, size_t numBuckets,
             size_t (*myhash)(const std::string& str)) {
  // Open provided file
  ifstream inFile(filename);
  if (!inFile) {
    cerr << "Couldn't open file: " << filename << endl;
    exit(1);
  }

  using hashval_t = uint32_t;
  using bucket_t = forward_list<string>;
  using map_t = unordered_map<hashval_t, bucket_t>;
  map_t infiniteBuckets;

  using bucket_count_t = unsigned int;
  vector<bucket_count_t> finiteBuckets(numBuckets);
  size_t itemCount = 0;

  // Get the current time before and after running the benchmark

  using clock = std::chrono::high_resolution_clock;
  clock::time_point startTime = clock::now();

  // Read data from the file, one item per line
  string line;
  while (inFile.good()) {
    getline(inFile, line);
    if (inFile.fail()) break;
    auto hash = myhash(line);
    infiniteBuckets[hash].push_front(line);
    ++finiteBuckets[hash % numBuckets];
    ++itemCount;
  }

  clock::time_point stopTime = clock::now();

  // Compute the elapsed duration, convert that to microseconds, and display

  clock::duration elapsed = stopTime - startTime;
  size_t ticks =
      std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

  cout << "Inserted " << itemCount << " items in ";
  cout << ticks << " ms\n" << std::endl;

  // Print stats for the 'finite' hash table
  double loadFactor = static_cast<double>(itemCount) / numBuckets;

  double accum = 0.0;
  bucket_count_t min = ~0;
  bucket_count_t max = 0;
  size_t empties = 0;
  for (bucket_count_t count : finiteBuckets) {
    if (count == 0) ++empties;
    if (count < min) min = count;
    if (count > max) max = count;
    double diff = count - loadFactor;
    accum += diff * diff;
  }

  double stdev = sqrt(accum / (itemCount - 1));

  cout << "For " << numBuckets << " buckets:"
       << "\n  - Empty bucket percentage:            "
       << double(empties) / double(numBuckets) * 100.0 << "%"
       << "\n  - Min items per bucket:               " << min
       << "\n  - Max items per bucket:               " << max
       << "\n  - Average items per bucket:           " << loadFactor
       << "\n  - Standard deviation:                 " << stdev
       << "\n  - Average items per non-empty bucket: "
       << double(itemCount) / double(numBuckets - empties) << endl;

  cout << "\nFor 'infinite' buckets (actually 2^32):" << endl;

  // Print stats for the 'infinite' hash table

  size_t collisions = 0;
  for (const map_t::value_type& bucket : infiniteBuckets)
    collisions += distance(bucket.second.begin(), bucket.second.end()) - 1;

  // First, print expected vs actual collisions
  //
  // We use the theory from class to predict how many collisions we ought
  // to expect.
  double expected, stddev;
  if (itemCount == 0) {
    expected = stddev = 0.0;
  } else {
    double buckets = pow(2.0, sizeof(hashval_t) * 8);
    // 2^(no of bits stored)
    double powerPart = pow((buckets - 1.0) / buckets, itemCount - 1);
    expected = powerPart * (1.0 - itemCount) + (1.0 - powerPart) * buckets;
    stddev = sqrt(
        ((powerPart * (-1 + buckets + itemCount) * 1)) *
        (1.0 - powerPart * itemCount + (1.0 - powerPart) * (buckets - 1.0)) /
        buckets);
  }
  cout << "  - Expected: " << expected << " hash collisions, stddev " << stddev
       << endl;

  cout << "  - Actual:   " << infiniteBuckets.size() << " unique hashes, ";
  if (collisions == 0) {
    cout << "no hash collisions!" << endl;
  } else {
    cout << collisions << " hash collisions:\n  - Specifically:" << endl;

    unsigned int limit = 100;
    for (const map_t::value_type& bucket : infiniteBuckets) {
      size_t bucket_size = distance(bucket.second.begin(), bucket.second.end());
      if (bucket_size > 1) {
        if (--limit == 0) {
          cout << "\t... etc. ..." << endl;
          break;
        }
        cout << "\t" << bucket.first << ":";
        unsigned int innerLimit = 20;
        for (const string& word : bucket.second) {
          if (--innerLimit == 0) {
            cout << "  [...]";
            break;
          }
          cout << " " << word;
        }
        cout << endl;
      }
    }
  }

  // Calculate average avalanche
  //   (the number of bits that change in the output
  //      when one bit changes in the input).
  //
  //   This is one way to measure whether small changes in the
  //   output result in large changes in the input (so that,
  //   for example, even if someone gives us lots of very similar
  //   keys, they will end up well spread out in the hash table).
  cout << "\nAverage avalanche:" << endl;

  // Create a C++11 random engine, using the machine's system-specific
  // entropy source to provide the seeds.
  std::default_random_engine randomEngine(std::random_device {}());

  for (int stringSize : {1, 2, 4, 8, 16, 32, 64, 128, 256}) {
    using small_uniform_dist_t = std::uniform_int_distribution<uint8_t>;
    constexpr int ITERS = 1000000;

    small_uniform_dist_t arbitraryChar(0, UCHAR_MAX);
    string trialString;
    for (int i = 0; i < stringSize; ++i)
      trialString.push_back(arbitraryChar(randomEngine));

    size_t previousHash = myhash(trialString);

    small_uniform_dist_t arbitraryIndex(0, stringSize - 1);
    small_uniform_dist_t arbitraryBit(0, CHAR_BIT - 1);

    unsigned int total_delta = 0;
    for (int i = 0; i < ITERS; ++i) {
      uint8_t theChar = arbitraryIndex(randomEngine);
      uint8_t theBit = arbitraryBit(randomEngine);
      trialString[theChar] ^= 1 << theBit;  // flip one bit
      size_t thisHash = myhash(trialString);
      size_t hashDiff = previousHash ^ thisHash;
      unsigned int delta = countBits(hashDiff);
      // cout << "  - " << thisHash << ", delta = "
      //      << delta << " bits" << endl;
      total_delta += delta;
      previousHash = thisHash;
    }

    cout << "\t" << double(total_delta) / double(ITERS) << "/"
         << sizeof(size_t) * 8 << " bits for one bit change in " << stringSize
         << " character key" << endl;
  }
}

/**
 * \brief Driver for testing
 *
 */
int main(int argc, char** argv) {
  // Process the arguments
  if (argc != 3) {
    cerr << "Usage: stringhash-test num-buckets file-of-words" << endl;
    return 1;
  }

  size_t numBuckets = strtoul(argv[1], nullptr, 0);
  if (numBuckets == 0 || numBuckets > 1000000) {
    cerr << "Please give a sane number of buckets!" << endl;
    return 2;
  }

  const char* filename = argv[2];

  for (const HashFunctionInfo& test : hashInfo) {
    cout << "== Testing " << test.name_ << "\n\n";
    doTests(filename, numBuckets, test.func_);
    cout << endl;
  }

  return 0;
}
