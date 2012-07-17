// Copyright 2012 the SK authors. All rights reserved.

#ifndef LIBSK_VARINT_H_
#define LIBSK_VARINT_H_

#include <stdint.h>
#include <string>

namespace sk {

class Slice;

// Decodes a varint of up to 64 bits from |in| and stores it to |out|.
// Returns true iff a valid varint was found.
bool DecodeVarint(Slice in, uint64_t* out);

// Encodes |in| as a varint and stores it to |out|.
void EncodeVarint(uint64_t in, std::string* out);

}  // namespace sk

#endif  // LIBSK_VARINT_H_
