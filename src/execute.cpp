#include <cstdint>
#include <string>
#include <functional>

#include "resp/types.hpp"
#include "storage/hashtable.hpp"
#include "storage/entry.hpp"

std::string PING_command (std::vector<RespToken>, HashTable*) {
    return "+PONG";
}

std::string GET_command (std::vector<RespToken> tokens, HashTable* db) {
    if (tokens.size() != 2)
        return "-error: wrong number of arguments for 'get' command";

    Entry* node = db->get(tokens[1].value);
    if (node == nullptr)
        return "$-1";

    return "+" + node->value;
}

std::string SET_command (std::vector<RespToken> tokens, HashTable* db) {
    uint8_t size = tokens.size();
    if (size < 3)
        return "-error: wrong number of arguments for 'set' command";

    else if (size > 3)
        return "-error: syntax error";

    std::string_view key = tokens[1].value;
    std::string_view val = tokens[2].value;
  
    db->set(key, val);

    return "+OK";
}

std::string DEL_command(std::vector<RespToken> tokens, HashTable* db) {
    if (tokens.size() != 2)
        return "-error: wrong number of arguments for 'del' command";

    return ":" + std::to_string(db->del(tokens[1].value));
}

struct ExecCommand {
    std::string command;
    std::function<std::string (const std::vector<RespToken>&, HashTable* db)> func; 
};

struct ExecCommand execute[] = {
    { "PING", PING_command },
    { "ping", PING_command },
    
    { "GET", GET_command },
    { "SET", SET_command },
    { "DEL", DEL_command }
};
uint8_t exec_len = std::size(execute);

std::string execute_command(const std::vector<RespToken>& tokens, HashTable* db) {
    if (tokens.size() < 1)
        return "-error: empty command";

    for (int i = 0; i < exec_len; i++) {
        if (execute[i].command == tokens[0].value)
            return execute[i].func(tokens, db);
    }
    return "-error: uknown command";
}
