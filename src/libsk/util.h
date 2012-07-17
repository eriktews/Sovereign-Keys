// Copyright 2012 the SK authors. All rights reserved.

#ifndef LIBSK_UTIL_H_
#define LIBSK_UTIL_H_

#include <algorithm>

namespace sk {

class Slice;
typedef std::pair<Slice, Slice> KeyValue;

// Returns the size of its argument array.
template<class Type, ptrdiff_t n>
ptrdiff_t arraysize(Type (&)[n]) {
  return n;
}

// DISALLOW_EVIL_CONSTRUCTORS disallows the copy and operator= functions.
// It goes in the private: declarations in a class.
#define DISALLOW_EVIL_CONSTRUCTORS(TypeName) \
  TypeName(const TypeName&);                 \
  void operator=(const TypeName&)

}  // namespace sk

#endif  // LIBSK_UTIL_H_
