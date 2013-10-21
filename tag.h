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
    struct bad_accessor {};
    ~Value();
    Value();
    Value(const Value&);
    Value& operator=(const Value&);

    enum tag_type get_type() const;
    int get_int() const;
    std::string get_str() const;

    void set_int(int);
    void set_str(const std::string &);
    void set_invalid(void);
};

std::pair<std::string, std::string> tag_arg_to_pair(std::string);
enum tag_type tag_to_type(std::string);
Value tag_value_from_value(enum tag_type, std::string);
void list_tags(void);

#endif /* INC_TAG_H */
