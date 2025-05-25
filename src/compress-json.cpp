#include "compress-json.hpp"
#include <fstream>
#include <iostream>

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
 * 1 10 32bits : 4 Byte long storage
 * 1 11 64bits : 8 Byte long storage
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

void compress(std::string const & json_file_path, std::string const & output_path)
{
    // Json source file read and checks
    any_type::Any json = any_type::readJson(json_file_path);
    if(json.getStatus() == any_type::ANY_OBJECT_STATUS::IMPORT_FAIL)
    {
        std::cerr << "Error : Cannot open json source file at : " << json_file_path << std::endl;
        return;
    }
    if(json.getStatus() == any_type::ANY_OBJECT_STATUS::KO)
    {
        std::cerr << "Error : unexpected json content" << std::endl;
        return;
    }

    BitStream compressed_stream;
    compress_root_node(json, compressed_stream);
    compressed_stream.save(output_path);
}

BitStream compress(std::string const & json_content)
{
    any_type::Any json = any_type::readJsonStr(json_content);

    BitStream compressed_stream;

    if(json.getStatus() == any_type::ANY_OBJECT_STATUS::KO)
    {
        std::cerr << "Error : unexpected json content" << std::endl;
        return compressed_stream;
    }

    compress_root_node(json, compressed_stream);
    return compressed_stream;
}


/*{{{ FUNCTION compress_object }}}*/
    // /*{{{ IF /*{{{ VALUE compress_param_type }}}*/ == object }}}*/
    if(node.contains("/*{{{ VALUE compress_param_name }}}*/"))
    {
        bitStream.push(true);
        any_type::Any object_to_compress = node["/*{{{ VALUE compress_param_name }}}*/"];
        compress_/*{{{ VALUE compress_param_name }}}*/(object_to_compress, bitStream);
    }
    else
    {
        bitStream.push(false);
    }
    /*{{{ END }}}*/
/*{{{ END }}}*/

/*{{{ FUNCTION compress_integer }}}*/
    // /*{{{ IF /*{{{ VALUE compress_param_type }}}*/ == integer }}}*/
    if(node.contains("/*{{{ VALUE compress_param_name }}}*/"))
    {
        bitStream.push(true);
        bitStream.push(static_cast<unsigned long long>(node["/*{{{ VALUE compress_param_name }}}*/"].getInt()));
    }
    else
    {
        bitStream.push(false);
    }
    /*{{{ END }}}*/
/*{{{ END }}}*/

/*{{{ FUNCTION compress_string }}}*/
    // /*{{{ IF /*{{{ VALUE compress_param_type }}}*/ == string }}}*/
    if(node.contains("/*{{{ VALUE compress_param_name }}}*/"))
    {
        bitStream.push(true);
        bitStream.push(node["/*{{{ VALUE compress_param_name }}}*/"].getStr());
    }
    else
    {
        bitStream.push(false);
    }
    /*{{{ END }}}*/
/*{{{ END }}}*/

/*{{{ FUNCTION compress_boolean }}}*/
    // /*{{{ IF /*{{{ VALUE compress_param_type }}}*/ == boolean}}}*/
    if(node.contains("/*{{{ VALUE compress_param_name }}}*/"))
    {
        bitStream.push(true);
        bitStream.push(node["/*{{{ VALUE compress_param_name }}}*/"].getBool());
    }
    else
    {
        bitStream.push(false);
    }
    /*{{{ END }}}*/
/*{{{ END }}}*/

/*{{{ FUNCTION compress_float }}}*/
    // /*{{{ IF /*{{{ VALUE compress_param_type }}}*/ == float}}}*/
    if(node.contains("/*{{{ VALUE compress_param_name }}}*/"))
    {
        bitStream.push(true);
        bitStream.push_flt_str(node["/*{{{ VALUE compress_param_name }}}*/"].getFltStr());
    }
    else
    {
        bitStream.push(false);
    }
    /*{{{ END }}}*/
/*{{{ END }}}*/

