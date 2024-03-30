#include "header.hpp"
#include "Server.hpp"

int main(int ac, char **av) {
    if (ac != 3) {
        std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }

    for (int i = 0; av[1][i]; i++) {
        if (!isdigit(av[1][i])) {
            std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
            return 1;
        }
    }

	Server server(std::atoi(av[1]), av[2]);
    server.run();

	return 0;
}
