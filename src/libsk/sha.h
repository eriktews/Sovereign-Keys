// Copyright 2012 the SK authors. All rights reserved.

#ifndef LIBSK_SHA_H_
#define LIBSK_SHA_H_

#include <string>

namespace sk {
class Slice;

// Returns the SHA-256 hash of |in|.
std::string ComputeSHA256(Slice in);

}  // namespace sk

#endif  // LIBSK_SHA_H_
