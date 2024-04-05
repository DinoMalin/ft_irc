#include "Server.hpp"

void Server::handlePING(Client &client, Message message) {
	if (message.parameters.size() == 0) {
		sendError(461, client, message, "");
		return ;
	}
	std::string res = ":" + std::string(ADDRESS) + " PONG " + std::string(ADDRESS) + " :" + message.parameters[0] + CRLF;
	send(client.getSocket(), res.c_str(), res.length(), MSG_DONTWAIT | MSG_NOSIGNAL);
}