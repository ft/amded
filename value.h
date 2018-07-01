/*
 * Copyright (c) 2013-2018 amded workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file value.h
 * @brief API of a container that holds multiple data-types
 */

#ifndef INC_VALUE_H
#define INC_VALUE_H

#include <string>

#include <tstring.h>

#include "amded.h"

class Value {
private:
    enum tag_type type;
    union {
        bool b;
        int i;
        TagLib::String s;
    };

public:
    /* exception class */
    class bad_accessor {};

    /* Default {de,con}struction */
    Value();
    ~Value();

    /* Construction from boolean */
    Value(bool);
    Value& operator=(const bool&);

    /* Construction from integers */
    Value(int);
    Value& operator=(const int&);

    /* Construction from strings */
    Value(const TagLib::String&);
    Value& operator=(const TagLib::String&);
    Value(const std::string&);
    Value& operator=(const std::string&);

    /* copy construction and assignment */
    Value(const Value&);
    Value& operator=(const Value&);

    /* move construction and assignment */
    Value(Value&&) noexcept;
    Value& operator=(Value&&) noexcept;

    enum tag_type get_type() const;
    bool get_bool() const;
    int get_int() const;
    TagLib::String get_str() const;

    void set_bool(bool);
    void set_int(int);
    void set_str(const TagLib::String &);
    void set_invalid(void);
};


#endif /* INC_VALUE_H */
