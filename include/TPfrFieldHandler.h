#pragma once

#include "TPfrKeyValueHandler.h"
#include "TPfrType.h"

#include <memory.h>
#include <string_view>

/**
 * The collection of specializations for the most common types
 **/

///\todo Some types were unpesialized:
/// T_STOP       = 0,
/// T_UTF7       = 11,
/// T_STRUCT     = 12,
/// T_UTF8       = 16,
/// T_UTF16      = 17

namespace apache::thrift::serialize {

/**
 * \brief Specialization for any type
 */
template <class Field, class Enable = void> struct TPfrFieldHandler {
  static_assert(
      false,
      "Unsupported type! Please specify TPfrFieldHandler for your type!");
};

/*!
 * \brief T_BOOL specialization
 */
template <> struct TPfrFieldHandler<bool> {
  static void handleWrite(const bool value, const std::string_view &name,
                          const size_t &index, uint32_t &written,
                          const std::shared_ptr<protocol::TProtocol> protocol) {
    written +=
        protocol->writeFieldBegin(name.data(), protocol::TType::T_BOOL, index);
    written += protocol->writeBool(value);
    written += protocol->writeFieldEnd();
  }

  static void handleRead(bool &value, uint32_t &read,
                         const std::shared_ptr<protocol::TProtocol> protocol) {
    read += protocol->readBool(value);
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
  static void handleWrite(const Field &value, const std::string_view &name,
                          const size_t &index, uint32_t &written,
                          const std::shared_ptr<protocol::TProtocol> protocol) {
    written +=
        protocol->writeFieldBegin(name.data(), protocol::TType::T_BYTE, index);
    written += protocol->writeByte(static_cast<signed char>(value));
    written += protocol->writeFieldEnd();
  }

  static void handleRead(Field &value, uint32_t &read,
                         const std::shared_ptr<protocol::TProtocol> protocol) {
    signed char &tLink = reinterpret_cast<signed char &>(value);
    read += protocol->readByte(tLink);
  }
};

/*!
 * \brief T_I16 specialization
 */
template <class Field>
struct TPfrFieldHandler<Field, typename std::enable_if_t<
                                   std::is_same_v<signed short int, Field> ||
                                   std::is_same_v<unsigned short int, Field>>> {
  static void handleWrite(const Field &value, const std::string_view &name,
                          const size_t &index, uint32_t &written,
                          const std::shared_ptr<protocol::TProtocol> protocol) {
    written +=
        protocol->writeFieldBegin(name.data(), protocol::TType::T_I16, index);
    written += protocol->writeI16(static_cast<signed short int>(value));
    written += protocol->writeFieldEnd();
  }

  static void handleRead(Field &value, uint32_t &read,
                         const std::shared_ptr<protocol::TProtocol> protocol) {
    read += protocol->readI16(value);
  }
};

/*!
 * \brief T_I32 specialization
 */
template <class Field>
struct TPfrFieldHandler<
    Field, typename std::enable_if_t<std::is_same_v<signed int, Field> ||
                                     std::is_same_v<unsigned int, Field>>> {
  static void handleWrite(const Field &value, const std::string_view &name,
                          const size_t &index, uint32_t &written,
                          const std::shared_ptr<protocol::TProtocol> protocol) {
    written +=
        protocol->writeFieldBegin(name.data(), protocol::TType::T_I32, index);
    written += protocol->writeI32(static_cast<signed int>(value));
    written += protocol->writeFieldEnd();
  }

  static void handleRead(Field &value, uint32_t &read,
                         const std::shared_ptr<protocol::TProtocol> protocol) {
    signed int &tLink = reinterpret_cast<signed int &>(value);
    read += protocol->readI32(tLink);
  }
};

/*!
 * \brief T_I64 specialization
 */
template <class Field>
struct TPfrFieldHandler<Field, typename std::enable_if_t<
                                   std::is_same_v<signed long int, Field> ||
                                   std::is_same_v<unsigned long int, Field>>> {
  static void handleWrite(const Field &value, const std::string_view &name,
                          const size_t &index, uint32_t &written,
                          const std::shared_ptr<protocol::TProtocol> protocol) {
    written +=
        protocol->writeFieldBegin(name.data(), protocol::TType::T_I64, index);
    written += protocol->writeI64(static_cast<signed long int>(value));
    written += protocol->writeFieldEnd();
  }

  static void handleRead(Field &value, uint32_t &read,
                         const std::shared_ptr<protocol::TProtocol> protocol) {
    signed long int &tLink = reinterpret_cast<signed long int &>(value);
    read += protocol->readI64(tLink);
  }
};

/*!
 * \brief T_DOUBLE specialization
 */
template <class Field>
struct TPfrFieldHandler<
    Field, typename std::enable_if_t<std::is_floating_point_v<Field>>> {
  static void handleWrite(const Field &value, const std::string_view &name,
                          const size_t &index, uint32_t &written,
                          const std::shared_ptr<protocol::TProtocol> protocol) {
    written += protocol->writeFieldBegin(name.data(), protocol::TType::T_DOUBLE,
                                         index);
    written += protocol->writeDouble(static_cast<double>(value));
    written += protocol->writeFieldEnd();
  }

  static void handleRead(Field &value, uint32_t &read,
                         const std::shared_ptr<protocol::TProtocol> protocol) {
    double tValue{};
    uint32_t tRead{read};
    tRead += protocol->readDouble(tValue);
    if (tRead > read) {
      value = static_cast<double>(tValue);
      read = tRead;
    }
  }
};

/*!
 * \brief T_MAP specialization
 */
template <class Key, class Value>
struct TPfrFieldHandler<std::map<Key, Value>> {
  static void handleWrite(const std::map<Key, Value> &map,
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

  static void handleRead(std::map<Key, Value> &map, uint32_t &read,
                         const std::shared_ptr<protocol::TProtocol> protocol) {
    protocol::TType keyType{}, valueType{};
    uint32_t mapSize{0};
    read += protocol->readMapBegin(keyType, valueType, mapSize);
    handleMapRecord(map, mapSize, read, protocol);
  }

private:
  static void
  handleMapRecord(std::map<Key, Value> &map, uint32_t &mapSize,
                  uint32_t &bytesRead,
                  const std::shared_ptr<protocol::TProtocol> protocol) {
    if (mapSize == 0) {
      bytesRead += protocol->readMapEnd();
      return;
    }
    auto pair = std::make_pair<Key, Value>({}, {});
    TPfrFieldHandler<Key>::handleRead(pair.first, bytesRead, protocol);
    TPfrFieldHandler<Value>::handleRead(pair.second, bytesRead, protocol);
    map.emplace(std::move(pair));
    mapSize -= 1;
    handleMapRecord(map, mapSize, bytesRead, protocol);
  }
};

/*!
 * \brief T_STRING specialization
 */
template <> struct TPfrFieldHandler<std::string> {
  static void handleWrite(const std::string &s, const std::string_view &name,
                          const size_t &index, uint32_t &written,
                          const std::shared_ptr<protocol::TProtocol> protocol) {
    written += protocol->writeFieldBegin(name.data(), protocol::TType::T_STRING,
                                         index);
    written += protocol->writeString(s);
    written += protocol->writeFieldEnd();
  }

  static void handleRead(std::string &s, uint32_t &read,
                         const std::shared_ptr<protocol::TProtocol> protocol) {
    read += protocol->readString(s);
  }
};

/*!
 * \brief T_STRING specialization for string_view
 */
template <> struct TPfrFieldHandler<std::string_view> {
  static void handleWrite(const std::string_view &s,
                          const std::string_view &name, const size_t &index,
                          uint32_t &written,
                          const std::shared_ptr<protocol::TProtocol> protocol) {
    written += protocol->writeFieldBegin(name.data(), protocol::TType::T_STRING,
                                         index);
    written +=
        protocol->writeString(s.data()); // only writeString(const std::string&
                                         // str) exists in TProtocol.h
    written += protocol->writeFieldEnd();
  }

  static void handleRead(std::string_view &value, uint32_t &read,
                         const std::shared_ptr<protocol::TProtocol> protocol) {
    std::string tStr{};
    tStr.reserve(128);
    read += protocol->readString(tStr);

    if (value.empty())
      return;

    ///\warning Danger operation. Memory corruption is possible
    memcpy((void *)value.data(), tStr.data(),
           std::min(tStr.size(), value.size()));
  }
};

/*!
 * \brief T_STRING specialization for vector<char>
 */
template <> struct TPfrFieldHandler<std::vector<char>> {
    static void handleWrite(const std::vector<char> &s,
                            const std::string_view &name, const size_t &index,
                            uint32_t &written,
                            const std::shared_ptr<protocol::TProtocol> protocol) {
        written += protocol->writeFieldBegin(name.data(), protocol::TType::T_STRING,
                                             index);
        std::string tmp;
        tmp.reserve(s.size());
        std::copy(std::begin(s),std::end(s),std::back_inserter(tmp));
        written +=
            protocol->writeBinary(tmp);
        written += protocol->writeFieldEnd();
    }

    static void handleRead(std::vector<char> &value, uint32_t &read,
                           const std::shared_ptr<protocol::TProtocol> protocol) {
        std::string tStr{};
        tStr.reserve(128);///\todo why 128?
        read += protocol->readString(tStr);

        if (value.empty())
            return;

        value.clear();
        value.reserve(tStr.size());
        std::copy(std::begin(tStr), std::end(tStr), std::back_inserter(value));
    }
};

/*!
 * \brief T_LIST specialization
 */
template <class Value> struct TPfrFieldHandler<std::list<Value>> {
  static void handleWrite(const std::list<Value> &lst,
                          const std::string_view &name, const size_t &index,
                          uint32_t &written,
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

  static void handleRead(std::list<Value> &list, uint32_t &read,
                         const std::shared_ptr<protocol::TProtocol> protocol) {
    protocol::TType valueType{};
    uint32_t listSize{0};
    read += protocol->readListBegin(valueType, listSize);
    handleListRecord(list, listSize, read, protocol);
  }

private:
  static void
  handleListRecord(std::list<Value> &list, uint32_t &listSize,
                   uint32_t &bytesRead,
                   const std::shared_ptr<protocol::TProtocol> protocol) {
    if (listSize == 0) {
      bytesRead += protocol->readListEnd();
      return;
    }
    Value tVal{};
    TPfrFieldHandler<Value>::handleRead(tVal, bytesRead, protocol);
    list.emplace_back(std::move(tVal));
    listSize -= 1;
    handleListRecord(list, listSize, bytesRead, protocol);
  }
};

/*!
 * \brief T_SET specialization
 */
template <class Value> struct TPfrFieldHandler<std::set<Value>> {
  static void handleWrite(const std::set<Value> &set,
                          const std::string_view &name, const size_t &index,
                          uint32_t &written,
                          const std::shared_ptr<protocol::TProtocol> protocol) {
    written +=
        protocol->writeFieldBegin(name.data(), protocol::TType::T_SET, index);
    written += protocol->writeSetBegin(TPfrType<Value>::type, set.size());
    for (auto &es : set) {
      ListValueHandler<Value>::handle(es, protocol.get(), written);
    }
    written += protocol->writeSetEnd();
    written += protocol->writeFieldEnd();
  }

  static void handleRead(std::set<Value> &value, uint32_t &read,
                         const std::shared_ptr<protocol::TProtocol> protocol) {
    // read += protocol->readBool(value);
  }
};

} // namespace apache::thrift::serialize
