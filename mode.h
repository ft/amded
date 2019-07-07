/*
 * Copyright (c) 2019 amded workers, All rights reserved.
 *
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file mode.h
 * @brief Encapsulation for amded's mode of operation
 */

#ifndef INC_MODE_H
#define INC_MODE_H

namespace Amded {

/** possible operation modes */
enum class OperationMode {
    /** no mode defined, yet; or broken operation mode */
    INVALID,
    /** list file's tags in human readable form */
    LIST_HUMAN,
    /** list file's tags in machine readable form */
    LIST_MACHINE,
    /** list file's tags in machine readable form - JSON flavour */
    LIST_JSON,
    /** modify meta information in file(s) */
    TAG,
    /** Remove all tags from a file */
    STRIP
};

class Mode {
public:
    Mode() : mode(OperationMode::INVALID) {};
    void set(OperationMode nm) { mode = nm; };
    OperationMode get(void) const { return mode; };
    bool is_invalid(void) const { return mode == OperationMode::INVALID; };
    bool is_list_mode(void) const {
        return (mode == OperationMode::LIST_MACHINE ||
                mode == OperationMode::LIST_HUMAN ||
                mode == OperationMode::LIST_JSON);
    };
    bool is_write_mode(void) const {
        return (mode == OperationMode::TAG ||
                mode == OperationMode::STRIP);
    };
    bool multimode_ok(void) const {
        return ((!is_invalid()) || mode != OperationMode::TAG);
    };
    bool singlemode_ok(void) const { return (!is_invalid()); };

private:
    OperationMode mode;
};

}; /* namespace Amded */

#endif /* INC_MODE_H */
