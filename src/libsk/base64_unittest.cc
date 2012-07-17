// Copyright 2012 the SK authors. All rights reserved.

#include "base64.h"

#include <assert.h>
#include <string>

#include "slice.h"
#include "mt_random.h"
#include "gtest/gtest.h"

using std::string;

namespace sk {
namespace {
string Encode(string data) {
  string result;
  EncodeBase64(data, &result);
  return result;
}

string Decode(string data) {
  string result;
  if (!DecodeBase64(data, &result)) {
    assert(0 && "Decode failed.");
  }
  return result;
}
}  // namespace

TEST(Base64Test, Encode) {
  // Test vectors from http://tools.ietf.org/html/rfc4648.
  EXPECT_EQ("", Encode(""));
  EXPECT_EQ("Zg==", Encode("f"));
  EXPECT_EQ("Zm8=", Encode("fo"));
  EXPECT_EQ("Zm9v", Encode("foo"));
  EXPECT_EQ("Zm9vYg==", Encode("foob"));
  EXPECT_EQ("Zm9vYmE=", Encode("fooba"));
  EXPECT_EQ("Zm9vYmFy", Encode("foobar"));
}

TEST(Base64Test, Decode) {
  // Test vectors from http://tools.ietf.org/html/rfc4648.
  EXPECT_EQ("", Decode(""));
  EXPECT_EQ("f", Decode("Zg=="));
  EXPECT_EQ("fo", Decode("Zm8="));
  EXPECT_EQ("foo", Decode("Zm9v"));
  EXPECT_EQ("foob", Decode("Zm9vYg=="));
  EXPECT_EQ("fooba", Decode("Zm9vYmE="));
  EXPECT_EQ("foobar", Decode("Zm9vYmFy"));
}

TEST(Base64Test, DecodeErrors) {
  string unused;

  // The empty string is a valid encoding.
  EXPECT_TRUE(DecodeBase64("", &unused));

  // An encoded string must be a multiple of 4 bytes long.
  EXPECT_FALSE(DecodeBase64("Zg", &unused));

  // Fail for invalid characters such as *.
  // Check each offset in the main decode loop and in the padding.
  EXPECT_FALSE(DecodeBase64("*m9vYg==", &unused));
  EXPECT_FALSE(DecodeBase64("Z*9vYg==", &unused));
  EXPECT_FALSE(DecodeBase64("Zm*vYg==", &unused));
  EXPECT_FALSE(DecodeBase64("Zm9*Yg==", &unused));
  EXPECT_FALSE(DecodeBase64("*g==", &unused));
  EXPECT_FALSE(DecodeBase64("Z*==", &unused));
  EXPECT_FALSE(DecodeBase64("*m8=", &unused));
  EXPECT_FALSE(DecodeBase64("Z*8=", &unused));
  EXPECT_FALSE(DecodeBase64("Zm*=", &unused));

  // Only the last two bytes may be padding.
  EXPECT_FALSE(DecodeBase64("=m9vYg==", &unused));
  EXPECT_FALSE(DecodeBase64("Z=9vYg==", &unused));
  EXPECT_FALSE(DecodeBase64("Zm=vYg==", &unused));
  EXPECT_FALSE(DecodeBase64("Zm9=Yg==", &unused));
  EXPECT_FALSE(DecodeBase64("=m9v", &unused));
  EXPECT_FALSE(DecodeBase64("Z=9v", &unused));
  EXPECT_FALSE(DecodeBase64("Zm=v", &unused));
  EXPECT_FALSE(DecodeBase64("Z==v", &unused));
  EXPECT_FALSE(DecodeBase64("===v", &unused));
  EXPECT_FALSE(DecodeBase64("Z===", &unused));
  EXPECT_FALSE(DecodeBase64("====", &unused));
}

TEST(Base64Test, Randoms) {
  const int kNumRandoms = 1000;
  const int kMaxBlockSize = 5000;
  MTRandom prng;
  prng.Seed(42);
  for (int i = 0; i < kNumRandoms; i++) {
    string data;
    const int length = prng.GetRandom() % kMaxBlockSize;
    for (int j = 0; j < length; j++)
      data.append(1, prng.GetRandom() & 255);
    EXPECT_EQ(data, Decode(Encode(data)));
  }
}
}  // namespace sk
