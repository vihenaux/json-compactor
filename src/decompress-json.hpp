#pragma once

#include <string>
#include "bitstream.hpp"
#include "cpp-any-type/any.hpp"

void decompress(std::string & const json_file_path, std::string & const output_path);
any_type::Any decompress(BitStream & bitStream);

any_type::Any decompress_root_node(BitStream & bitStream);

/*{{{ FUNCTION compress_object_detail }}}*/
// /*{{{ IF /*{{{ VALUE parent_type }}}*/ == object }}}*/
any_type::Any decompress_/*{{{ VALUE object_name }}}*/(BitStream & bitStream);
/*{{{ END }}}*/
// /*{{{ IF /*{{{ VALUE parent_type }}}*/ == array }}}*/
any_type::Any decompress_/*{{{ VALUE object_name }}}*/_item(BitStream & bitStream);
/*{{{ END }}}*/
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