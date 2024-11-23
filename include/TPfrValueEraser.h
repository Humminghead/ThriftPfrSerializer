#pragma once

#include <type_traits>

namespace apache::thrift::serialize {

/**
 The ValueEraser class defines how to erase field's value
 */
template <typename T, typename Enable = void> struct ValueEraser {
#if GCPP_VERSION >= 130100
    static_assert(false, "Please specify Erase function!");
#endif
};

template <typename T>
struct ValueEraser<T, typename std::enable_if_t<std::is_default_constructible_v<T>>>
{
    static void Erase(T &value) { value = {}; }
};
} // namespace apache::thrift::serialize

