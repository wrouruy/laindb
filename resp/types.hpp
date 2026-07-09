#pragma once

enum class RespType {
    SIMPLE_STR,     // +
    SIMPLE_ERR,     // -
    INTEGERS,       // :
    BULK_STR,       // $
    NULL_BULK_STR,  // $-1
    ARRAYS,         // *
    NULLS,          // _
    BOOLEANS,       // #
    DOUBLES,        // ,
    BIG_NUMBERS,    // (
    BULK_ERR,       // !
    VERBATIM_STR,   // =
    MAPS,           // %
    ATTRIBUTES,     // |
    SETS,           // ~
    PUSHES          // >
};

struct RespToken {
    std::string value;
    RespType type;
};

