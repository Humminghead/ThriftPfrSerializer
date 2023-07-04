#pragma once

#include <thrift/protocol/TProtocol.h>
#include <utility>

namespace apache::thrift::serialization {

template <typename T>
uint32_t write(const T &value, protocol::TProtocol *protocol);

template <>
uint32_t write<bool>(const bool &value, protocol::TProtocol *protocol) {
  return protocol->writeBool(value);
}

///\todo unsigned char
template <>
uint32_t write<char>(const char &c, protocol::TProtocol *protocol) {
  return protocol->writeByte(c);
}

///\todo unsigned type
template <>
uint32_t write<int8_t>(const int8_t &byte, protocol::TProtocol *protocol) {
  return protocol->writeByte(byte);
}

///\todo unsigned type
template <>
uint32_t write<int16_t>(const int16_t &i16, protocol::TProtocol *protocol) {
  return protocol->writeI16(i16);
}

///\todo unsigned type
template <>
uint32_t write<int32_t>(const int32_t &i32, protocol::TProtocol *protocol) {
  return protocol->writeI32(i32);
}

///\todo unsigned type
template <>
uint32_t write<int64_t>(const int64_t &i64, protocol::TProtocol *protocol) {
  return protocol->writeI64(i64);
}

///\todo unsigned type
template <>
uint32_t write<double>(const double &dub, protocol::TProtocol *protocol) {
  return protocol->writeDouble(dub);
}

template <>
uint32_t write<std::string>(const std::string &str, protocol::TProtocol *protocol) {
  return protocol->writeString(str);
}

///\todo std::string_view
// template<>
// uint32_t writeBinary(const std::string& str,protocol::TProtocol *protocol) {
//     return protocol->writeBinary (str);
//}

template <class Key, class Value> struct KeyValueHandler;

template <class Key, class Value> struct KeyValueHandler {
  static void handle(const std::pair<Key, Value> &kv,
                     protocol::TProtocol *protocol, uint32_t &written) {
    written += write<Key>(kv.first, protocol);
    written += write<Value>(kv.second, protocol);
  }
};

} // namespace apache::thrift::serialization
