#include "header.hpp"
#include "Server.hpp"

Server server;

void sigInt(int code) {
    if (code == SIGINT)
        server.kill();
    exit(SIGINT);
}

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

    signal(SIGINT, &sigInt);

	server.init(std::atoi(av[1]), av[2]);
    server.run();

	return 0;
}
