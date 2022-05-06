#include "FanmetaEnum.h"
#include <google/protobuf/io/printer.h>

using namespace google::protobuf;

class FanmetaEnumImpl
{
public:
	const EnumDescriptor* enum_descriptor;
};


FanmetaEnum::FanmetaEnum(const google::protobuf::EnumDescriptor* descriptor)
{
	impl_ = new FanmetaEnumImpl;
	impl_->enum_descriptor = descriptor;
}

FanmetaEnum::~FanmetaEnum()
{
	if (impl_ != nullptr)
	{
		delete impl_;
		impl_ = nullptr;
	}
}

void FanmetaEnum::Write(google::protobuf::io::Printer& printer)
{
	printer.Print("[CSLStub.Out(\"Network\")]\npublic static class $p$ {\n", "p", impl_->enum_descriptor->name());
	for (int i = 0; i < impl_->enum_descriptor->value_count(); ++i)
	{
		auto val = impl_->enum_descriptor->value(i);
		printer.Print("\tpublic const string $p$ = ", "p", val->name());
		printer.Print("\"$p$\";\n", "p", val->name());
	}
	printer.Print("}\n");
}


