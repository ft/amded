/*
 * Copyright (c) 2013-2017 amded workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file value.cpp
 * @brief Implementation of a container that holds multiple data-types
 */

#include <string>

#include <tstring.h>

#include "amded.h"
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
    if (type != TAG_INTEGER) {
        throw bad_accessor {};
    }
    return i;
}

bool
Value::get_bool(void) const
{
    if (type != TAG_BOOLEAN) {
        throw bad_accessor {};
    }
    return b;
}

TagLib::String
Value::get_str(void) const
{
    if (type != TAG_STRING) {
        throw bad_accessor {};
    }
    return s;
}

void
Value::set_int(int new_i)
{
    if (type == TAG_STRING) {
        s.~stdstring();
    }
    type = TAG_INTEGER;
    i = new_i;
}

void
Value::set_bool(bool new_b)
{
    if (type == TAG_STRING) {
        s.~stdstring();
    }
    type = TAG_BOOLEAN;
    b = new_b;
}

void
Value::set_invalid(void)
{
    if (type == TAG_STRING) {
        s.~stdstring();
    }
    type = TAG_INVALID;
}

void
Value::set_str(const TagLib::String &new_s)
{
    if (type == TAG_STRING) {
        s = new_s;
    } else {
        new(&s) TagLib::String{new_s};
        type = TAG_STRING;
    }
}

Value::Value()
{
    type = TAG_INVALID;
}

Value::Value(bool new_b)
{
    type = TAG_BOOLEAN;
    b = new_b;
}

Value::Value(int new_i)
{
    type = TAG_INTEGER;
    i = new_i;
}

Value::Value(const TagLib::String &new_s)
{
    type = TAG_STRING;
    new(&s) TagLib::String{new_s};
}

Value::Value(const std::string &new_s)
{
    type = TAG_STRING;
    new(&s) TagLib::String(new_s, TagLib::String::Type::UTF8);
}

Value::Value(const Value &orig)
{
    switch (orig.type) {
    case TAG_BOOLEAN:
        b = orig.b;
        type = TAG_BOOLEAN;
        break;
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

Value::Value (Value &&orig) noexcept
{
    switch (orig.type) {
    case TAG_BOOLEAN:
        b = orig.b;
        type = TAG_BOOLEAN;
        break;
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
    orig.type = TAG_INVALID;
}

Value::~Value()
{
    if (type == TAG_STRING) {
        s.~stdstring();
    }
}

Value &
Value::operator=(const Value &new_value)
{
    if (type == TAG_STRING && new_value.type == TAG_STRING) {
        s = new_value.s;
        return *this;
    }

    if (type == TAG_STRING) {
        s.~stdstring();
    }

    switch (new_value.type) {
    case TAG_STRING:
        new(&s) TagLib::String{new_value.s};
        break;
    case TAG_BOOLEAN:
        b = new_value.b;
        break;
    default:
        i = new_value.i;
    }

    type = new_value.type;
    return *this;
}

Value &
Value::operator=(Value &&new_value) noexcept
{
    if (type == TAG_STRING && new_value.type == TAG_STRING) {
        s = new_value.s;
        return *this;
    }

    if (type == TAG_STRING) {
        s.~stdstring();
    }

    switch (new_value.type) {
    case TAG_STRING:
        new(&s) TagLib::String{new_value.s};
        break;
    case TAG_BOOLEAN:
        b = new_value.b;
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
Value::operator=(const bool &new_value)
{
    this->set_bool(new_value);
    return *this;
}

Value &
Value::operator=(const TagLib::String &new_value)
{
    this->set_str(new_value);
    return *this;
}

Value &
Value::operator=(const std::string &new_value)
{
    TagLib::String n(new_value, TagLib::String::Type::UTF8);
    this->set_str(n);
    return *this;
}
