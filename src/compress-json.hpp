#pragma once

#include <string>
#include "cpp-any-type/any.hpp"

void compress(std::string & const json_file_path, std::string & const output_path);
std::string compress(std::string & const json_content);

std::string compress_root_node(any_type::Any & root_node);
