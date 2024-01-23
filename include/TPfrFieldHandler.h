#pragma once

#include "TPfrKeyValueHandler.h"
#include "TPfrType.h"

#include <memory.h>
#include <string_view>

///\todo enum TType {
// T_STOP       = 0,
//    T_UTF7       = 11,
//    T_STRUCT     = 12,
//    T_SET        = 14,
//    T_UTF8       = 16,
//    T_UTF16      = 17
//};

namespace apache::thrift::serialize {

template <class Field, class Enable = void> struct TPfrFieldHandler;

/*!
 * \brief T_BOOL specialization
 */
template <> struct TPfrFieldHandler<bool> {
  static void handle(const bool value, const std::string_view &name,
                     const size_t &index, uint32_t &written,
                     const std::shared_ptr<protocol::TProtocol> protocol) {
    written +=
        protocol->writeFieldBegin(name.data(), protocol::TType::T_BOOL, index);
    written += protocol->writeBool(value);
    written += protocol->writeFieldEnd();
  }
};

/*!
 * \brief T_BYTE specialization
 */
template <class Field>
struct TPfrFieldHandler<
    Field, typename std::enable_if_t<std::is_same_v<char, Field> ||
                                     std::is_same_v<signed char, Field> ||
                                     std::is_same_v<unsigned char, Field>>> {
  static void handle(const Field &value, const std::string_view &name,
                     const size_t &index, uint32_t &written,
                     const std::shared_ptr<protocol::TProtocol> protocol) {
    written +=
        protocol->writeFieldBegin(name.data(), protocol::TType::T_BYTE, index);
    written += protocol->writeByte(static_cast<signed char>(value));
    written += protocol->writeFieldEnd();
  }
};

/*!
 * \brief T_I16 specialization
 */
template <class Field>
struct TPfrFieldHandler<Field, typename std::enable_if_t<
                                   std::is_same_v<signed short int, Field> ||
                                   std::is_same_v<unsigned short int, Field>>> {
  static void handle(const Field &value, const std::string_view &name,
                     const size_t &index, uint32_t &written,
                     const std::shared_ptr<protocol::TProtocol> protocol) {
    written +=
        protocol->writeFieldBegin(name.data(), protocol::TType::T_I16, index);
    written += protocol->writeI16(static_cast<signed short int>(value));
    written += protocol->writeFieldEnd();
  }
};

/*!
 * \brief T_I32 specialization
 */
template <class Field>
struct TPfrFieldHandler<
    Field, typename std::enable_if_t<std::is_same_v<signed int, Field> ||
                                     std::is_same_v<unsigned int, Field>>> {
  static void handle(const Field &value, const std::string_view &name,
                     const size_t &index, uint32_t &written,
                     const std::shared_ptr<protocol::TProtocol> protocol) {
    written +=
        protocol->writeFieldBegin(name.data(), protocol::TType::T_I32, index);
    written += protocol->writeI32(static_cast<signed int>(value));
    written += protocol->writeFieldEnd();
  }
};

/*!
 * \brief T_I64 specialization
 */
template <class Field>
struct TPfrFieldHandler<Field, typename std::enable_if_t<
                                   std::is_same_v<signed long int, Field> ||
                                   std::is_same_v<unsigned long int, Field>>> {
  static void handle(const Field &value, const std::string_view &name,
                     const size_t &index, uint32_t &written,
                     const std::shared_ptr<protocol::TProtocol> protocol) {
    written +=
        protocol->writeFieldBegin(name.data(), protocol::TType::T_I64, index);
    written += protocol->writeI32(static_cast<signed long int>(value));
    written += protocol->writeFieldEnd();
  }
};

/*!
 * \brief T_DOUBLE specialization
 */
template <class Field>
struct TPfrFieldHandler<
    Field, typename std::enable_if_t<std::is_floating_point_v<Field>>> {
  static void handle(const Field &value, const std::string_view &name,
                     const size_t &index, uint32_t &written,
                     const std::shared_ptr<protocol::TProtocol> protocol) {
    written += protocol->writeFieldBegin(name.data(), protocol::TType::T_DOUBLE,
                                         index);
    written += protocol->writeDouble(static_cast<double>(value));
    written += protocol->writeFieldEnd();
  }
};

/*!
 * \brief T_MAP specialization
 */
template <class Key, class Value>
struct TPfrFieldHandler<std::map<Key, Value>> {
  static void handle(const std::map<Key, Value> &map,
                     const std::string_view &name, const size_t &index,
                     uint32_t &written,
                     const std::shared_ptr<protocol::TProtocol> protocol) {
    written +=
        protocol->writeFieldBegin(name.data(), protocol::TType::T_MAP, index);
    written += protocol->writeMapBegin(TPfrType<Key>::type,
                                       TPfrType<Value>::type, map.size());
    for (auto &pair : map) {
      KeyValueHandler<Key, Value>::handle(pair, protocol.get(), written);
    }
    written += protocol->writeMapEnd();
    written += protocol->writeFieldEnd();
  }
};

/*!
 * \brief T_STRING specialization
 */
template <> struct TPfrFieldHandler<std::string> {
  static void handle(const std::string &s, const std::string_view &name,
                     const size_t &index, uint32_t &written,
                     const std::shared_ptr<protocol::TProtocol> protocol) {
    written += protocol->writeFieldBegin(name.data(), protocol::TType::T_STRING,
                                         index);
    written += protocol->writeString(s);
    written += protocol->writeFieldEnd();
  }
};

/*!
 * \brief T_STRING specialization for string_view
 */
template <> struct TPfrFieldHandler<std::string_view> {
  static void handle(const std::string_view &s, const std::string_view &name,
                     const size_t &index, uint32_t &written,
                     const std::shared_ptr<protocol::TProtocol> protocol) {
    written += protocol->writeFieldBegin(name.data(), protocol::TType::T_STRING,
                                         index);
    written +=
        protocol->writeString(s.data()); // only writeString(const std::string&
                                         // str) exists in TProtocol.h
    written += protocol->writeFieldEnd();
  }
};

/*!
 * \brief T_LIST specialization
 */
template <class Value> struct TPfrFieldHandler<std::list<Value>> {
  static void handle(const std::list<Value> &lst, const std::string_view &name,
                     const size_t &index, uint32_t &written,
                     const std::shared_ptr<protocol::TProtocol> protocol) {
    written +=
        protocol->writeFieldBegin(name.data(), protocol::TType::T_LIST, index);
    written += protocol->writeListBegin(TPfrType<Value>::type, lst.size());
    for (auto &el : lst) {
      ListValueHandler<Value>::handle(el, protocol.get(), written);
    }
    written += protocol->writeListEnd();
    written += protocol->writeFieldEnd();
  }
};

} // namespace apache::thrift::serialize
