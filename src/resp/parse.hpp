#pragma once

#include <string>
#include <cstddef>

struct RespToken;

std::vector<RespToken> parse_resp(std::string_view buf, std::size_t length);
