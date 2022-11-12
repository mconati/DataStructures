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
  throw std::logic_error("Not yet implemented");
}
