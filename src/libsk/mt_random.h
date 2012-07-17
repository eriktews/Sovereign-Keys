// Copyright 2012 the SK authors. All rights reserved.

#ifndef LIBSK_MT_RANDOM_H_
#define LIBSK_MT_RANDOM_H_

#include <stddef.h>
#include <stdint.h>
#include "util.h"

namespace sk {

// |MTRandom| implements Mersenne twister MT19937-64, a pseudo-random
// number generator that returns 64 bit values. It is provided to make
// tests that depend on pseudo-random numbers portable. It may also be
// useful for algorithmic randomization.
//
// This class is NOT safe for cryptographic use.
class MTRandom {
  public:
    MTRandom();

    // Seeds the generator with |value|.
    void Seed(uint64_t value);

    // Returns the next pseudo-random value.
    uint64_t GetRandom();

  protected:
    static const int kMT19937_64StateSize = 312;

  private:
    DISALLOW_EVIL_CONSTRUCTORS(MTRandom);

    void GenerateState();

    uint64_t state_[kMT19937_64StateSize];
    int pos_;
};
}  // namespace sk

#endif  // LIBSK_MT_RANDOM_H_
