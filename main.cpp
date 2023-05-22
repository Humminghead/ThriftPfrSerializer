#include <iostream>
//#include "datatransportbase.h"
#include "universalpipe.h"

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

struct MyModel{
    serialization::TModelField<int> a{"Field0", {}, true};
//    serialization::TModelField<float> b{"Field1", {}, true};
//    serialization::TModelField<std::string> c{"Field2", {}, true};
};

int main()
{
    auto trans = std::make_shared<transport::TSimpleFileTransport>("/tmp/json.txt", false, true);
    auto proto = std::make_shared<protocol::TJSONProtocol>(trans);

    MyModel data;
    serialization::TPfrSerializer<MyModel> serialzer(proto);

    serialzer.Serialize(std::move(data));


    return 0;
}
