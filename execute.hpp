std::string SET_command (std::vector<RespToken> tokens, HashTable* db) {
    uint8_t size = tokens.size();
    if (size < 3)
        return "-error: wrong number of arguments for 'set' command\r\n";

    else if (size > 3)
        return "-error: syntax error";

    std::string_view key = tokens[1].value;
    std::string_view val = tokens[2].value;
  
    db->set(key, val);

    return "+OK\r\n";
}

std::string GET_command (std::vector<RespToken> tokens, HashTable* db) {
    if (tokens.size() != 2)
        return "-error: wrong number of arguments for 'get' command";

    Entry* node = db->get(tokens[1].value);
    if (!node)
        return "$-1\r\n";

    return "+" + node->value + "\r\n";
}

struct ExecCommand {
    std::string command;
    std::function<std::string (const std::vector<RespToken>&, HashTable* db)> func; 
};

struct ExecCommand execute[] = {
    { "GET", GET_command },
    { "SET", SET_command }
};
uint8_t exec_len = 2;

std::string execute_command(std::vector<RespToken> tokens, HashTable* db) {
    if (tokens.size() < 1)
        return "-error: empty command\r\n";

    for (int i = 0; i < exec_len; i++) {
        if (execute[i].command == tokens[0].value)
            return execute[i].func(tokens, db);
    }
    return "-error: uknown command\r\n";
}
