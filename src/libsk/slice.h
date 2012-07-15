// Copyright 2012 the SK authors. All rights reserved.

#ifndef LIBSK_SLICE_H_
#define LIBSK_SLICE_H_

#include <stddef.h>
#include <stdint.h>
#include <string.h>  // for memcmp
#include <string>

#include "util.h"

namespace sk {

// A |Slice| is a range of bytes in memory backed by a non-owned buffer.
// Users of this class must ensure the underlying buffer is valid.
class Slice {
  public:
    Slice(const Slice& slice)
      : data_(slice.data()), length_(slice.length()) { }
    // Allow implicit conversion from string for convenience.
    Slice(const std::string& str)
      : data_(reinterpret_cast<const uint8_t*>(str.data())),
        length_(str.size()) { }
    // This is provided for unit tests only.
    // TODO(jered): Add an #if test guard here.
    Slice(const char* data)
      : data_(reinterpret_cast<const uint8_t*>(data)),
        length_(strlen(data)) { }
    Slice(const uint8_t* data, size_t length)
      : data_(data), length_(length) { }

    const uint8_t *data() const { return data_; }
    size_t length() const { return length_; }
    uint8_t operator[](size_t i) const { return data_[i]; }

    // Consumes |num| bytes from the beginning of the slice.
    void Consume(size_t num) {
      data_ += num;
      length_ -= num;
    }

  private:
    Slice() { };

    const uint8_t* data_;
    size_t length_;
};

inline bool operator==(const Slice& x, const Slice& y) {
  return x.length() == y.length() &&
         memcmp(x.data(), y.data(), x.length()) == 0;
}

}  // namespace sk

#endif  // LIBSK_SLICE_H_