/*{{{ FUNCTION compress_double }}}*/
    // /*{{{ IF /*{{{ VALUE compress_param_type }}}*/ == double }}}*/
    if(node.contains("/*{{{ VALUE compress_param_name }}}*/"))
    {
        bitStream.push(true);
        bitStream.push_flt_str(node["/*{{{ VALUE compress_param_name }}}*/"].getFltStr());
    }
    else
    {
        bitStream.push(false);
    }
    /*{{{ END }}}*/
/*{{{ END }}}*/

/*{{{ FUNCTION compress_array }}}*/
    // /*{{{ IF /*{{{ VALUE compress_param_type }}}*/ == array }}}*/
    if(node.contains("/*{{{ VALUE compress_param_name}}}*/"))
    {
        bitStream.push(true);
        bitStream.push(static_cast<unsigned long long>(node["/*{{{ VALUE compress_param_name }}}*/"].size()));
        for(unsigned int i(0); i < node["/*{{{ VALUE compress_param_name }}}*/"].size(); ++i)
        {
            // /*{{{ IF /*{{{ VALUE compress_param_array_type }}}*/ == object }}}*/
            any_type::Any item_to_compress = node["/*{{{ VALUE property_name }}}*/"][i];
            compress_/*{{{ VALUE property_name }}}*/_item(item_to_compress, bitStream);
            /*{{{ END }}}*/
            // /*{{{ IF /*{{{ VALUE compress_param_array_type }}}*/ == integer }}}*/
            bitStream.push(static_cast<unsigned long long>(node["/*{{{ VALUE property_name }}}*/"][i].getInt()));
            /*{{{ END }}}*/
            // /*{{{ IF /*{{{ VALUE compress_param_array_type }}}*/ == string }}}*/
            bitStream.push(node["/*{{{ VALUE property_name }}}*/"][i].getStr());
            /*{{{ END }}}*/
            // /*{{{ IF /*{{{ VALUE compress_param_array_type }}}*/ == boolean }}}*/
            bitStream.push(node["/*{{{ VALUE property_name }}}*/"][i].getBool());
            /*{{{ END }}}*/
            // /*{{{ IF /*{{{ VALUE compress_param_array_type }}}*/ == float }}}*/
            bitStream.push_flt_str(node["/*{{{ VALUE property_name }}}*/"][i].getFltStr());
            /*{{{ END }}}*/
            // /*{{{ IF /*{{{ VALUE compress_param_array_type }}}*/ == double }}}*/
            bitStream.push_flt_str(node["/*{{{ VALUE property_name }}}*/"][i].getFltStr());
            /*{{{ END }}}*/
        }
    }
    else
    {
        bitStream.push(false);
    }
    /*{{{ END }}}*/
/*{{{ END }}}*/

void compress_root_node(any_type::Any & node, BitStream & bitStream)
{
    // /*{{{ IF /*{{{ VALUE type}}}*/ == object }}}*/
    if(node.size() == 0)
    {
        bitStream.push(false);
        return;
    }
    bitStream.push(true);
    /*{{{ FOREACH property_name properties }}}*/

    // /*{{{ LET compress_param_type properties /*{{{ VALUE property_name }}}*/ type }}}*/
    // /*{{{ LET compress_param_name property_name }}}*/
    /*{{{ CALL compress_object }}}*/
    /*{{{ CALL compress_integer }}}*/
    /*{{{ CALL compress_string }}}*/
    /*{{{ CALL compress_boolean }}}*/
    /*{{{ CALL compress_float }}}*/
    /*{{{ CALL compress_double }}}*/
    // /*{{{ LET compress_param_array_type properties /*{{{ VALUE property_name }}}*/ items type }}}*/
    /*{{{ CALL compress_array }}}*/

    /*{{{ END }}}*/
    /*{{{ END }}}*/
}

