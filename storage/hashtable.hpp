#include "entry.hpp"

class HashTable {
    private:
        static constexpr int bucketsAmount = 1028;
        std::array<Entry*, bucketsAmount> buckets{};

        uint64_t hash_func(std::string_view s) { // fnv-1a
            uint64_t hash = 14695981039346656037ull;
            for (char c : s) {
                hash ^= static_cast<uint8_t>(c);
                hash *= 1099511628211ull;
            }
            return hash;
        }

    public:
        Entry* get(std::string_view key) {
            Entry* node = buckets[hash_func(key) % bucketsAmount];

            while(node != nullptr) {
                if (node->key == key)
                    return node;
                node = node->next;
            }
            return nullptr;
        }

        void set(std::string_view key, std::string_view value) {
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

        // void erase(std::string_view key);
};


