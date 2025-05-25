#include "decompress-json.hpp"

void decompress(std::string const & compressed_file_path, std::string const & output_path)
{
    BitStream input(compressed_file_path);
    any_type::saveJson(decompress_root_node(input), output_path);
}

any_type::Any decompress(std::string const & compressed_file_path)
{
    BitStream input(compressed_file_path);

    return decompress_root_node(input);
}


/*{{{ FUNCTION decompress_object }}}*/
// /*{{{ IF /*{{{ VALUE decompress_param_type }}}*/ == object }}}*/
    if(bitStream.get_bool())
    {
        object_to_return.add("/*{{{ VALUE decompress_param_name }}}*/", decompress_/*{{{ VALUE decompress_param_name }}}*/(bitStream));
    }
    else
    {
        object_to_return.add("/*{{{ VALUE decompress_param_name }}}*/", any_type::Any());
    }
/*{{{ END }}}*/
/*{{{ END }}}*/

/*{{{ FUNCTION decompress_integer }}}*/
// /*{{{ IF /*{{{ VALUE decompress_param_type }}}*/ == integer }}}*/
    if(bitStream.get_bool())
    {
        object_to_return.add("/*{{{ VALUE decompress_param_name }}}*/", any_type::Any(static_cast<int>(bitStream.get_int())));
    }
    else
    {
        object_to_return.add("/*{{{ VALUE decompress_param_name }}}*/", any_type::Any());
    }
/*{{{ END }}}*/
/*{{{ END }}}*/

/*{{{ FUNCTION decompress_string }}}*/
// /*{{{ IF /*{{{ VALUE decompress_param_type }}}*/ == string }}}*/
    if(bitStream.get_bool())
    {
        object_to_return.add("/*{{{ VALUE decompress_param_name }}}*/", any_type::Any(bitStream.get_str()));
    }
    else
    {
        object_to_return.add("/*{{{ VALUE decompress_param_name }}}*/", any_type::Any());
    }
/*{{{ END }}}*/
/*{{{ END }}}*/

/*{{{ FUNCTION decompress_boolean }}}*/
// /*{{{ IF /*{{{ VALUE decompress_param_type }}}*/ == boolean}}}*/
    if(bitStream.get_bool())
    {
        object_to_return.add("/*{{{ VALUE decompress_param_name }}}*/", any_type::Any(bitStream.get_bool()));
    }
    else
    {
        object_to_return.add("/*{{{ VALUE decompress_param_name }}}*/", any_type::Any());
    }
/*{{{ END }}}*/
/*{{{ END }}}*/

/*{{{ FUNCTION decompress_float }}}*/
// /*{{{ IF /*{{{ VALUE decompress_param_type }}}*/ == float}}}*/
    if(bitStream.get_bool())
    {
        std::string tmp_flt_str = bitStream.get_flt_str();
        object_to_return.add("/*{{{ VALUE decompress_param_name }}}*/", any_type::Any(std::stod(tmp_flt_str), tmp_flt_str));
    }
    else
    {
        object_to_return.add("/*{{{ VALUE decompress_param_name }}}*/", any_type::Any());
    }
/*{{{ END }}}*/
/*{{{ END }}}*/

/*{{{ FUNCTION decompress_double }}}*/
// /*{{{ IF /*{{{ VALUE decompress_param_type }}}*/ == double }}}*/
    if(bitStream.get_bool())
    {
        std::string tmp_flt_str = bitStream.get_flt_str();
        object_to_return.add("/*{{{ VALUE decompress_param_name }}}*/", any_type::Any(std::stod(tmp_flt_str), tmp_flt_str));
    }
    else
    {
        object_to_return.add("/*{{{ VALUE decompress_param_name }}}*/", any_type::Any());
    }
/*{{{ END }}}*/
/*{{{ END }}}*/

/*{{{ FUNCTION decompress_array }}}*/
// /*{{{ IF /*{{{ VALUE decompress_param_type }}}*/ == array }}}*/
    if(bitStream.get_bool())
    {
        any_type::Any array_items((std::vector<any_type::Any>()));
        unsigned long long array_size = bitStream.get_int();
        for(unsigned long long i(0); i < array_size; ++i)
        {
            // /*{{{ IF /*{{{ VALUE decompress_param_array_type }}}*/ == object }}}*/
            array_items.add(decompress_/*{{{ VALUE property_name }}}*/_item(bitStream));
            /*{{{ END }}}*/
            // /*{{{ IF /*{{{ VALUE decompress_param_array_type }}}*/ == integer }}}*/
            array_items.add(static_cast<int>(bitStream.get_int()));
            /*{{{ END }}}*/
            // /*{{{ IF /*{{{ VALUE decompress_param_array_type }}}*/ == string }}}*/
            array_items.add(bitStream.get_str());
            /*{{{ END }}}*/
            // /*{{{ IF /*{{{ VALUE decompress_param_array_type }}}*/ == boolean }}}*/
            array_items.add(bitStream.get_bool());
            /*{{{ END }}}*/
            // /*{{{ IF /*{{{ VALUE decompress_param_array_type }}}*/ == float }}}*/
            std::string tmp_flt_str = bitStream.get_flt_str();
            array_items.add(any_type::Any(std::stod(tmp_flt_str), tmp_flt_str));
            /*{{{ END }}}*/
            // /*{{{ IF /*{{{ VALUE decompress_param_array_type }}}*/ == double }}}*/
            std::string tmp_flt_str = bitStream.get_flt_str();
            array_items.add(any_type::Any(std::stod(tmp_flt_str), tmp_flt_str));
            /*{{{ END }}}*/
        }
        object_to_return.add("/*{{{ VALUE decompress_param_name }}}*/", array_items);
    }
    else
    {
        object_to_return.add("/*{{{ VALUE decompress_param_name }}}*/", any_type::Any());
    }
