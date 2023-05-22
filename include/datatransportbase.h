#pragma once

namespace apache::thrift::serialization
{
class TAbstractSerializer
{
public:
    virtual ~TAbstractSerializer() = default;

    virtual void beforeSerialization() noexcept = 0;
    virtual void afterSerialization() noexcept = 0;
};
};  // namespace apache::thrift::serialization
