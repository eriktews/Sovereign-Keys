// Copyright 2012 the SK authors. All rights reserved.

#include "sha.h"

#include <string>
#include <openssl/sha.h>
#include "slice.h"

namespace sk {

std::string ComputeSHA256(Slice in) {
  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, in.data(), in.length());
  std::string hash;
  hash.resize(SHA256_DIGEST_LENGTH);
  // string bytes up to size() are contiguous.
  // stackoverflow.com/questions/1986966/
  //   does-s0-point-to-contiguous-characters-in-a-stdstring
  SHA256_Final(reinterpret_cast<unsigned char*>(&hash[0]), &sha256);
  return hash;
}

}  // namespace sk
