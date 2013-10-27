/*
 * Copyright (c) 2013 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in doc/LICENCE.
 */

/**
 * @file cmdline.cpp
 * @brief Command line argument processing
 */

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include "cmdline.h"
#include "taggit.h"

/**
 * A mapping of tag-names to internal representation and type.
 *
 * Internally, taggit does not process strings each time it has to check which
 * tag it is working on. Instead an internal identifier of type "enum tag_id"
 * is used.
 *
 * Since tags can contain different values, this also maps to an "enum
 * tag_type" value, that identifies the type of value a tag normally contains.
 */
static std::map< std::string,
                 std::pair< enum tag_id,
                            enum tag_type > >
tag_map = {
    { "artist",         { T_ARTIST,         TAG_STRING  } },
    { "album",          { T_ALBUM,          TAG_STRING  } },
    { "comment",        { T_COMMENT,        TAG_STRING  } },
    { "compilation",    { T_COMPILATION,    TAG_STRING  } },
    { "genre",          { T_GENRE,          TAG_STRING  } },
    { "tracknumber",    { T_TRACKNUMBER,    TAG_INTEGER } },
    { "tracktitle",     { T_TRACKTITLE,     TAG_STRING  } },
    { "year",           { T_YEAR,           TAG_INTEGER } }
};

/** Print a list of supported tags to ‘stdout‘. */
void
list_tags(void)
{
    for (auto &iter : tag_map)
        std::cout << iter.first << std::endl;
}

/**
 * Split a tag definition into key and value
 *
 * A tag definition looks like this: foo=bar
 *
 * ‘foo’ may not contain an equal sign and it may not be empty.. ‘bar’ may be
 * an arbitrary string; including the empty one.
 *
 * If there is no equal sign in ‘data’, or the string before the equal sign is
 * empty, throw ‘taggit_broken_tag_def’.
 *
 * @param   data      the tag definition to process
 *
 * @return A pair of strings: key and value
 */
std::pair< std::string, std::string >
tag_arg_to_pair(std::string data)
{
    std::pair< std::string, std::string > t;
    unsigned long eqidx;

    eqidx = data.find("=");
    if (eqidx == 0 || eqidx == data.npos)
        throw taggit_broken_tag_def{};
    t.first = data.substr(0, eqidx);
    t.second = data.substr(eqidx + 1);
    return t;
}

/**
 * Return a ‘tag_type’ that corresponds to ‘name’.
 *
 * This is a look-up via ‘tag_map’. Returns TAG_INVALID if the tag name is not
 * supported by taggit.
 *
 * @param    name        The tag-name to perform the loop-up on.
 *
 * @return enum tag_type for ‘name’. TAG_INVALID if ‘name’ is unknown.
 */
enum tag_type
tag_to_type(std::string name)
{
    auto p = tag_map.find(name);
    if (p == tag_map.end())
        return TAG_INVALID;
    return p->second.second;
}

/**
 * Return a ‘tag_id’ that corresponds to ‘name’.
 *
 * This is a look-up via ‘tag_map’. Returns T_UNKNOWN if the tag name is not
 * supported by taggit.
 *
 * @param    name        The tag-name to perform the loop-up on.
 *
 * @return enum tag_type for ‘name’. T_UNKNOWN if ‘name’ is unknown.
 */
enum tag_id
tag_to_id(std::string name)
{
    auto p = tag_map.find(name);
    if (p == tag_map.end())
        return T_UNKNOWN;
    return p->second.first;
}

/**
 * Convert ‘value’ to a Value class instance, depending on ‘type’.
 *
 * ‘Value’ is a class that can hold different types of data transparently.
 * Namely integers and strings, which are also possible values for tags in
 * taggit.
 *
 * This takes a string (possibly provided by the user on the command line) and
 * converts it into a value type hinted at by ‘type’. This function returns
 * TAG_INVALID if a conversion failed or if ‘type’ is unknown.
 *
 * @param    name        The tag-name to perform the loop-up on.
 *
 * @return enum tag_type for ‘name’. T_UNKNOWN if ‘name’ is unknown.
 */
Value
tag_value_from_value(enum tag_type type, std::string value)
{
    Value retval;

    if (type == TAG_STRING)
        retval = value;
    else if (type == TAG_INTEGER) {
        try {
            retval = std::stoi(value);
        }
        catch (std::invalid_argument) {
            std::cerr << "Invalid integer value: "
                      << value
                      << std::endl;
            goto error;
        }
        catch (std::out_of_range) {
            std::cerr << "Integer string out of range: "
                      << value
                      << std::endl;
            goto error;
        }
    } else {
        std::cerr << "Unknown tag type: " << type << std::endl;
        goto error;
    }

    return retval;
error:
    retval.set_invalid();
    return retval;
}
