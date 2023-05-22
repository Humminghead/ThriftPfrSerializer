#pragma once

#include <thrift/protocol/TProtocol.h>

#include <boost/pfr.hpp>
#include <iostream>
#include <memory>
#include <string_view>
#include <type_traits>
//#include "datatransportbase.h"

namespace apache::thrift::serialization
{
template <typename T>
class TModelField
{
public:
    using value_type = T;

    constexpr TModelField(std::string_view name, T&& value, bool empty = true) : m_Name{name}, m_Value{std::move(value)}, m_IsEmpty{empty}
    {
    }

    auto SetValue(const T& value) noexcept { m_Value = value; }
    auto SetValue(T&& value) noexcept
    {
        SetEmpty(false);
        m_Value = std::move(value);
    }
    auto SetEmpty(const bool empty) noexcept { m_IsEmpty = empty; }

    auto& GetValue() const noexcept { return m_Value; }
    auto& GetName() const noexcept { return m_Name; }
    bool IsEmpty() const noexcept { return m_IsEmpty; };

private:
    const std::string_view m_Name;
    T m_Value;
    bool m_IsEmpty;
};


template <class Field>
struct TPfrFieldHandler;

template <>
struct TPfrFieldHandler<int32_t> {
    static void Handle(const int& field, const std::string_view& name, const size_t index, size_t& written,
                       const std::shared_ptr<protocol::TProtocol> protocol)
    {
        written += protocol->writeFieldBegin(name.data(), protocol::TType::T_I32, index);
        written += protocol->writeI32(field);
        written += protocol->writeFieldEnd();
    }
};

template <>
struct TPfrFieldHandler<float> {
    static void Handle(const int& field, const std::string_view& name, const size_t index, size_t& written,
                       const std::shared_ptr<protocol::TProtocol> protocol)
    {
    }
};

// template<>
// struct TPfrFieldHandler<std::string>
//{
//    template<class F>
//    static auto Handle(const F& field, const std::shared_ptr<protocol::TProtocol> protocol){
//            size_t bytes_writen {0};
//            bytes_writen += protocol->writeFieldBegin("", protocol::TType::T_STRING, /*idx*/0);
//            bytes_writen += protocol->writeString(field);
//            bytes_writen += protocol->writeFieldEnd();
//            return bytes_writen;
//    }
//};


template <class T>
class TTT;

template <class Model>
class TPfrSerializer /*: public TAbstractSerializer*/
{
    std::shared_ptr<protocol::TProtocol> m_Protocol;
    size_t m_BytesWriten;

public:
    using thrift_model = Model;

    TPfrSerializer(std::shared_ptr<protocol::TProtocol> proto) : /*TAbstractSerializer(),*/ m_Protocol(std::move(proto)) {}

    bool Serialize(thrift_model&& data) noexcept
    {
        static_assert(std::is_class_v<std::decay_t<thrift_model>>, "Data isn't class or struct!");
        m_BytesWriten = 0;
        m_BytesWriten += m_Protocol->writeStructBegin("");
        boost::pfr::for_each_field(data, [&](auto& field, const size_t index) {
            using field_type = typename std::remove_reference_t<decltype(field)>::value_type;
            if (field.isEmpty()) return;
            TPfrFieldHandler<field_type>::Handle(field.GetValue(), field.GetName(), index, m_BytesWriten, m_Protocol);
        });
        m_BytesWriten += m_Protocol->writeStructEnd();

        return m_BytesWriten > 0;
    }
};
/// Add template specialization

}  // namespace apache::thrift::serialization
