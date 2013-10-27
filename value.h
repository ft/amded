/*
 * Copyright (c) 2013 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file value.h
 * @brief API of a container that holds multiple data-types
 */

#ifndef INC_VALUE_H
#define INC_VALUE_H

#include <string>

#include "taggit.h"

class Value {
private:
    enum tag_type type;
    union {
        int i;
        std::string s;
    };

public:
    /* exception class */
    class bad_accessor {};

    /* Default {de,con}struction */
    Value();
    ~Value();

    /* Construction from integers */
    Value(int);
    Value& operator=(const int&);

    /* Construction from strings */
    Value(std::string);
    Value& operator=(const std::string&);

    /* copy construction and assignment */
    Value(const Value&);
    Value& operator=(const Value&);

    /* move construction and assignment */
    Value(Value&&);
    Value& operator=(Value&&);

    enum tag_type get_type() const;
    int get_int() const;
    std::string get_str() const;

    void set_int(int);
    void set_str(const std::string &);
    void set_invalid(void);
};


#endif /* INC_VALUE_H */
