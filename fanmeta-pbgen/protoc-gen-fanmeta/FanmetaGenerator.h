
#pragma once
#include <cstdint>
#include <string>

#include <google/protobuf/compiler/code_generator.h>

namespace google {
    namespace protobuf {
        class FileDescriptor;
    }  // namespace protobuf
}  // namespace google

namespace google {
    namespace protobuf {
        namespace compiler {
            class FanmetaGenerator : public CodeGenerator {
            public:
                FanmetaGenerator();
                ~FanmetaGenerator() override;

                static std::string GetOutputFileName(const FileDescriptor* file);
                static std::string GetOutputFileName(const std::string& file);

                // implements CodeGenerator ----------------------------------------

                bool Generate(const FileDescriptor* file, const std::string& parameter,
                    GeneratorContext* context, std::string* error) const override;

                uint64_t GetSupportedFeatures() const override;
                void SuppressFeatures(uint64_t features);

            private:
                std::string name_;
                uint64_t suppressed_features_ = 0;
            };

        }  // namespace compiler
    }  // namespace protobuf
}  // namespace google

