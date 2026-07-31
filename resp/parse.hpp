
#include "types.hpp"

std::vector<RespToken> parse_resp(std::string_view buf, std::size_t length) {
    std::vector<RespToken> res;
    std::size_t i = 0;

    while (i < length) {
        // Захист від виходу за межі: перевіряємо, чи є взагалі CRLF
        std::size_t lineEnd = buf.find("\r\n", i);
        if (lineEnd == std::string_view::npos || lineEnd >= length) {
            break; // Неповні дані або кінець буфера
        }

        // Обчислюємо чисту довжину рядка без урахування префікса (+, -, :) та без \r\n
        std::size_t token_len = lineEnd - (i + 1);

        switch (buf[i]) {
            case '+': {
                std::string value = std::string(buf.substr(i + 1, token_len));
                res.push_back(RespToken{ value, RespType::SIMPLE_STR });
                i = lineEnd + 2; // Перестрибуємо поточний рядок та його \r\n
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
                    i = lineEnd + 2; // Перестрибуємо тільки $-1\r\n
                    break;
                }

                std::size_t content_start = lineEnd + 2;
                
                // Захист: перевіряємо, чи весь контент Bulk String фізично присутній у буфері
                if (content_start + bulk_length > length) {
                    return res; // Дані не повні, припиняємо парсинг
                }

                std::string content = std::string(buf.substr(content_start, bulk_length));
                res.push_back(RespToken{ content, RespType::BULK_STR });

                // Перестрибуємо: префікс з довжиною + контент + кінцевий \r\n контенту (2 байти)
                i = content_start + bulk_length + 2;
                break;
            }

            default:
                // Якщо зустріли невідомий символ або зайвий \r\n між токенами
                i++;
                break;
        }
    }

    return res;
}
