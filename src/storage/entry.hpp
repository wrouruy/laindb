#pragma once

#include <string>
#include "../resp/types.hpp"

class Entry {
    public:
        std::string value;
        std::string key;
        RespType type;

        Entry* hash; // hMap
        Entry* next;
        
        void set(std::string_view newValue, RespType newType) {
            value = newValue;
            type = newType;
        }
};
