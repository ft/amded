#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include "tag.h"
#include "taggit.h"

using stdstring = std::string;

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

static std::map< enum tag_id, Value > newtags;

void
add_tag(enum tag_id id, Value v)
{
    newtags[id] = v;
}

/** Print a list of supported tags to ‘stdout‘. */
void
list_tags(void)
{
    for (auto iter = tag_map.begin(); iter != tag_map.end(); ++iter)
        std::cout << iter->first << std::endl;
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

enum tag_type
tag_to_type(std::string name)
{
    auto p = tag_map.find(name);
    if (p == tag_map.end())
        return TAG_INVALID;
    return p->second.second;
}

enum tag_id
tag_to_id(std::string name)
{
    auto p = tag_map.find(name);
    if (p == tag_map.end())
        return T_UNKNOWN;
    return p->second.first;
}

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

/*
 * Implement Value methods:
 */

enum tag_type
Value::get_type(void) const
{
    return type;
}

int
Value::get_int(void) const
{
    if (type != TAG_INTEGER)
        throw bad_accessor {};

    return i;
}

std::string
Value::get_str(void) const
{
    if (type != TAG_STRING)
        throw bad_accessor {};

    return s;
}

void
Value::set_int(int new_i)
{
    if (type == TAG_STRING)
        s.~stdstring();

    type = TAG_INTEGER;
    i = new_i;
}

void
Value::set_invalid(void)
{
    if (type == TAG_STRING)
        s.~stdstring();
    type = TAG_INVALID;
}

void
Value::set_str(const std::string &new_s)
{
    if (type == TAG_STRING)
        s = new_s;
    else {
        new(&s) std::string{new_s};
        type = TAG_STRING;
    }
}

Value::Value()
{
    type = TAG_INVALID;
}

Value::Value(int new_i)
{
    type = TAG_INTEGER;
    i = new_i;
}

Value::Value(std::string new_s)
{
    type = TAG_STRING;
    new(&s) std::string{new_s};
}

Value::Value(const Value &orig)
{
    switch (orig.type) {
    case TAG_INTEGER:
        i = orig.i;
        type = TAG_INTEGER;
        break;
    case TAG_STRING:
        new(&s) std::string{orig.s};
        type = TAG_STRING;
        break;
    default:
        type = TAG_INVALID;
    }
}

Value::Value(Value &&orig)
{
    switch (orig.type) {
    case TAG_INTEGER:
        i = orig.i;
        type = TAG_INTEGER;
        break;
    case TAG_STRING:
        new(&s) std::string{std::move(orig.s)};
        type = TAG_STRING;
        break;
    default:
        type = TAG_INVALID;
    }
    orig.type = TAG_INVALID;
}

Value::~Value()
{
    if (type == TAG_STRING)
        s.~stdstring();
}

Value &
Value::operator=(const Value &new_value)
{
    if (type == TAG_STRING && new_value.type == TAG_STRING) {
        s = new_value.s;
        return *this;
    }

    if (type == TAG_STRING)
        s.~stdstring();

    switch (new_value.type) {
    case TAG_STRING:
        new(&s) std::string{new_value.s};
        break;
    default:
        i = new_value.i;
    }

    type = new_value.type;
    return *this;
}

Value &
Value::operator=(Value &&new_value)
{
    if (type == TAG_STRING && new_value.type == TAG_STRING) {
        s = std::move(new_value.s);
        return *this;
    }

    if (type == TAG_STRING)
        s.~stdstring();

    switch (new_value.type) {
    case TAG_STRING:
        new(&s) std::string{std::move(new_value.s)};
        break;
    default:
        i = new_value.i;
    }

    type = new_value.type;
    return *this;
}

Value &
Value::operator=(const int &new_value)
{
    this->set_int(new_value);
    return *this;
}

Value &
Value::operator=(const std::string &new_value)
{
    this->set_str(new_value);
    return *this;
}
