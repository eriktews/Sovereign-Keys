// Copyright 2012 the SK authors. All rights reserved.

#ifndef LIBSK_VARINT_H_
#define LIBSK_VARINT_H_

#include <stddef.h>  // for size_t
#include <stdint.h>
#include <string>

namespace sk {

class Slice;

// Encodes |in| using URL-safe base64 and appends the result to |out|.
void EncodeBase64(Slice in, std::string* out);

// Decodes URL-safe base64 from |in| and appends the result to |out|.
// Returns true iff the entire input was decoded successfully.
bool DecodeBase64(Slice in, std::string* out);

}  // namespace sk

#endif  // LIBSK_VARINT_H_
