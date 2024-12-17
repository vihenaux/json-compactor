#pragma once

#include <string>
#include "bitstream.hpp"
#include "cpp-any-type/any.hpp"

void compress(std::string & const json_file_path, std::string & const output_path);
BitStream compress(std::string & const json_content);

void compress_root_node(any_type::Any & root_node, BitStream & bitStream);
