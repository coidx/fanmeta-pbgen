#include "FanmetaMessage.h"

#include <google/protobuf/io/printer.h>
#include <string>

using std::string;
using namespace google::protobuf;

const char* BASE_FIELD_FORMAT = "$p$";
class FanmetaMessageImpl
{
public:
	const Descriptor* message_desc;

public:
	static string GenRepeatedField(string& content);
	static string GenMapField(string& key_type, string& val_type);
	static void AddFrontTab(string& content);
	static void AddBackTab(string& content);
	static void AddTab(string& content);
	static string GenFieldTypeName(int type, const FieldDescriptor* f);
	void WriteField(io::Printer& printer);
};

string FanmetaMessageImpl::GenFieldTypeName(int type, const FieldDescriptor* f)
{
	string type_name;
	switch (type)
	{
	case FieldDescriptor::TYPE_ENUM:
	{
		type_name = "string";
		break;
	}
	case FieldDescriptor::TYPE_INT64:
	case FieldDescriptor::TYPE_UINT64:
	case FieldDescriptor::TYPE_FIXED64:
	{
		type_name = "long";
		break;
	}
	case FieldDescriptor::TYPE_UINT32:
	{
		type_name = "uint";
		break;
	}
	case FieldDescriptor::TYPE_INT32:
	{
		type_name = "int";
		break;
	}
	case FieldDescriptor::TYPE_BYTES:
	{
		type_name = "byte[]";
		break;
	}
	case FieldDescriptor::TYPE_MESSAGE:
	{
		type_name = f->message_type()->name();
		break;
	}
	default:
	{
		type_name = f->type_name();
		break;
	}
	}

	if (f->is_map())
	{
		const FieldDescriptor* key = f->message_type()->map_key();
		const FieldDescriptor* val = f->message_type()->map_value();
		auto key_name = GenFieldTypeName(key->type(), key);
		auto val_name = GenFieldTypeName(val->type(), val);
		type_name = GenMapField(key_name, val_name);
	}
	else if (f->is_repeated())
	{

		type_name = GenRepeatedField(type_name);
	}

	return type_name;
}


string FanmetaMessageImpl::GenRepeatedField(string& content)
{
	content = ("LuaArray<" + content + ">");
	return content;
}

string FanmetaMessageImpl::GenMapField(string& key_type, string& val_type)
{
	return ("LuaMap<" + key_type + "," + val_type + ">");
}


void FanmetaMessageImpl::AddFrontTab(string& content)
{
	content.insert(0, "\t");
}

void FanmetaMessageImpl::AddBackTab(string& content)
{
	content.push_back('\t');
}

void FanmetaMessageImpl::AddTab(string& content)
{
	AddFrontTab(content);
	AddBackTab(content);
}

void FanmetaMessageImpl::WriteField(io::Printer& printer)
{
	// Fields
	auto public_str = "\tpublic ";
	for (int j = 0; j < message_desc->field_count(); j++)
	{
		auto f = message_desc->field(j);
		auto type_format_val = string(BASE_FIELD_FORMAT);
		auto name_format_val = string(BASE_FIELD_FORMAT);
		auto type_name = GenFieldTypeName(f->type(), f);

		printer.WriteRaw(public_str, strlen(public_str));
		AddBackTab(type_format_val);
		printer.Print(type_format_val.c_str(), "p", type_name);
		printer.Print(name_format_val.c_str(), "p", f->name());
		printer.WriteRaw(";\n", 2);
	}
}


FanmetaMessage::FanmetaMessage(const Descriptor* descriptor)
{
	impl_ = new FanmetaMessageImpl;
	impl_->message_desc = descriptor;
}

FanmetaMessage::~FanmetaMessage()
{
	if (impl_ != nullptr)
	{
		delete(impl_);
		impl_ = nullptr;
	}
}

void FanmetaMessage::Write(io::Printer& printer)
{
	printer.Print("public class $p$ {\n", "p", impl_->message_desc->name());
	impl_->WriteField(printer);
	printer.Print("}\n");

}


