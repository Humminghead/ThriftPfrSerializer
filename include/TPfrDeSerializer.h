#pragma once

#include "TPfrFieldHandler.h"

#include <boost/pfr/core.hpp>
#include <memory>
#include <thrift/protocol/TProtocol.h>
#include <type_traits>

namespace apache::thrift::serialize {

template <class Model> class TPfrDeserializer {
  std::shared_ptr<protocol::TProtocol> protocol_;
  uint32_t totalBytesRead_;
  std::string mTemp_{};
  protocol::TType mType{};
  int16_t mId{0};

public:
  using thrift_model = Model;

  TPfrDeserializer(std::shared_ptr<protocol::TProtocol> proto)
      : protocol_(std::move(proto)) {
    mTemp_.reserve(1024);
  }

  bool deserialize(thrift_model &modelData) {
    static_assert(std::is_class_v<std::decay_t<thrift_model>>,
                  "Data isn't class or struct!");
    totalBytesRead_ = 0;
    mType = {};
    mId = {};
    mTemp_.clear();

    // Read begin
    totalBytesRead_ += protocol_->readStructBegin(mTemp_);
    mTemp_.clear();

    // First read
    totalBytesRead_ += protocol_->readFieldBegin(mTemp_, mType, mId);
    ///\todo if mType == T_STOP?
    boost::pfr::for_each_field(modelData, [&](auto &field, const size_t index) {
      using field_type =
          typename std::remove_reference_t<decltype(field)>::value_type;
      if (index == mId) {
        field.SetEmpty(false);
        TPfrFieldHandler<field_type>::handleRead(field.ModValue(),
                                                 totalBytesRead_, protocol_);
        totalBytesRead_ += protocol_->readFieldEnd();
        totalBytesRead_ += protocol_->readFieldBegin(mTemp_, mType, mId);
      } else {
        field.SetEmpty(true);
      }
    });
    totalBytesRead_ += protocol_->readStructEnd();
    return (totalBytesRead_ > 0);
  }
};
} // namespace apache::thrift::serialize
