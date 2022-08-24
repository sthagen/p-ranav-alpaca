#pragma once
#include <cstring>
#include <iterator>
#include <alpaca/detail/type_traits.h>
#include <alpaca/detail/variable_length_encoding.h>

namespace alpaca {

namespace detail {

/// Forward declare
template <typename T>
void to_bytes_router(const T &input, std::vector<uint8_t> &bytes);

template <typename T, typename U> void append(const T &value, U &bytes) {
  std::copy(static_cast<const char *>(static_cast<const void *>(&value)),
            static_cast<const char *>(static_cast<const void *>(&value)) +
                sizeof value,
            std::back_inserter(bytes));
}

// Integral types
template <typename T>
typename std::enable_if<std::is_integral_v<T>, void>::type
to_bytes(const T &input, std::vector<uint8_t> &bytes) {

  // for smaller ints, save as is
  // don't perform variable-length encoding
  if constexpr (std::is_same_v<T, uint8_t> || std::is_same_v<T, uint16_t> ||
                std::is_same_v<T, int8_t> || std::is_same_v<T, int16_t>) {
    append(input, bytes);
  } else {
    encode_varint<T>(input, bytes);
  }
}

// Specializations for particular numeric types

static inline void to_bytes(const bool &input, std::vector<uint8_t> &bytes) {
  append(input, bytes);
}

static inline void to_bytes(const char &input, std::vector<uint8_t> &bytes) {
  to_bytes(static_cast<uint8_t>(input), bytes);
}

static inline void to_bytes(const float &input, std::vector<uint8_t> &bytes) {
  append(input, bytes);
}

static inline void to_bytes(const double &input, std::vector<uint8_t> &bytes) {
  append(input, bytes);
}

// Specialization for string

static inline void to_bytes(const std::string &input,
                            std::vector<uint8_t> &bytes) {
  // save string length
  to_bytes(input.size(), bytes);

  for (auto &c : input) {
    append(c, bytes);
  }
}

} // namespace detail

} // namespace alpaca