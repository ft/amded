/*
 * Copyright (c) 2013-2017 amded workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file cmdline.cpp
 * @brief Command line argument processing
 */

#include <cstdlib>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include "cmdline.h"
#include "file-spec.h"
#include "setup.h"
#include "tag.h"
#include "amded.h"

/**
 * Split a tag definition into key and value
 *
 * A tag definition looks like this: foo=bar
 *
 * ‘foo’ may not contain an equal sign and it may not be empty.. ‘bar’ may be
 * an arbitrary string; including the empty one.
 *
 * If there is no equal sign in ‘data’, or the string before the equal sign is
 * empty, throw ‘amded_broken_tag_def’.
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
        throw amded_broken_tag_def{};
    t.first = data.substr(0, eqidx);
    t.second = data.substr(eqidx + 1);
    return t;
}

/**
 * Return a ‘tag_type’ that corresponds to ‘name’.
 *
 * This is a look-up via ‘tag_map’. Returns TAG_INVALID if the tag name is not
 * supported by amded.
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
 * supported by amded.
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
 * amded.
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

static std::vector<std::string>
split(const std::string &str, const std::string &delim)
{
    std::vector<std::string> retval;
    size_t start = 0, end;

    for (;;) {
        end = str.find(delim, start);
        retval.push_back(str.substr(start, end - start));
        if (end == std::string::npos)
            return retval;
        start = end + 1;
    }
}

/**
 * Change one of amded's file-type to tag-implemenation maps
 *
 * Some file types may contain information in one or more of several different
 * tag implementations. For example, mp3 files may contain ID3v1, ID3v2 and
 * apetag tags. The read-map decides which implementation is prefered. The user
 * passes in a read-map definition via the command line. Its format looks like
 * this:
 *
 *   FILETYPE0=impl0,impl1,...[:FILETYPE1=impl4,impl5,...]
 *
 * The write-map is defined exactly like that.
 *
 * The filetag_map from file-spec.cpp defines which tag-implementations are
 * valid for which file type.
 *
 * This function reads this definition and fills the ‘m’ parameter.
 *
 * @param  m     map parameter to modify
 * @param  def   mapping definition string
 *
 * @return void
 */
static void
setup_map(std::map<enum file_type, std::vector< enum tag_impl>> &m,
          std::string def)
{
    std::vector<std::string> defs = split(def, ":");
    for (auto &di : defs) {
        std::vector<enum tag_impl> ttypes;
        std::pair<std::string, std::string> entry;
        try {
            entry = tag_arg_to_pair(di);
        }
        catch (amded_broken_tag_def) {
            std::cerr << PROJECT << ": Broken map-definition: "
                      << '"' << di << '"'
                      << std::endl;
            exit(EXIT_FAILURE);
        }
        Amded::FileType ft(entry.first);

        if (ft.get_id() == FILE_T_INVALID) {
            std::cerr << PROJECT << ": Invalid file type: "
                      << entry.first << std::endl;
            exit(EXIT_FAILURE);
        }
        if (!is_multitag_type(ft.get_id())) {
            std::cerr << PROJECT << ": File type is not a multi-tag type: "
                      << entry.first << std::endl;
            exit(EXIT_FAILURE);
        }

        std::vector<std::string> types = split(entry.second, ",");
        for (auto &ei : types) {
            Amded::TagImplementation ti(ei);
            if (ti.get_id() == TAG_T_INVALID ||
                ti.get_id() == TAG_T_NONE)
            {
                std::cerr << PROJECT << ": Invalid tag type: "
                          << ei << std::endl;
                exit(EXIT_FAILURE);
            }
            if (!tag_impl_allowed_for_file_type(ft.get_id(), ti.get_id())) {
                std::cerr << PROJECT << ": Tag type ("
                          << ti.get_label() << ") not allowed for file type: "
                          << ft.get_label() << std::endl;
                exit(EXIT_FAILURE);
            }
            ttypes.push_back(ti.get_id());
        }
        m[ft.get_id()] = ttypes;
    }
}

void
setup_readmap(std::string def)
{
    read_map = filetag_map;
    if (def != "")
        setup_map(read_map, def);
}

void
setup_writemap(std::string def)
{
    for (auto &iter : filetag_map)
        write_map[iter.first] = { iter.second[0] };

    if (def != "")
        setup_map(write_map, def);
}

void
amded_parameters(std::string def)
{
    for (auto &iter : split(def, ","))
        if (iter == "")
            continue;
        else if (iter == "show-empty")
            set_opt(AMDED_LIST_ALLOW_EMPTY_TAGS);
        else if (iter == "keep-unsupported")
            set_opt(AMDED_KEEP_UNSUPPORTED_TAGS);
        else if (iter == "json-dont-use-base64")
            set_opt(AMDED_JSON_DONT_USE_BASE64);
        else {
            std::cerr << PROJECT << ": Unknown parameter: `"
                      << iter << "'" << std::endl;
            exit(EXIT_FAILURE);
        }
}
