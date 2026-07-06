std::string execute_command(std::vector<RespToken> tokens) {
    std::string res;
    switch (tokens[0].type) {
        case RespType::BULK_STR:
            if (tokens[0].value == "GET") {
                return "$12\r\nhello world!\r\n";
            }
            else
                return "-Error: unknown command: " + tokens[0].value + "\r\n";
            break;

        default:
            return "-Error: unknown command\r\n";
            break;
    }

    return res;
}
