/*
 * Copyright (c) 2013 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file value.cpp
 * @brief Implementation of a container that holds multiple data-types
 */

#include <tstring.h>

#include "taggit.h"
#include "value.h"

using stdstring = TagLib::String;

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

TagLib::String
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
Value::set_str(const TagLib::String &new_s)
{
    if (type == TAG_STRING)
        s = new_s;
    else {
        new(&s) TagLib::String{new_s};
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

Value::Value(TagLib::String new_s)
{
    type = TAG_STRING;
    new(&s) TagLib::String{new_s};
}

Value::Value(const Value &orig)
{
    switch (orig.type) {
    case TAG_INTEGER:
        i = orig.i;
        type = TAG_INTEGER;
        break;
    case TAG_STRING:
        new(&s) TagLib::String{orig.s};
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
        new(&s) TagLib::String{std::move(orig.s)};
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
        new(&s) TagLib::String{new_value.s};
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
        new(&s) TagLib::String{std::move(new_value.s)};
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
Value::operator=(const TagLib::String &new_value)
{
    this->set_str(new_value);
    return *this;
}
