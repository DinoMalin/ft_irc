#include "Server.hpp"

void Server::handleNICK(Client &client, Message message) {
	std::string res;

	if (message.parameters.size() == 0) {
		sendError(431, client, message, "");
		return ;
	}

	std::string newNick(message.parameters[0]);
	if (clientExist(newNick)) {
		sendError(433, client, message, newNick);
		return ;
	}

	if (client.getUsername().length()) {
		std::string res1 = ":" + std::string(ADDRESS) + " 001 " + client.getNickname() + CRLF;
		send(client.getSocket(), res.c_str(), res.length(), 0);
	}
	client.setNickname(newNick);
}