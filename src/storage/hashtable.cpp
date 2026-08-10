#include <string_view>
#include <cstdint>
#include <array>

#include "entry.hpp"
#include "hashtable.hpp"

uint64_t HashTable::hash_func(std::string_view s) { // fnv-1a
    uint64_t hash = 14695981039346656037ull;
    for (char c : s) {
        hash ^= static_cast<uint8_t>(c);
        hash *= 1099511628211ull;
    }
    return hash;
}

Entry* HashTable::get(std::string_view key) {
    Entry* node = buckets[hash_func(key) % bucketsAmount];

    while(node != nullptr) {
        if (node->key == key)
            return node;
        node = node->next;
    }
    return nullptr;
}

void HashTable::set(std::string_view key, std::string_view value) {
    Entry* node = get(key);

    if (node != nullptr) {
        node->value = value;
        return;
    }

    size_t index = hash_func(key) % bucketsAmount;
    Entry* new_node = new Entry();
    new_node->key = std::string(key);
    new_node->value = std::string(value);
        
    new_node->next = buckets[index];
    buckets[index] = new_node;
}

int HashTable::del(std::string_view key) {
    size_t index = hash_func(key) % bucketsAmount;
    Entry* node = buckets[index];

    if (node == nullptr) return 0;

    if (node->key == key) { // if remove a first element of a linked list
        buckets[index] = node->next;
        delete node;
        return 1;
    }

    Entry* prev;
    while (node != nullptr) { // searching for a correct node in a linked list
        if (node->key == key) { // if its the one we are looking for
            prev->next = node->next;
            delete node;
        }

        prev = node;
        node = node->next;
    }

    return 0;
}


