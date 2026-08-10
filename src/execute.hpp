#pragma once

#include <string>
#include <vector>

class HashTable;
struct RespToken;

std::string execute_command(
    const std::vector<RespToken>& tokens,
    HashTable* db
);
