#include <vector>
#include <cstddef>
#include <string>

#include "types.hpp"

std::vector<RespToken> parse_resp(std::string_view buf, std::size_t length) {
    std::vector<RespToken> res;
    std::size_t i = 0;

    while (i < length) {
        std::size_t lineEnd = buf.find("\r\n", i);
        if (lineEnd == std::string_view::npos || lineEnd >= length) {
            break;
        }

        std::size_t token_len = lineEnd - (i + 1);

        switch (buf[i]) {
            case '+': {
                std::string value = std::string(buf.substr(i + 1, token_len));
                res.push_back(RespToken{ value, RespType::SIMPLE_STR });
                i = lineEnd + 2;
                break;
            }

            case '-': {
                std::string value = std::string(buf.substr(i + 1, token_len));
                res.push_back(RespToken{ value, RespType::SIMPLE_ERR });
                i = lineEnd + 2; 
                break;
            }

            case ':': {
                std::string value = std::string(buf.substr(i + 1, token_len));
                res.push_back(RespToken{ value, RespType::INTEGERS });
                i = lineEnd + 2; 
                break;
            }

            case '$': {
                std::string length_str = std::string(buf.substr(i + 1, token_len));
                int bulk_length = std::stoi(length_str);

                if (bulk_length == -1) {
                    res.push_back(RespToken{ "", RespType::NULL_BULK_STR });
                    i = lineEnd + 2;
                    break;
                }

                std::size_t content_start = lineEnd + 2;
                
                if (content_start + bulk_length > length) {
                    return res;
                }

                std::string content = std::string(buf.substr(content_start, bulk_length));
                res.push_back(RespToken{ content, RespType::BULK_STR });

                i = content_start + bulk_length + 2;
                break;
            }

            default:
                i++;
                break;
        }
    }

    return res;
}
