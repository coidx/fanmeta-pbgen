
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/descriptor.h>

#pragma once
class FanmetaMessage
{
public:
	FanmetaMessage(const google::protobuf::Descriptor* descriptor);
	~FanmetaMessage();

public:
	void Write(google::protobuf::io::Printer& printer);

private:
	class FanmetaMessageImpl* impl_;
};

