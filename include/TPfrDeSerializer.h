#pragma once

#include "TPfrFieldHandler.h"

#include <boost/pfr/core.hpp>
#include <memory>
#include <thrift/protocol/TProtocol.h>
#include <type_traits>

namespace apache::thrift::serialize {

template <class Model> class TPfrDeserializer {
  std::shared_ptr<protocol::TProtocol> protocol_;  
  std::string mTemp_{};
  protocol::TType mType{};
  int16_t mId{0};

public:
  using thrift_model = Model;

  TPfrDeserializer(std::shared_ptr<protocol::TProtocol> proto, const size_t tempStrCapacity = 128)
      : protocol_(std::move(proto)) {
    mTemp_.reserve(tempStrCapacity);
  }

  uint32_t deserialize(thrift_model &modelData) {
    static_assert(std::is_class_v<std::decay_t<thrift_model>>,
                  "Data isn't class or struct!");
    uint32_t totalBytesRead = 0;
    mType = {};
    mId = {};
    mTemp_.clear();

    // Read begin
    totalBytesRead += protocol_->readStructBegin(mTemp_);
    mTemp_.clear();

    // First read
    totalBytesRead += protocol_->readFieldBegin(mTemp_, mType, mId);
    ///\todo if mType == T_STOP?
    boost::pfr::for_each_field(modelData, [&](auto &field, const size_t index) {
      using field_type =
          typename std::remove_reference_t<decltype(field)>::value_type;
      if (mType != protocol::TType::T_STOP) {
        if (index == mId) {
          field.SetEmpty(false);
          TPfrFieldHandler<field_type>::handleRead(field.Value(),
                                                   totalBytesRead, protocol_);
          totalBytesRead += protocol_->readFieldEnd();
          mTemp_.clear();
          mId = 0;
          totalBytesRead += protocol_->readFieldBegin(mTemp_, mType, mId);
        } else {
          field.SetEmpty(true);
        }
      }
    });
    totalBytesRead += protocol_->readStructEnd();
    return totalBytesRead;
  }

  thrift_model deserialize(uint32_t& totalBytesRead)
  {
      thrift_model message;
      totalBytesRead += deserialize(message);
      return message;
  }
};
} // namespace apache::thrift::serialize
