#include "TModelField.h"
#include "TPfrSerializer.h"
#include <iostream>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TFDTransport.h>
#include <thrift/transport/TFileTransport.h>
#include <thrift/transport/TSimpleFileTransport.h>
#include <thrift/transport/TTransport.h>
#include <thrift/transport/TVirtualTransport.h>
using namespace std;
using namespace apache::thrift;

struct MyModel {
  static constexpr auto name = "MyModel";

  serialization::TModelField<int> a{"Field0", {}, true};
  serialization::TModelField<float> b{"Field1", {}, true};
  serialization::TModelField<double> c{"Field2", {3.8}, false};
  serialization::TModelField<std::map<int, int>> d{"Map", {}, false};
  //    serialization::TModelField<std::string> c{"Field2", {}, true};
};

int main() {
  auto trans = std::make_shared<transport::TSimpleFileTransport>(
      "/tmp/json.txt", false, true);
  auto proto = std::make_shared<protocol::TJSONProtocol>(trans);

  MyModel data;
  data.a.setValue(1);
  data.b.setValue(2.5);
  serialization::TPfrSerializer<MyModel> serialzer(proto);

  serialzer.serialize(std::move(data));

  return 0;
}
