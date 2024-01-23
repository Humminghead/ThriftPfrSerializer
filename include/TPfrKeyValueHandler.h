#pragma once

#include <thrift/protocol/TProtocol.h>
#include <utility>

namespace apache::thrift::serialize {

template <typename T>
uint32_t write(const T &value, protocol::TProtocol *protocol);

template <>
uint32_t write<bool>(const bool &value, protocol::TProtocol *protocol) {
  return protocol->writeBool(value);
}

template <> uint32_t write<char>(const char &c, protocol::TProtocol *protocol) {
  return protocol->writeByte(c);
}

template <>
uint32_t write<signed char>(const signed char &byte, protocol::TProtocol *protocol) {
  return protocol->writeByte(byte);
}

template <>
uint32_t write<unsigned char>(const unsigned char &byte, protocol::TProtocol *protocol) {
    return protocol->writeByte(byte);
}

template <>
uint32_t write<signed short int>(const signed short int &i16, protocol::TProtocol *protocol) {
  return protocol->writeI16(i16);
}

template <>
uint32_t write<unsigned short int>(const unsigned short int &i16, protocol::TProtocol *protocol) {
    return protocol->writeI16(i16);
}

template <>
uint32_t write<signed int>(const signed int &i32, protocol::TProtocol *protocol) {
  return protocol->writeI32(i32);
}

template <>
uint32_t write<unsigned int>(const unsigned int &i32, protocol::TProtocol *protocol) {
    return protocol->writeI32(i32);
}

template <>
uint32_t write<signed long int>(const signed long int &i64, protocol::TProtocol *protocol) {
  return protocol->writeI64(i64);
}

template <>
uint32_t write<unsigned long int>(const unsigned long int &i64, protocol::TProtocol *protocol) {
    return protocol->writeI64(static_cast<unsigned long int>(i64));
}

template <>
uint32_t write<float>(const float &flt, protocol::TProtocol *protocol) {
    return protocol->writeDouble(static_cast<double>(flt));
}

template <>
uint32_t write<double>(const double &dub, protocol::TProtocol *protocol) {
  return protocol->writeDouble(dub);
}

template <>
uint32_t write<std::string>(const std::string &str,
                            protocol::TProtocol *protocol) {
  return protocol->writeString(str);
}

template <>
uint32_t write(const std::string_view &str, protocol::TProtocol *protocol) {
  return protocol->writeString(str.data());
}

template <class Key, class Value> struct KeyValueHandler;
template <class Key, class Value> struct KeyValueHandler {
  static void handle(const std::pair<Key, Value> &kv,
                     protocol::TProtocol *protocol, uint32_t &written) {
    written += write<Key>(kv.first, protocol);
    written += write<Value>(kv.second, protocol);
  }
};

template <class Value> struct ListValueHandler;
template <class Value> struct ListValueHandler {
  static void handle(const Value &v, protocol::TProtocol *protocol,
                     uint32_t &written) {
    written += write<Value>(v, protocol);
  }
};

} // namespace apache::thrift::serialize
