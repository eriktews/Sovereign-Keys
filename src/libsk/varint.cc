// Copyright 2012 the SK authors. All rights reserved.

#include "varint.h"

#include <stdint.h>
#include <string>

#include "slice.h"

namespace sk {

bool DecodeVarint(Slice in, uint64_t* out) {
  *out = 0;
  const int kMaxVarintBytes = (64 + 6) / 7;
  int num_bytes_decoded = 0;
  while (in.length() > 0 && num_bytes_decoded < kMaxVarintBytes) {
    const uint8_t data = in.ConsumeFirst();
    *out |= static_cast<uint64_t>(data & 0x7f) << (7 * num_bytes_decoded);
    num_bytes_decoded++;
    if (!(data & 0x80)) {
      // Found the end of the varint.
      return true;
    }
  }
  // The varint was truncated or too long.
  return false;
}

void EncodeVarint(uint64_t in, std::string* out) {
  do {
    out->append(1, (0x80 * (in > 0x7f)) | (in & 0x7f));
    in >>= 7;
  } while (in > 0);
}

}  // namespace sk
