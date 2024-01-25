#include "TModelField.h"
#include "TPfrDeSerializer.h"
#include "TPfrSerializer.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TFDTransport.h>
#include <thrift/transport/TFileTransport.h>
#include <thrift/transport/THeaderTransport.h>
#include <thrift/transport/TSimpleFileTransport.h>
#include <thrift/transport/TTransport.h>
#include <thrift/transport/TVirtualTransport.h>

#include <benchmark/benchmark.h>

using namespace apache::thrift;

struct MyModel {
  static constexpr serialize::TModelName name = "MyModel";
  serialize::TModelField<bool> f0{"Bool", {}};
  serialize::TModelField<char> f1{"Char", {}};
  serialize::TModelField<int8_t> f2{"Int8", {}};
  serialize::TModelField<uint8_t> f3{"Uint8", {}};
  serialize::TModelField<int32_t> f4{"Int", {}};
  serialize::TModelField<uint32_t> f5{"UInt", {}};
  serialize::TModelField<int64_t> f6{"Int64", {}};
  serialize::TModelField<uint64_t> f7{"UInt64", {}};
  serialize::TModelField<float> f8{"Float", {}};
  serialize::TModelField<double> f9{"Double", {3.8}};
  serialize::TModelField<std::map<int, double>> f10{"Map", {}};
  serialize::TModelField<std::string> f11{"String", {"Hello Thrift"}, false};
  serialize::TModelField<std::list<int>> f12{"List", {1, 2, 3}, false};
  serialize::TModelField<std::string_view> f13{"StringView", {}};
};

int main() {
  auto transWr = std::make_shared<transport::TSimpleFileTransport>(
      "/tmp/out.json", false, true);
  auto transRd = std::make_shared<transport::TSimpleFileTransport>(
      "/tmp/out.json", true, false);
  auto protoWr = std::make_shared<protocol::TJSONProtocol>(transWr);
  auto protoRd = std::make_shared<protocol::TJSONProtocol>(transRd);

  MyModel data;
  data.f2.SetValue(1);
  data.f9.SetValue(2.5);
  data.f10.Value().try_emplace(4, 5);
  data.f10.Value().try_emplace(6, 7);
  data.f12.Value().push_back(12);
  serialize::TPfrSerializer<MyModel> serialzer(protoWr);
  serialize::TPfrDeserializer<MyModel> deserialzer(protoRd);

  std::array<char,20> temp{};
  data.f13.SetValue(std::string_view{temp.data(),20});

  auto wr = serialzer.serialize(data);
  data.f2.Erase();
  data.f9.Erase();
  data.f10.Erase();
  data.f12.Erase();

  auto rd = deserialzer.deserialize(data);

  return wr == rd;
}
