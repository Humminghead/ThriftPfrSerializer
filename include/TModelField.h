#pragma once

#include <string_view>

namespace apache::thrift::serialization {
template <typename T> class TModelField {
public:
  using value_type = T;

  constexpr TModelField(std::string_view name, T &&value, bool empty = true)
      : name_{name}, value_{std::move(value)}, empty_{empty} {}

  auto setValue(const T &value) noexcept { value_ = value; }
  auto setValue(T &&value) noexcept {
    setEmpty(false);
    value_ = std::move(value);
  }
  auto setEmpty(const bool empty) noexcept { empty_ = empty; }

  auto &getValue() const noexcept { return value_; }
  auto &getName() const noexcept { return name_; }
  bool isEmpty() const noexcept { return empty_; };

private:
  const std::string_view name_;
  T value_;
  bool empty_;
};
} // namespace apache::thrift::serialization
