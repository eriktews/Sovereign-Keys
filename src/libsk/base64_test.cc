// Copyright 2012 the SK authors. All rights reserved.

#include "base64.h"

#include <string>

#include "slice.h"
#include "gtest/gtest.h"

using std::string;

namespace sk {
namespace {
// Guaranteed to be random, chosen by fair dice roll. ;-)
const char kTestData[] = {
  0x28, 0x33, 0x89, 0xb4, 0xa7, 0xbc, 0x6a, 0xf0, 0xd5, 0x63, 0x0c, 0x68,
  0x58, 0xca, 0xb3, 0x6d, 0xdc, 0x5d, 0x32, 0x1b, 0xad, 0x1b, 0x33, 0xaf,
  0x2c, 0x0c, 0x56, 0x89, 0xf5, 0x70, 0x34, 0xf6, 0x39, 0x95, 0xd2, 0x02,
  0x22, 0x08, 0x93, 0xb0, 0xea, 0x9f, 0x53, 0x80, 0x4c, 0xf7, 0x93, 0xea,
  0x79, 0xb2
};

// Encoded using the GNU base64 program.
const char kTestDataBase64[] =
  "KDOJtKe8avDVYwxoWMqzbdxdMhutGzOvLAxWifVwNPY5ldICIgiTsOqfU4BM95PqebI=";

string Encode(string data) {
  string result;
  EncodeBase64(data, &result);
  return result;
}

string Decode(string data) {
  string result;
  DecodeBase64(data, &result);
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

  // Test some random data.
  EXPECT_EQ(kTestDataBase64, Encode(kTestData));
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

  // Test some random data.
  EXPECT_EQ(kTestData, Decode(kTestDataBase64));
}

TEST(Base64Test, DecodeErrors) {
  string dummy;

  // The empty string is a valid encoding.
  EXPECT_TRUE(DecodeBase64("", &dummy));

  // An encoded string must be a multiple of 4 bytes long.
  EXPECT_FALSE(DecodeBase64("Zg", &dummy));

  // Fail for invalid characters such as *.
  // Check each offset in the main decode loop and in the padding.
  EXPECT_FALSE(DecodeBase64("*m9vYg==", &dummy));
  EXPECT_FALSE(DecodeBase64("Z*9vYg==", &dummy));
  EXPECT_FALSE(DecodeBase64("Zm*vYg==", &dummy));
  EXPECT_FALSE(DecodeBase64("Zm9*Yg==", &dummy));
  EXPECT_FALSE(DecodeBase64("*g==", &dummy));
  EXPECT_FALSE(DecodeBase64("Z*==", &dummy));
  EXPECT_FALSE(DecodeBase64("*m8=", &dummy));
  EXPECT_FALSE(DecodeBase64("Z*8=", &dummy));
  EXPECT_FALSE(DecodeBase64("Zm*=", &dummy));

  // Only the last two bytes may be padding.
  EXPECT_FALSE(DecodeBase64("=m9vYg==", &dummy));
  EXPECT_FALSE(DecodeBase64("Z=9vYg==", &dummy));
  EXPECT_FALSE(DecodeBase64("Zm=vYg==", &dummy));
  EXPECT_FALSE(DecodeBase64("Zm9=Yg==", &dummy));
  EXPECT_FALSE(DecodeBase64("=m9v", &dummy));
  EXPECT_FALSE(DecodeBase64("Z=9v", &dummy));
  EXPECT_FALSE(DecodeBase64("Zm=v", &dummy));
  EXPECT_FALSE(DecodeBase64("Z==v", &dummy));
  EXPECT_FALSE(DecodeBase64("===v", &dummy));
  EXPECT_FALSE(DecodeBase64("Z===", &dummy));
  EXPECT_FALSE(DecodeBase64("====", &dummy));
}
}  // namespace sk
