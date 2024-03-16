#include "header.hpp"
#include "Server.hpp"

// int main () {
// 	Message res = getParsedCommand(":doctor!doctor@baz.example.org PRIVMSG rory :Hey Rory, I'm leaving...\r\n");

// 	std::cout << "PREFIX\t\t: " << res.prefix << std::endl;
// 	std::cout << "CMD\t\t: " << res.command << std::endl;
// 	for (size_t i = 0; i < res.commandParameters.size(); i++) {
// 		std::cout << "ARG[" << i << "]\t\t: " << res.commandParameters[i] << std::endl;
// 	}
// 	std::cout << "REMAINDER\t: " << res.remainder << std::endl;
// 	std::cout << "FULL CMD\t: " << res.fullCmd << std::endl;
// }

int main() {
	Server server;
    server.run();

	return 0;
}
