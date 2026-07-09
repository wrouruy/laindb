#define ASIO_STANDALONE
#include <asio.hpp>
#include <iostream>
#include <memory>
#include "resp/types.hpp"
#include "resp/parse.hpp"
#include "execute.hpp"

using asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(tcp::socket socket)
        : socket_(std::move(socket))
    {
    }

    void start()
    {
        std::cout << "Client connected!\n";
        read();
    }

private:
    void read()
    {
        auto self = shared_from_this();

        socket_.async_read_some(
            asio::buffer(buffer_),
            [this, self](std::error_code ec, std::size_t length)
            {
                if (ec)
                {
                    std::cout << "Client disconnected\n";
                    return;
                }
                
                const char* buf = buffer_.data();

                std::vector<RespToken> values = parse_resp(buf, length);

                const char* enum2str[] = { "SIMPLE_STR", "SIMPLE_ERR", "INTEGER", "BULK_STR" };

                std::string response = execute_command(values);
                // for (int i = 0; i < resp.size(); i++) {
                    
                //     std::cout << "type: " << enum2str[static_cast<int>(resp[i].type)] << ", value: " << resp[i].value << std::endl;
                // }
                std::cout << response << std::endl;
 
                asio::async_write(
                    socket_,
                    asio::buffer(response),
                    [this, self](std::error_code ec, std::size_t)
                    {
                        if (!ec)
                            read();
                    });
            });
    }

    tcp::socket socket_;
    std::array<char, 1024> buffer_;
};

int main()
{
    asio::io_context io;

    tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 8080));

    std::function<void()> accept;

    accept = [&]()
    {
        acceptor.async_accept(
            [&](std::error_code ec, tcp::socket socket)
            {
                if (!ec)
                    std::make_shared<Session>(std::move(socket))->start();

                accept();
            });
    };

    accept();

    io.run();
}
