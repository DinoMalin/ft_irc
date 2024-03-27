#include "header.hpp"
#include "Server.hpp"

int main(int ac, char **av) {
    if (ac != 3) {
        std::cout << "Usage: ./ircserv <password> <port>" << std::endl;
        return 0;
    }

	Server server(av[1], std::atoi(av[2]));
    server.run();

	return 0;
}
