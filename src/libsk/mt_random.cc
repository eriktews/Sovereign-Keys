// Copyright 2012 the SK authors. All rights reserved.
//
// This implementation is adapted from [1] and [2], because we
// don't want to depend on boost and need 64-bit random values.
// [1] www.bedaux.net/mtrand/mtrand.h.html
// [2] www.boost.org/doc/libs/1_50_0/boost/random/mersenne_twister.hpp

#include "mt_random.h"

namespace sk {
namespace {
inline uint64_t Twiddle(uint64_t u, uint64_t v) {
  return (((u & 0xffffffff80000000) | (v & 0x7fffffff)) >> 1) ^
      ((v & 1) * 0xb5026f5aa96619e9);
}
}

MTRandom::MTRandom()
  : pos_(0) {
}

void MTRandom::Seed(uint64_t value) {
  state_[0] = value;
  for (pos_ = 1; pos_ < kMT19937_64StateSize; pos_++)
    state_[pos_] =
        (6364136223846793005 * (state_[pos_ - 1] ^ (state_[pos_ - 1] >> 62)) +
         pos_) & 0xffffffffffffffff;
}

uint64_t MTRandom::GetRandom() {
  if (pos_ == kMT19937_64StateSize)
    GenerateState();
  uint64_t value = state_[pos_++];
  value ^= ((value >> 29) & 0x5555555555555555);
  value ^= ((value << 17) & 0x71d67fffeda60000);
  value ^= ((value << 37) & 0xfff7eee000000000);
  value ^= (value >> 43);
  return value;
}

void MTRandom::GenerateState() {
  const int m = 156;
  const int n = kMT19937_64StateSize;
  for (int i = 0; i < n - m; i++)
    state_[i] = state_[i + m] ^ Twiddle(state_[i], state_[i + 1]);
  for (int i = n - m; i < n - 1; i++)
    state_[i] = state_[i + m - n] ^ Twiddle(state_[i], state_[i + 1]);
  state_[n - 1] = state_[m - 1] ^ Twiddle(state_[n - 1], state_[0]);
  pos_ = 0;
}

}  // namespace sk
