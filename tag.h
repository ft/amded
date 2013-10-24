#ifndef INC_TAG_H
#define INC_TAG_H

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

std::pair<std::string, std::string> tag_arg_to_pair(std::string);
enum tag_id tag_to_id(std::string);
enum tag_type tag_to_type(std::string);
Value tag_value_from_value(enum tag_type, std::string);
void list_tags(void);
void add_tag(enum tag_id, Value);
void list_newtags(void);

#endif /* INC_TAG_H */
