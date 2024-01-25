#pragma once

#include "TPfrFieldHandler.h"

#include <boost/pfr/core.hpp>
#include <memory>
#include <thrift/protocol/TProtocol.h>
#include <type_traits>

namespace apache::thrift::serialize {
/**
 * Serializes a given protocol to memory, disk, or over a network from a
 * model
 */
template <class Model> class TPfrSerializer {
  std::shared_ptr<protocol::TProtocol> protocol_;
  uint32_t totalBytesWriten_;

public:
  using thrift_model = Model;

  TPfrSerializer(std::shared_ptr<protocol::TProtocol> proto)
      : protocol_(std::move(proto)) {}

  const uint32_t &serialize(const thrift_model &modelData) {
    static_assert(std::is_class_v<std::decay_t<thrift_model>>,
                  "Data isn't class or struct!");
    totalBytesWriten_ = 0;
    totalBytesWriten_ += protocol_->writeStructBegin(thrift_model::name);
    boost::pfr::for_each_field(modelData, [&](auto &field, const size_t index) {
      using field_type =
          typename std::remove_reference_t<decltype(field)>::value_type;
      if (field.IsEmpty())
        return;
      TPfrFieldHandler<field_type>::handleWrite(
          field.Value(), field.Name(), index, totalBytesWriten_, protocol_);
    });
    totalBytesWriten_ += protocol_->writeStructEnd();

    return totalBytesWriten_;
  }
};
} // namespace apache::thrift::serialize
