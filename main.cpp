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
};

void serializeExample(){
    auto transWr = std::make_shared<transport::TSimpleFileTransport>(
        "/tmp/out.json", false, true);

    auto protoWr = std::make_shared<protocol::TJSONProtocol>(transWr);
    serialize::TPfrSerializer<MyModel> serialzer(protoWr);

    MyModel data;
    data.f2.SetValue(1);
    data.f9.SetValue(2.5);
    data.f10.Value().try_emplace(4, 5);
    data.f10.Value().try_emplace(6, 7);
    data.f12.Value().push_back(12);

    serialzer.serialize(data);
}

void deserializeExample(){
    auto transRd = std::make_shared<transport::TSimpleFileTransport>(
        "/tmp/out.json", true, false);

    auto protoRd = std::make_shared<protocol::TJSONProtocol>(transRd);

    MyModel data;
    serialize::TPfrDeserializer<MyModel> deserialzer(protoRd);

    deserialzer.deserialize(data);
}


struct StringViewModel{
    static constexpr serialize::TModelName name = "";
    serialize::TModelField<std::string_view> f0{"StringView", {}};
};

void deserializeStringViewExample(){
    StringViewModel data;

    const std::array<char, 20> seriaizationMemTemp  {"Hello thrift!"};
          std::array<char, 20> deseriaizationMemTemp{};

    //Serialization
    {
        auto transWr = std::make_shared<transport::TSimpleFileTransport>("/tmp/sv.json", false, true);
        auto protoWr = std::make_shared<protocol::TJSONProtocol>(transWr);

        serialize::TPfrSerializer<StringViewModel> serialzer(protoWr);
        data.f0.SetValue(std::string_view{seriaizationMemTemp.data(), 20});
        serialzer.serialize(data);
    }

    // Deserialization
    {
        auto transRd = std::make_shared<transport::TSimpleFileTransport>("/tmp/sv.json", true, false);
        auto protoRd = std::make_shared<protocol::TJSONProtocol>(transRd);

        serialize::TPfrDeserializer<StringViewModel> deserialzer(protoRd);
        data.f0.SetValue(std::string_view{deseriaizationMemTemp.data(), 20});
        deserialzer.deserialize(data);
    }

    int compare{};
}


int main()
{
    serializeExample();
    deserializeExample();
    deserializeStringViewExample();

    return 0;
}
