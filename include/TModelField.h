#pragma once

#include <string_view>

namespace apache::thrift::serialize {

using TModelName = const char *const;

/**
 The ValueEraser class defines how to erase field's value
 */
template <typename T, typename Enable = void> struct ValueEraser {
  static_assert(false, "Please specify Erase function!");
};

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
  constexpr TModelField(const std::string_view name, const T &value,
                        bool empty = true)
      : mNname{name}, mValue{std::move(value)}, mEmpty{empty} {}

  constexpr TModelField(const std::string_view name, T &&value,
                        bool empty = true)
      : mNname{name}, mValue{std::move(value)}, mEmpty{empty} {}

  /**
    Copy\Move operations
   */
  TModelField(const TModelField &f) {
      mEmpty = f.mEmpty;
      mValue = f.mValue;
  }

  TModelField(TModelField &&f) {
      mEmpty = std::move(f.mEmpty);
      mValue = std::move(f.mValue);
  };

  TModelField &operator=(const TModelField &f) {
      mEmpty = f.mEmpty;
      mValue = f.mValue;
  }

  TModelField &operator=(TModelField &&f) {
      mEmpty = std::move(f.mEmpty);
      mValue = std::move(f.mValue);
  };

  ~TModelField() = default;

  /**
   Sets the field value
   */
  auto SetValue(const T &value) noexcept {
    SetEmpty(false);
    mValue = value;
  }
  auto SetValue(T &&value) noexcept {
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
  auto &Name() const noexcept { return mNname; }

  /**
   Return the field's state
   */
  bool IsEmpty() const noexcept { return mEmpty; };

  /**
   Erarses the field's value to default value
   */
  void Erase() { ValueEraser<T>::Erase(mValue); }

private:
  const std::string_view mNname;
  T mValue;
  bool mEmpty;
};

template <typename T>
struct ValueEraser<
    T, typename std::enable_if_t<std::is_default_constructible_v<T>>> {
  static void Erase(T &value) { value = {}; }
};

} // namespace apache::thrift::serialize
