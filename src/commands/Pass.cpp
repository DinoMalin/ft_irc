#include "Server.hpp"

void Server::handlePASS(Client &client, Message message) {
	if (message.parameters.size() == 0) {
		sendError(461, client, message, "");
		return ;
	}
	if (client.getRegistered() && client.getRegistered() && client.getNickname().length() && client.getUsername().length() && client.getHostname().length()) {
		sendError(462, client, message, "");
		return ;
	}
	if (message.parameters[0] != _password) {
		sendError(464, client, message, "");
		return ;
	}

	std::string res = ":" + std::string(ADDRESS) + " 001 " + client.getNickname() + " :Password correct" + CRLF;
	client.setRegistered();
	send(client.getSocket(), res.c_str(), res.length(), 0);
}