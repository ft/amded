/*
 * Copyright (c) 2015-2017 amded workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file list-json.cpp
 * @brief Tag reader frontend for machines via JSON.
 */

#include <iostream>
#include <sstream>
#include <memory>
#include <json/json.h>
#include <json/writer.h>
#include <stdlib.h>

#include <b64/encode.h>

#include "list.h"
#include "list-json.h"
#include "setup.h"

static Json::Value data;

static void
push_item(std::string file, std::pair<const std::string, Value> &value)
{
    switch (value.second.get_type()) {
    case TAG_INTEGER:
        data[file][value.first] = value.second.get_int();
        break;
    case TAG_BOOLEAN:
        data[file][value.first] = value.second.get_bool();
        break;
    case TAG_STRING:
        if (get_opt(AMDED_JSON_DONT_USE_BASE64))
            data[file][value.first] =
                value.second.get_str().toCString(true);
        else {
            base64::encoder enc;
            std::istringstream in {value.second.get_str().to8Bit(true)};
            std::ostringstream out;
            enc.encode(in, out);
            data[file][value.first] = out.str();
        }
        break;
    default:
        std::cout << "Invalid type in: " << value.first << std::endl;
        std::cout << "This is a bug. Please report!" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void
amded_push_json(const struct amded_file &file)
{
    std::map<std::string, Value> basics;
    std::map<std::string, Value> tags;
    std::map<std::string, Value> props;

    basics = amded_list_amded(file);
    tags = amded_list_tags(file);
    props = amded_list_audioprops(file.fh->audioProperties());

    for (auto &iter : basics)
        push_item(file.name, iter);
    for (auto &iter : tags)
        push_item(file.name, iter);
    for (auto &iter : props)
        push_item(file.name, iter);
}

void
amded_list_json(void)
{
    Json::StreamWriterBuilder builder;
    /* This makes the JSON output minimally packed. */
    builder.settings_["indentation"] = "";
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(data, &std::cout);
}
