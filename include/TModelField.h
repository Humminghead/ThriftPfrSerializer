#pragma once

#include <TPfrMacro.h>
#include <TPfrValueEraser.h>
#include <string_view>

namespace apache::thrift::serialize {

using TModelName = const char *const;

/**
 Class helps to define a communication interface used for the communication
 between services
 */
template <typename T> class TModelField {
  public:
    using value_type = T;

    /**
     Creates a model field
     */
    constexpr TModelField(const std::string_view name, const value_type &value, bool empty = true)
        : mName{name}, mValue{std::move(value)}, mEmpty{empty} {}

    constexpr TModelField(const std::string_view name, value_type &&value, bool empty = true)
        : mName{name}, mValue{std::move(value)}, mEmpty{empty} {}

    /**
      Copy\Move operations
     */
    TModelField(const TModelField &f) : mName{f.mName} {
        mEmpty = f.mEmpty;
        mValue = f.mValue;
    }

    TModelField(TModelField &&f) : mName{std::move(f.mName)} {
        mEmpty = std::move(f.mEmpty);
        mValue = std::move(f.mValue);
    };

    TModelField &operator=(const TModelField &f) {
        mName = f.mName;
        mEmpty = f.mEmpty;
        mValue = f.mValue;

        return *this;
    }

    TModelField &operator=(TModelField &&f) {
        mName = std::move(f.mName);
        mEmpty = std::move(f.mEmpty);
        mValue = std::move(f.mValue);

        return *this;
    };

    ~TModelField() = default;

    /**
     Sets the field value
     */
    auto SetValue(const value_type &value) noexcept {
        SetEmpty(false);
        mValue = value;
    }
    auto SetValue(value_type &&value) noexcept {
        SetEmpty(false);
        mValue = std::move(value);
    }

    /**
     Sets the field's state to empty (it will not be serialized if it is empty)
     */
    auto SetEmpty(const bool empty) noexcept { mEmpty = empty; }

    /**
     Return the field's value
     */
    auto &Value() noexcept { return mValue; }
    auto &Value() const noexcept { return mValue; }
    /**
     Return the field's name
     */
    auto &Name() const noexcept { return mName; }

    /**
     Return the field's state
     */
    bool IsEmpty() const noexcept { return mEmpty; };

    /**
     Erarses the field's value to default value
     */
    void Erase() { ValueEraser<value_type>::Erase(mValue); SetEmpty(true); }

  private:
    std::string_view mName;
    value_type mValue;
    bool mEmpty;
};

} // namespace apache::thrift::serialize
