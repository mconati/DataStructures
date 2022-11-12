/**
 * \file noisytransmit.hpp
 * \author CS70 Provided Code
 *
 * \brief Defines the NoisyTransmission class for stepping through a
 * chunkystring and randomly doubling, erasing, or leaving each character
 * unchanged.
 *
 */

#ifndef NOISYTRANSMISSION_HPP_INCLUDED
#define NOISYTRANSMISSION_HPP_INCLUDED 1

#include <random>
#include "chunkystring.hpp"

class NoisyTransmission {
 public:
  /**
   * \brief Create a noisy channel with a given error rate
   *
   * \details When the `transmit` member function is called, each character
   *          in the transmitted string has an `errorRate` probability
   *          being doubled, an `errorRate` probability of being deleted,
   *          and 1 - 2 * `errorRate` probability of being left alone.
   */
  explicit NoisyTransmission(float errorRate);

  /// Transmit a message over the noisy channel (modifying it in-place)
  void transmit(ChunkyString& message);

 private:
  float errorRate_;  ///< Probability of doubling and probability of deleting
  std::uniform_real_distribution<> dis_;  ///< Uniformly distributed floats
  std::mt19937 gen_;                      ///< Random-number generator

  /// Seed the random-number generator
  void seed();

  /// Get a random number
  float getRandomFloat();
};

#endif
