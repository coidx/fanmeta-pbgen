
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/descriptor.h>

#pragma once
class FanmetaEnum
{
public:
	FanmetaEnum(const google::protobuf::EnumDescriptor* descriptor);
	~FanmetaEnum();

public:
	void Write(google::protobuf::io::Printer& printer);

private:
	class FanmetaEnumImpl* impl_;
};

