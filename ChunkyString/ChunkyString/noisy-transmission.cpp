/**
 * \file noisytransmit.cpp
 * \author Fill me in!
 *
 * \brief Defines the noisyTransmit() function for stepping through a
 * chunkystring and randomly doubling, erasing, or leaving each character
 * unchanged.
 *
 * \details
 *
 * \remarks
 *
 */
#include <fstream>
#include <iostream>
#include <random>
#include "noisy-transmission.hpp"
#include <stdexcept>
#include "chunkystring.hpp"

NoisyTransmission::NoisyTransmission(float errorRate)
    : errorRate_(errorRate), dis_(0, 1) {
  seed();
}

void NoisyTransmission::seed() {
  std::random_device rd;
  gen_.seed(rd());
}

float NoisyTransmission::getRandomFloat() { return dis_(gen_); }

void NoisyTransmission::transmit(ChunkyString& message) {
  // set up random value
  float p = 0; // <- fix
  for (auto j = message.begin(); j != message.end(); ++j) {
    p = getRandomFloat();
    if (p < errorRate_) {
      // delete based on error rate chance
      message.erase(j);
    } else if (p < (2 * errorRate_)) {
      // duplicate based on error rate chance
      message.insert(j, *j);
    } else {
      // otherwise do nothing :)
    }
  }
}
