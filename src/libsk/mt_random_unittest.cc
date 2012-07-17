// Copyright 2012 the SK authors. All rights reserved.

#include "mt_random.h"

#include <stdint.h>
#include <vector>

#include "gtest/gtest.h"
#include "util.h"

namespace sk {

// Generated using an independent Go implementation.
const uint64_t kTestVector[] = {
    0xc151df7d6ee5e2d6, 0xa3978fb9b92502a8, 0xc08c967f0e5e7b0a,
    0x22e2c43f8a1ad34e, 0xe73ca28e4d361955, 0x1814dc629c7f4f7c,
    0x93170a1965d42420, 0x5f75917a3eb7b900, 0x461c9cf62eb9fcb6,
    0x63e8cae041677d61, 0x32b846d0bbd3f4b, 0x861191c96090b446,
    0xaf6df0655c6891d8, 0xa32897ae188a782e, 0xd398c3c9b02b56cd,
    0xf2194bc7d5976b28, 0xc0d2eda553d1bc36, 0x72ec29f7fc409872,
    0xbfb48552d60ec21, 0x10894433f9475527, 0xbf62e22b9623fb4b,
    0x2639bc290a9eeb52, 0x6cdd812c68f90519, 0x199cad8504e80ac9,
    0x24d78d1d1b2520c5, 0x181baef38522f794, 0x87b8a689f6b57129,
    0x7239f67bad2506ec, 0xc5e48fc8e6f31d96, 0x1f5d587b2a99fb54,
    0xbec32aaf5dd5e327, 0x52e1d21451942857
};

TEST(MTRandomTest, GetRandom) {
  MTRandom prng;
  prng.Seed(42);
  for (int i = 0; i < arraysize(kTestVector); i++)
    EXPECT_EQ(kTestVector[i], prng.GetRandom());
}

}  // namespace sk
