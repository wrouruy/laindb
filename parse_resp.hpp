std::vector<RespToken> parse_resp(std::string buf, std::size_t length) {
    std::vector<RespToken> res;
    for (std::size_t i = 0; i < length; i++) {
        std::size_t lineEnd = buf.find("\r\n", i);
        switch (buf[i]) {
            case '+':
                res.push_back({
                    buf.substr(i + 1, lineEnd),
                    RespType::SIMPLE_STR
                });
                i += lineEnd;
                break;

            case '-':
                res.push_back({
                    buf.substr(i + 1, lineEnd),
                    RespType::SIMPLE_ERR
                });
                i += lineEnd;
                break;

            case ':':
                res.push_back({
                    buf.substr(i + 1, lineEnd),
                    RespType::INTEGERS
                });
                i += lineEnd;
                break;

            case '$':
                std::string length_str = buf.substr(i + 1, lineEnd - (i + 1));
                int length = std::stoi(length_str);

                size_t content_start = lineEnd + 2;
                std::string content = buf.substr(content_start, length);

                res.push_back({
                    content,
                    RespType::BULK_STR
                });
                break;
        }
    }

    return res;
}

