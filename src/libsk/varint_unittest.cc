// Copyright 2012 the SK authors. All rights reserved.

#include "varint.h"

#include <string>

#include "gtest/gtest.h"
#include "mt_random.h"
#include "slice.h"

using std::string;

namespace sk {
namespace {
string Encode(uint64_t value) {
  string result;
  EncodeVarint(value, &result);
  return result;
}

uint64_t Decode(string value) {
  uint64_t result;
  if (!DecodeVarint(value, &result)) {
    assert(0 && "Decode failed.");
  }
  return result;
}
}

TEST(VarintTest, Encode) {
  EXPECT_EQ(string(1, '\0'), Encode(0));
  EXPECT_EQ("\x01", Encode(0x1ULL));
  EXPECT_EQ("\x80\x01", Encode(0x80ULL));
  EXPECT_EQ("\x80\x80\x01", Encode(0x4000ULL));
  EXPECT_EQ("\x80\x80\x80\x01", Encode(0x200000ULL));
  EXPECT_EQ("\x80\x80\x80\x80\x01", Encode(0x10000000ULL));
  EXPECT_EQ("\x80\x80\x80\x80\x80\x01", Encode(0x800000000ULL));
  EXPECT_EQ("\x80\x80\x80\x80\x80\x80\x01", Encode(0x40000000000ULL));
  EXPECT_EQ("\x80\x80\x80\x80\x80\x80\x80\x01", Encode(0x2000000000000ULL));
  EXPECT_EQ("\x80\x80\x80\x80\x80\x80\x80\x80\x01",
            Encode(0x100000000000000ULL));
  EXPECT_EQ("\x80\x80\x80\x80\x80\x80\x80\x80\x80\x01",
            Encode(0x8000000000000000ULL));
}

TEST(VarintTest, Decode) {
  EXPECT_EQ(0ULL, Decode(string(1, '\0')));
  EXPECT_EQ(0x1ULL, Decode("\x01"));
  EXPECT_EQ(0x80ULL, Decode("\x80\x01"));
  EXPECT_EQ(0x4000ULL, Decode("\x80\x80\x01"));
  EXPECT_EQ(0x200000ULL, Decode("\x80\x80\x80\x01"));
  EXPECT_EQ(0x10000000ULL, Decode("\x80\x80\x80\x80\x01"));
  EXPECT_EQ(0x800000000ULL, Decode("\x80\x80\x80\x80\x80\x01"));
  EXPECT_EQ(0x40000000000ULL, Decode("\x80\x80\x80\x80\x80\x80\x01"));
  EXPECT_EQ(0x2000000000000ULL, Decode("\x80\x80\x80\x80\x80\x80\x80\x01"));
  EXPECT_EQ(0x100000000000000ULL,
            Decode("\x80\x80\x80\x80\x80\x80\x80\x80\x01"));
  EXPECT_EQ(0x8000000000000000ULL,
            Decode("\x80\x80\x80\x80\x80\x80\x80\x80\x80\x01"));
}

TEST(VarintTest, Randoms) {
  const int kNumRandoms = 100000;
  MTRandom prng;
  for (int i = 0; i < kNumRandoms; i++) {
    uint64_t value = prng.GetRandom();
    EXPECT_EQ(value, Decode(Encode(value)));
  }
}

TEST(VarintTest, DecodeErrors) {
  uint64_t unused;

  // The empty string is not a valid encoding.
  EXPECT_FALSE(DecodeVarint("", &unused));

  // Fail for truncated varints.
  EXPECT_FALSE(DecodeVarint(string(1, 0x80), &unused));
  EXPECT_FALSE(DecodeVarint(string(2, 0x80), &unused));
  EXPECT_FALSE(DecodeVarint(string(3, 0x80), &unused));
  EXPECT_FALSE(DecodeVarint(string(4, 0x80), &unused));
  EXPECT_FALSE(DecodeVarint(string(5, 0x80), &unused));
  EXPECT_FALSE(DecodeVarint(string(6, 0x80), &unused));
  EXPECT_FALSE(DecodeVarint(string(7, 0x80), &unused));
  EXPECT_FALSE(DecodeVarint(string(8, 0x80), &unused));
  EXPECT_FALSE(DecodeVarint(string(9, 0x80), &unused));
  EXPECT_FALSE(DecodeVarint(string(10, 0x80), &unused));

  // Fail for varints that are too long to represent in 64 bits.
  EXPECT_FALSE(DecodeVarint("\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x01",
                            &unused));
}
}
