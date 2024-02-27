# ThriftPfrSerializer

- [Design goals](#design-goals)
- [Example](#example)
- [Requirements](#requirements)

## Design goals
Apache Thrift comes with a complete set of server shells, ready to use, in almost all the supported languages. In some cases this is can be excess.
This library is one of many possible ways to avoiding of complicated code generation and gain control on it process. Now you can serialize/deserialize your data by the most optimal way, use pseudo-reflection mechanism from the [boost](https://www.boost.org/doc/libs/1_75_0/doc/html/boost_pfr.html) library.

## Example
```cpp

#include "TModelField.h"
#include "TPfrDeSerializer.h"
#include "TPfrSerializer.h"
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/transport/TSimpleFileTransport.h>

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


void serializeExample(std::filesystem::path&& path)
{
  auto transWr = std::make_shared<transport::TSimpleFileTransport>(
      path.string(), false, true);

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

void deserializeExample(std::filesystem::path&& path){
  auto transRd = std::make_shared<transport::TSimpleFileTransport>(
      path.string(), true, false);

  auto protoRd = std::make_shared<protocol::TJSONProtocol>(transRd);

  MyModel data;
  serialize::TPfrDeserializer<MyModel> deserialzer(protoRd);

  deserialzer.deserialize(data);
}


int main()
{
  serializeExample("/tmp/out.json");
  deserializeExample("/tmp/out.json");
  retun 0;
}

```

In this example the model data will be serialized as:

```json
{
  "2": {
    "i8": 1
  },
  "9": {
    "dbl": 2.5
  },
  "11": {
    "str": "Hello Thrift"
  },
  "12": {
    "lst": [
      "i32",
      4,
      1,
      2,
      3,
      12
    ]
  }
}
```

and deserialized as:

```cpp
struct MyModel { 
  ...
  serialize::TModelField<int8_t> f2{"Int8", {1}, false};
  ...
  serialize::TModelField<double> f9{"Double", {2.5}, false};  
  ...
  serialize::TModelField<std::string> f11{"String", {"Hello Thrift"}, false};  
  serialize::TModelField<std::list<int>> f12{"List", {1, 2, 3, 12}, false};  
};
```
All other fields remain empty (TModelField::IsEmpty() returns true).

Additional examples are available at [example](https://github.com/Humminghead/ThriftPfrSerializer/tree/main/example) directory

## Requirements
- Compiler with support C++17 
- Apache Thrift [library](https://thrift.apache.org/download) (tested on version 0.15.0)
- Boost Pfr [library](https://www.boost.org/doc/libs/1_75_0/doc/html/boost_pfr.html) or boost >= 1.75
