// protoc-gen-fanmeta.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "plugin.h"
#include "FanmetaGenerator.h"

using namespace google::protobuf::compiler;

int main(int argc, char* argv[])
{
	FanmetaGenerator generator;
    return PluginMain(argc, argv, &generator);
}
