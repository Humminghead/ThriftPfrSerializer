#pragma once

namespace apache::thrift::serialize {
///\todo
///    T_SET        = 14,
///    T_LIST       = 15,
///    T_UTF8       = 16,
///    T_UTF16      = 17

template <typename Type> struct TPfrType;

template <> struct TPfrType<bool> {
  constexpr static protocol::TType type = protocol::TType::T_BOOL;
};

template <> struct TPfrType<char> {
  constexpr static protocol::TType type = protocol::TType::T_BYTE;
};

template <> struct TPfrType<int8_t> {
  constexpr static protocol::TType type = protocol::TType::T_I08;
};

template <> struct TPfrType<int16_t> {
  constexpr static protocol::TType type = protocol::TType::T_I16;
};

template <> struct TPfrType<int32_t> {
  constexpr static protocol::TType type = protocol::TType::T_I32;
};

template <> struct TPfrType<int64_t> {
  constexpr static protocol::TType type = protocol::TType::T_I64;
};

template <> struct TPfrType<float> {
  constexpr static protocol::TType type = protocol::TType::T_DOUBLE;
};

template <> struct TPfrType<double> {
  constexpr static protocol::TType type = protocol::TType::T_DOUBLE;
};

template <> struct TPfrType<std::string> {
  constexpr static protocol::TType type = protocol::TType::T_STRING;
};

template <> struct TPfrType<std::string_view> {
  constexpr static protocol::TType type = protocol::TType::T_STRING;
};

template <typename Key, typename Value> struct TPfrType<std::map<Key, Value>> {
  constexpr static protocol::TType type = protocol::TType::T_MAP;
};
} // namespace apache::thrift::serialization
