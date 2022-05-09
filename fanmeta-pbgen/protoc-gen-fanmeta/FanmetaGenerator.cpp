#include "FanmetaGenerator.h"

#include <stdlib.h>

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include <google/protobuf/stubs/strutil.h>

#include <google/protobuf/stubs/logging.h>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/compiler/plugin.pb.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/text_format.h>
#include <google/protobuf/stubs/substitute.h>
#include "FanmetaMessage.h"
#include "FanmetaEnum.h"

#ifdef major
#undef major
#endif
#ifdef minor
#undef minor
#endif

namespace google {
	namespace protobuf {
		namespace compiler {

			// Returns the list of the names of files in all_files in the form of a
			// comma-separated string.
			std::string CommaSeparatedList(
				const std::vector<const FileDescriptor*>& all_files) {
				std::vector<std::string> names;
				for (size_t i = 0; i < all_files.size(); i++) {
					names.push_back(all_files[i]->name());
				}
				return Join(names, ",");
			}

			FanmetaGenerator::FanmetaGenerator() {}

			FanmetaGenerator::~FanmetaGenerator() {}

			uint64_t FanmetaGenerator::GetSupportedFeatures() const {
				uint64_t all_features = CodeGenerator::FEATURE_PROTO3_OPTIONAL;
				return all_features & ~suppressed_features_;
			}

			void FanmetaGenerator::SuppressFeatures(uint64_t features) {
				suppressed_features_ = features;
			}

			bool FanmetaGenerator::Generate(const FileDescriptor* file, const std::string& parameter, GeneratorContext* context, std::string* error) const {

				// Enum
				{
					std::unique_ptr<io::ZeroCopyOutputStream> output(context->Open(file->name() + ".enum.pb.cs"));
					io::Printer printer(output.get(), '$', nullptr);

					for (int i = 0; i < file->enum_type_count(); i++)
					{
						const EnumDescriptor* enum_desc = file->enum_type(i);
						FanmetaEnum enum_gen(enum_desc);
						enum_gen.Write(printer);
					}

					if (printer.failed()) {
						*error = "FanmetaEnum detected write error.";
						return false;
					}
				}
				// Message
				{
					std::unique_ptr<io::ZeroCopyOutputStream> output(context->Open(GetOutputFileName(file)));
					io::Printer printer(output.get(), '$', nullptr);

					for (int i = 0; i < file->message_type_count(); i++)
					{
						const Descriptor* message_desc = file->message_type(i);
						FanmetaMessage message(message_desc, file->package());
						message.Write(printer);
					}

					if (printer.failed()) {
						*error = "FanmetaMessage detected write error.";
						return false;
					}
				}

				// Rpc
				//{
				//	std::unique_ptr<io::ZeroCopyOutputStream> output(context->Open(file->name() + ".service.pb.cs"));
				//	io::Printer printer(output.get(), '$', nullptr);
				//	std::map<std::string, std::string> vars;

				//	for (int i = 0; i < file->service_count(); ++i)
				//	{
				//		auto service = file->service(i);
				//		
				//		printer.Print("\npublic static class RpcService {\n");
				//		for (int j = 0; j < service->method_count(); ++j)
				//		{
				//			auto method = service->method(i);
				//			vars.clear();
				//			vars.insert(std::make_pair("method_name", method->name()));
				//			vars.insert(std::make_pair("input_type", method->input_type()->name()));
				//			vars.insert(std::make_pair("output_type", method->output_type()->name()));
				//			printer.Print(vars, "\tpublic Promise $method_name$($input_type$ req) {\n\t}\n");
				//		}
				//		printer.Print("}\n");
				//	}
				//}
				return true;
			}

			std::string FanmetaGenerator::GetOutputFileName(const FileDescriptor* file) {
				return GetOutputFileName(file->name());
			}

			std::string FanmetaGenerator::GetOutputFileName(const std::string& file) {
				return file + ".pb.cs";
			}
		}  // namespace compiler
	}  // namespace protobuf
}  // namespace google

