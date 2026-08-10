#pragma once
#include <string_view>

class Entry;

class HashTable {
    private:
        static constexpr int bucketsAmount = 1028;
        std::array<Entry*, bucketsAmount> buckets{};

        uint64_t hash_func(std::string_view s);

    public:
        Entry* get(std::string_view key);
        void set(std::string_view key, std::string_view value);
        int del(std::string_view key);
};


