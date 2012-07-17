#include "sha.h"

#include <string>
#include <string.h>

#include "gtest/gtest.h"
#include "slice.h"

using sk::ComputeSHA256;
using sk::Slice;
using std::string;

namespace {
int Hex(char c) {
  if (c >= '0' && c <= '9')
    return c - '0';
  if (c >= 'a' && c <= 'f')
    return 10 + c - 'a';
  if (c >= 'A' && c <= 'F')
    return 10 + c - 'A';
  return 0;
}

string Unhex(Slice in) {
  string out;
  for (size_t i = 0; i < in.length(); i += 2)
    out.append(1, 16 * Hex(in[i]) + Hex(in[i + 1]));
  return out;
}
}

TEST(SHATest, SHA256) {
  // Generated using a non-OpenSSL SHA-256 implementation.
  EXPECT_EQ(Unhex("cc6004bb7ab40e52a92928146ffdb32f"
                  "10d48c599d5fbf9a7eaa4e3623ec7d69"),
            ComputeSHA256("testing 123"));
}
