#pragma once

#include "TPfrKeyValueHandler.h"
#include "TPfrType.h"

#include <memory.h>
#include <string_view>

///\todo enum TType {
// T_STOP       = 0,
//    T_VOID       = 1,
//    T_BOOL       = 2,
//    T_BYTE       = 3,
//    T_I08        = 3,
//    T_I16        = 6,
//    T_I32        = 8,
//    T_U64        = 9,
//    T_I64        = 10,
//    T_DOUBLE     = 4,
//    T_STRING     = 11,
//    T_UTF7       = 11,
//    T_STRUCT     = 12,
//    T_MAP        = 13,
//    T_SET        = 14,
//    T_LIST       = 15,
//    T_UTF8       = 16,
//    T_UTF16      = 17
//};

namespace apache::thrift::serialization {

template <class Field, class Enable = void> struct TPfrFieldHandler;

/*!
 * \brief T_I32 specialization
 */
template <> struct TPfrFieldHandler<int32_t> {
  static void handle(const int &value, const std::string_view &name,
                     const size_t &index, uint32_t &written,
                     const std::shared_ptr<protocol::TProtocol> protocol) {
    written +=
        protocol->writeFieldBegin(name.data(), protocol::TType::T_I32, index);
    written += protocol->writeI32(value);
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
    written += protocol->writeFieldBegin(name.data(),
                                         protocol::TType::T_MAP, index);
    written += protocol->writeMapBegin(TPfrType<Key>::type,
                                       TPfrType<Value>::type, map.size());
    for (auto &pair : map) {
      KeyValueHandler<Key, Value>::handle(pair, protocol.get(), written);
    }
    written += protocol->writeMapEnd();
    written += protocol->writeFieldEnd();
  }
};

} // namespace apache::thrift::serialization
