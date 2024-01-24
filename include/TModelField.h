#pragma once

#include <string_view>

namespace apache::thrift::serialize {

using TModelName = const char* const;

template <typename T> class TModelField {
public:
  using value_type = T;

  constexpr TModelField(const std::string_view name, const T &value,
                        bool empty = true)
      : mNname{name}, mValue{std::move(value)}, mEmpty{empty} {}

  constexpr TModelField(const std::string_view name, T &&value,
                        bool empty = true)
      : mNname{name}, mValue{std::move(value)}, mEmpty{empty} {}

  auto SetValue(const T &value) noexcept {
    SetEmpty(false);
    mValue = value;
  }
  auto SetValue(T &&value) noexcept {
    SetEmpty(false);
    mValue = std::move(value);
  }
  auto SetEmpty(const bool empty) noexcept { mEmpty = empty; }

  auto &ModValue() noexcept { return mValue; }
  auto &Value() const noexcept { return mValue; }
  auto &Name() const noexcept { return mNname; }
  bool IsEmpty() const noexcept { return mEmpty; };

private:
  const std::string_view mNname;
  T mValue;
  bool mEmpty;
};

} // namespace apache::thrift::serialize