/*{{{ FUNCTION compress_object_detail }}}*/
// /*{{{ IF /*{{{ VALUE parent_type }}}*/ == object }}}*/
void compress_/*{{{ VALUE object_name }}}*/(any_type::Any & node, BitStream & bitStream)
/*{{{ END }}}*/
// /*{{{ IF /*{{{ VALUE parent_type }}}*/ == array }}}*/
void compress_/*{{{ VALUE object_name }}}*/_item(any_type::Any & node, BitStream & bitStream)
/*{{{ END }}}*/
{
    /*{{{ FOREACH property_name object_to_compress properties }}}*/
    // /*{{{ LET compress_param_type object_to_compress properties /*{{{ VALUE property_name }}}*/ type }}}*/
    // /*{{{ LET compress_param_name property_name }}}*/
    /*{{{ CALL compress_object }}}*/
    /*{{{ CALL compress_integer }}}*/
    /*{{{ CALL compress_string }}}*/
    /*{{{ CALL compress_boolean }}}*/
    /*{{{ CALL compress_float }}}*/
    /*{{{ CALL compress_double }}}*/
    // /*{{{ LET compress_param_array_type object_to_compress properties /*{{{ VALUE property_name }}}*/ items type }}}*/
    /*{{{ CALL compress_array }}}*/
    /*{{{ END }}}*/
}

/*{{{ END }}}*/

/*{{{ FUNCTION compress_all_objects_detail }}}*/
/*{{{ CALL compress_object_detail }}}*/
/*{{{ FOREACH property_name object_to_compress properties }}}*/
// /*{{{ IF /*{{{ VALUE object_to_compress properties /*{{{ VALUE property_name }}}*/ type }}}*/ == object }}}*/
// /*{{{ LET parent_type object_to_compress properties /*{{{ VALUE property_name }}}*/ type }}}*/
// /*{{{ LET object_name property_name }}}*/
// /*{{{ LET object_to_compress object_to_compress properties /*{{{ VALUE property_name }}}*/ }}}*/
/*{{{ CALL compress_all_objects_detail }}}*/
/*{{{ END }}}*/
// /*{{{ IF /*{{{ VALUE object_to_compress properties /*{{{ VALUE property_name }}}*/ type }}}*/ == array }}}*/
// /*{{{ IF /*{{{ VALUE object_to_compress properties /*{{{ VALUE property_name }}}*/ items type }}}*/ == object }}}*/
// /*{{{ LET parent_type object_to_compress properties /*{{{ VALUE property_name }}}*/ type }}}*/
// /*{{{ LET object_name property_name }}}*/
// /*{{{ LET object_to_compress object_to_compress properties /*{{{ VALUE property_name }}}*/ items }}}*/ }}}*/
/*{{{ CALL compress_all_objects_detail }}}*/
/*{{{ END }}}*/
/*{{{ END }}}*/
/*{{{ END }}}*/
/*{{{ END }}}*/

// /*{{{ IF /*{{{ VALUE type}}}*/ == object }}}*/
/*{{{ FOREACH property_name properties }}}*/
// /*{{{ IF /*{{{ VALUE properties /*{{{ VALUE property_name }}}*/ type }}}*/ == object }}}*/
// /*{{{ LET object_to_compress properties /*{{{ VALUE property_name }}}*/ }}}*/
// /*{{{ LET parent_type type }}}*/
// /*{{{ LET object_name property_name }}}*/
/*{{{ CALL compress_all_objects_detail }}}*/
/*{{{ END }}}*/
// /*{{{ IF /*{{{ VALUE properties /*{{{ VALUE property_name }}}*/ type }}}*/ == array }}}*/
// /*{{{ IF /*{{{ VALUE properties /*{{{ VALUE property_name }}}*/ items type }}}*/ == object }}}*/
// /*{{{ LET object_to_compress properties /*{{{ VALUE property_name }}}*/ items }}}*/ }}}*/
// /*{{{ LET parent_type properties /*{{{ VALUE property_name }}}*/ type }}}*/
// /*{{{ LET object_name property_name }}}*/
/*{{{ CALL compress_all_objects_detail }}}*/
/*{{{ END }}}*/
/*{{{ END }}}*/
/*{{{ END }}}*/
/*{{{ END }}}*/
