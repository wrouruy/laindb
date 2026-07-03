#include <iostream>
#include <vector>
#define ASIO_STANDALONE
#include <asio.hpp>

using asio::ip::tcp;

int main() {
    try {
        asio::io_context io_context;

        // create listener on port 8080 for ipv4
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));
        std::cout << "Server is listening on port 8080..." << std::endl;

        // server just returns the same client's own data, echo yet
        while(1) {
            
            // create socket for further client
            tcp::socket socket(io_context);

            // wait for connection
            acceptor.accept(socket);
            std::cout << "Client connected successfully!" << std::endl;

            // buffer for reading client data
            std::vector<char> buffer(1024);
            asio::error_code error;

            // read client data
            size_t length = socket.read_some(asio::buffer(buffer), error);

            if (!error) {
                std::cout << "Received from client: ";
                std::cout.write(buffer.data(), length) << std::endl;

                // send this data back, echo
                asio::write(socket, asio::buffer(buffer, length));
            } else if (error != asio::error::eof) {
                throw asio::system_error(error);
            }

        }

        std::cout << "Server shutting down." << std::endl;

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
