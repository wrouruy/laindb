#define ASIO_STANDALONE
#include <asio.hpp>
#include <iostream>
#include <string_view>
#include <memory>
#include <vector>
#include <array>

#include "resp/parse.hpp"
#include "storage/hashtable.hpp"
#include "execute.hpp"

using asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(tcp::socket socket)
        : socket_(std::move(socket))
    {
    }

    void start(HashTable *db)
    {
        std::cout << "Client connected!\n";
        read(db);
    }

private:
    void read(HashTable *db)
    {
        auto self = shared_from_this();
        socket_.async_read_some(
            asio::buffer(buffer_),
            [this, self, db](std::error_code ec, std::size_t length) {
                if (ec) {
                    std::cout << "Client disconnected" << std::endl;
                    return;
                }

                std::string buf = buffer_.data();

                std::vector<RespToken> tokens = parse_resp(buf, length);
                std::string response = execute_command(tokens, db) + "\r\n";

                asio::async_write(
                    socket_,
                    asio::buffer(response),
                    [this, self, db](std::error_code ec, std::size_t length) {
                        if (!ec) read(db);
                    });
            }
        );
    };

    tcp::socket socket_;
    std::array<char, 1024> buffer_;
};

int main()
{
    HashTable db;

    asio::io_context io;
    tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 8080));

    std::function<void(HashTable *db)> accept;
    accept = [&](HashTable *db)
    {
        acceptor.async_accept(
            [&, db](std::error_code ec, tcp::socket socket)
            {
                if (!ec)
                    std::make_shared<Session>(std::move(socket))->start(db);

                accept(db);
            });
    };

    accept(&db);
    io.run();
}
