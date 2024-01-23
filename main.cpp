#include "TModelField.h"
#include "TPfrSerializer.h"
#include <iostream>
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
  static constexpr auto name = "MyModel";

  serialize::TModelField<bool> f0{"Bool", {}, false};
  serialize::TModelField<char> f1{"Char", {}, false};
  serialize::TModelField<int8_t> f2{"Int8", {}, false};
  serialize::TModelField<uint8_t> f3{"Uint8", {}, false};
  serialize::TModelField<int32_t> f4{"Int", {}, false};
  serialize::TModelField<uint32_t> f5{"UInt", {}, false};
  serialize::TModelField<int64_t> f6{"Int64", {}, false};
  serialize::TModelField<uint64_t> f7{"UInt64", {}, false};
  serialize::TModelField<float> f8{"Float", {}, false};
  serialize::TModelField<double> f9{"Double", {3.8}, false};
  serialize::TModelField<std::map<int, double>> f10{"Map", {}, false};
  serialize::TModelField<std::string> f11{"String", {"string"}, false};
  serialize::TModelField<std::list<int>> f12{"List", {1,2,3}, false};
  serialize::TModelField<std::string_view> f13{
      "StringView", {"string_view"}, false};
};

int main() {
  auto trans = std::make_shared<transport::TSimpleFileTransport>(
      "/tmp/json.bin", false, true);
  auto proto = std::make_shared<protocol::TJSONProtocol>(trans);

  MyModel data;
  data.f2.SetValue(1);
  data.f9.SetValue(2.5);
  data.f10.ModValue().try_emplace(4, 5);
  data.f12.ModValue().push_back(12);
  // data.f12.ModValue().emplace(7);
  serialize::TPfrSerializer<MyModel> serialzer(proto);

  serialzer.serialize(std::move(data));

  return 0;
}
