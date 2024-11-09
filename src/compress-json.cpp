#include "/*{{{ VALUE GCG_FILLED_VALUE_FILENAME}}}*/"
#include <fstream>
#include <iostream>
#include "cpp-any-type/any.hpp"

/* any first bit : null bit
 * Boolean :
 *  0 : null
 *  10 : False
 *  11 : True
 *
 * Integer (Half) :
 *  0 : null
 *  10000 : 0
 *  11111 : 15
 *
 * Integer (Byte) :
 * O : null
 * 10000.0000 : 0
 * 11111.1111 : 255
 *
 * Integer (16) :
 * 0 : null
 * 1 0...0 : 0
 * 1 1...1 : 65535
 *
 * Integer (32) :
 * 0 : null
 * 1 0...0 : 0
 * 1 1...1 : 2^32-1
 *
 * Integer (Variable) :
 * 0 : null
 * 1 00 8bits  : 1 Byte long storage
 * 1 01 16bits : 2 Byte long storage
 * 1 10 16bits : 4 Byte long storage
 * 1 11 16bits : 8 Byte long storage
 *
 * Float (Single precision) :
 * 0 : null
 * 1 32bits : Float value
 *
 * Float (Double precision) :
 * 0 : null
 * 1 64bits : Float value
 *
 * String (Very short) :
 * 0 : null
 * 1 Integer (Half) [Byte] : Up to 15 char string
 *
 * String (Short) :
 * 0 : null
 * 1 Integer (Byte) [Byte] : Up to 255 char string
 *
 * String (Variable) :
 * 0 : null
 * 1 Integer (Variable) [Byte] : Up to 2^64 char string
 *
 * Object :
 * 0 : null
 * 1 : exists
 *
 * Array (Integer any) :
 * 0 : null or empty
 * 1 Int items : Int+1 = number of items
 */

void compress(std::string & const json_file_path, std::string & const output_path)
{
    any_type::Any json_content = any_type::readJson(json_file_path);

    if(json_content.getStatus() != any_type::ANY_OBJECT_STATUS::OK)
    {
        std::cerr << "Error : Cannot open json source file at : " << json_file_path << std::endl;
        return;
    }
}
