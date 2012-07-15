// Copyright 2012 the SK authors. All rights reserved.

#include "base64.h"

#include <string>

#include "slice.h"

namespace sk {
namespace {
// Map from numbers to characters in the URL-safe base64 alphabet.
// Taken from http://tools.ietf.org/html/rfc4648.
static const char kNumToBase64[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

// A character code that is not a legal base64 digit.
static const uint8_t kBad = 128;

// Map from URL-safe base64 characters to the numbers they represent.
static const uint8_t kBase64ToNum[256] = {
  kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad,
  kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad,
  kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad,
  kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, 62, kBad, kBad,
  52, 53, 54, 55, 56, 57, 58, 59, 60, 61, kBad, kBad, kBad, kBad, kBad,
  kBad, kBad, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
  17, 18, 19, 20, 21, 22, 23, 24, 25, kBad, kBad, kBad, kBad, 63, kBad,
  26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
  44, 45, 46, 47, 48, 49, 50, 51, kBad, kBad, kBad, kBad, kBad, kBad,
  kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad,
  kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad,
  kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad,
  kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad,
  kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad,
  kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad,
  kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad,
  kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad,
  kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad,
  kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad, kBad,
  kBad, kBad, kBad, kBad, kBad, kBad, kBad,
};
}

void EncodeBase64(Slice in, std::string* out) {
  // There will be 4 output bytes for every 3 input bytes, rounding up
  // with padding.
  out->reserve(out->capacity() + 4 * ((in.length() + 2) / 3));
  while (in.length() >= 3) {
    // Encode groups of 3 bytes into 4 6-bit groups.
    out->append(1, kNumToBase64[(in[0] >> 2) & 0x3f]);
    out->append(1, kNumToBase64[((in[0] << 4) | (in[1] >> 4)) & 0x3f]);
    out->append(1, kNumToBase64[((in[1] << 2) | (in[2] >> 6)) & 0x3f]);
    out->append(1, kNumToBase64[in[2] & 0x3f]);
    in.Consume(3);
  }
  // Add padding characters if length is not a multiple of 6 bits.
  if (in.length() == 2) {
    out->append(1, kNumToBase64[(in[0] >> 2) & 0x3f]);
    out->append(1, kNumToBase64[((in[0] << 4) | (in[1] >> 4)) & 0x3f]);
    out->append(1, kNumToBase64[(in[1] << 2) & 0x3f]);
    out->append(1, '=');
  } else if (in.length() == 1) {
    out->append(1, kNumToBase64[(in[0] >> 2) & 0x3f]);
    out->append(1, kNumToBase64[(in[0] << 4) & 0x3f]);
    out->append(1, '=');
    out->append(1, '=');
  }
}

bool DecodeBase64(Slice in, std::string* out) {
  // There will be 3 bytes for every 4 input bytes, except padding.
  out->reserve(out->capacity() + 3 * (in.length() / 4));
  while (in.length() > 4) {
    // Decode the next four characters into 3 bytes.
    const uint8_t sextet0 = kBase64ToNum[in[0]];
    const uint8_t sextet1 = kBase64ToNum[in[1]];
    const uint8_t sextet2 = kBase64ToNum[in[2]];
    const uint8_t sextet3 = kBase64ToNum[in[3]];
    if ((sextet0 | sextet1 | sextet2 | sextet3) >= kBad)
      // One of the four characters is not in the base64 alphabet.
      return false;
    out->append(1, (sextet0 << 2) | (sextet1 >> 4));
    out->append(1, (sextet1 << 4) | (sextet2 >> 2));
    out->append(1, (sextet2 << 6) | sextet3);
    in.Consume(4);
  }
  if (in.length() == 0)
    // An empty input encodes the empty string.
    return true;
  if (in.length() != 4)
    // base64 encoded text must be a multiple of 4 characters long.
    return false;
  // Detect padding and decode one, two, or three remaining bytes.
  const uint8_t sextet0 = kBase64ToNum[in[0]];
  const uint8_t sextet1 = kBase64ToNum[in[1]];
  const uint8_t sextet2 = kBase64ToNum[in[2]];
  const uint8_t sextet3 = kBase64ToNum[in[3]];
  if ((sextet0 | sextet1) < kBad && in[2] == '=' && in[3] == '=') {
    out->append(1, (sextet0 << 2) | (sextet1 >> 4));
  } else if ((sextet0 | sextet1 | sextet2) < kBad && in[3] == '=') {
    out->append(1, (sextet0 << 2) | (sextet1 >> 4));
    out->append(1, (sextet1 << 4) | (sextet2 >> 2));
  } else if ((sextet0 | sextet1 | sextet2 | sextet3) < kBad) {
    out->append(1, (sextet0 << 2) | (sextet1 >> 4));
    out->append(1, (sextet1 << 4) | (sextet2 >> 2));
    out->append(1, (sextet2 << 6) | sextet3);
  } else {
    return false;
  }
  return true;
}

} // namespace sk