/*{{{ END }}}*/
/*{{{ END }}}*/

any_type::Any decompress_root_node(BitStream & bitStream)
{
    // /*{{{ IF /*{{{ VALUE type}}}*/ == object }}}*/
    if(!bitStream.get_bool())
    {
        return any_type::Any();
    }
    any_type::Any object_to_return((std::map<std::string,any_type::Any>()));
    /*{{{ FOREACH property_name properties }}}*/

    // /*{{{ LET decompress_param_type properties /*{{{ VALUE property_name }}}*/ type }}}*/
    // /*{{{ LET decompress_param_name property_name }}}*/
    /*{{{ CALL decompress_object }}}*/
    /*{{{ CALL decompress_integer }}}*/
    /*{{{ CALL decompress_string }}}*/
    /*{{{ CALL decompress_boolean }}}*/
    /*{{{ CALL decompress_float }}}*/
    /*{{{ CALL decompress_double }}}*/
    // /*{{{ LET decompress_param_array_type properties /*{{{ VALUE property_name }}}*/ items type }}}*/
    /*{{{ CALL decompress_array }}}*/

    /*{{{ END }}}*/
    /*{{{ END }}}*/
    return object_to_return;
}

/*{{{ FUNCTION decompress_object_detail }}}*/
// /*{{{ IF /*{{{ VALUE parent_type }}}*/ == object }}}*/
any_type::Any decompress_/*{{{ VALUE object_name }}}*/(BitStream & bitStream)
/*{{{ END }}}*/
// /*{{{ IF /*{{{ VALUE parent_type }}}*/ == array }}}*/
any_type::Any decompress_/*{{{ VALUE object_name }}}*/_item(BitStream & bitStream)
/*{{{ END }}}*/
{
    any_type::Any object_to_return((std::map<std::string,any_type::Any>()));

/*{{{ FOREACH property_name object_to_decompress properties }}}*/
// /*{{{ LET decompress_param_type object_to_decompress properties /*{{{ VALUE property_name }}}*/ type }}}*/
// /*{{{ LET decompress_param_name property_name }}}*/
/*{{{ CALL decompress_object }}}*/
/*{{{ CALL decompress_integer }}}*/
/*{{{ CALL decompress_string }}}*/
/*{{{ CALL decompress_boolean }}}*/
/*{{{ CALL decompress_float }}}*/
/*{{{ CALL decompress_double }}}*/
// /*{{{ LET decompress_param_array_type object_to_decompress properties /*{{{ VALUE property_name }}}*/ items type }}}*/
/*{{{ CALL decompress_array }}}*/
/*{{{ END }}}*/

    return object_to_return;
}

/*{{{ END }}}*/

/*{{{ FUNCTION decompress_all_objects_detail }}}*/
/*{{{ CALL decompress_object_detail }}}*/
/*{{{ FOREACH property_name object_to_decompress properties }}}*/
// /*{{{ IF /*{{{ VALUE object_to_decompress properties /*{{{ VALUE property_name }}}*/ type }}}*/ == object }}}*/
// /*{{{ LET parent_type object_to_decompress properties /*{{{ VALUE property_name }}}*/ type }}}*/
// /*{{{ LET object_name property_name }}}*/
// /*{{{ LET object_to_decompress object_to_decompress properties /*{{{ VALUE property_name }}}*/ }}}*/
/*{{{ CALL decompress_all_objects_detail }}}*/
/*{{{ END }}}*/
// /*{{{ IF /*{{{ VALUE object_to_decompress properties /*{{{ VALUE property_name }}}*/ type }}}*/ == array }}}*/
// /*{{{ IF /*{{{ VALUE object_to_decompress properties /*{{{ VALUE property_name }}}*/ items type }}}*/ == object }}}*/
// /*{{{ LET parent_type object_to_decompress properties /*{{{ VALUE property_name }}}*/ type }}}*/
// /*{{{ LET object_name property_name }}}*/
// /*{{{ LET object_to_decompress object_to_decompress properties /*{{{ VALUE property_name }}}*/ items }}}*/ }}}*/
/*{{{ CALL decompress_all_objects_detail }}}*/
/*{{{ END }}}*/
/*{{{ END }}}*/
/*{{{ END }}}*/
/*{{{ END }}}*/

// /*{{{ IF /*{{{ VALUE type}}}*/ == object }}}*/
/*{{{ FOREACH property_name properties }}}*/
// /*{{{ IF /*{{{ VALUE properties /*{{{ VALUE property_name }}}*/ type }}}*/ == object }}}*/
// /*{{{ LET object_to_decompress properties /*{{{ VALUE property_name }}}*/ }}}*/
// /*{{{ LET parent_type type }}}*/
// /*{{{ LET object_name property_name }}}*/
/*{{{ CALL decompress_all_objects_detail }}}*/
/*{{{ END }}}*/
// /*{{{ IF /*{{{ VALUE properties /*{{{ VALUE property_name }}}*/ type }}}*/ == array }}}*/
// /*{{{ IF /*{{{ VALUE properties /*{{{ VALUE property_name }}}*/ items type }}}*/ == object }}}*/
// /*{{{ LET object_to_decompress properties /*{{{ VALUE property_name }}}*/ items }}}*/ }}}*/
// /*{{{ LET parent_type properties /*{{{ VALUE property_name }}}*/ type }}}*/
// /*{{{ LET object_name property_name }}}*/
/*{{{ CALL decompress_all_objects_detail }}}*/
/*{{{ END }}}*/
/*{{{ END }}}*/
/*{{{ END }}}*/
/*{{{ END }}}*/
