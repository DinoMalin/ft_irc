#include "Server.hpp"

void Server::handleUSER(Client &client, Message message) {
	if (!client.getRegistered())
		return ;
	if (client.getNickname().length() && client.getUsername().length() && client.getHostname().length()) {
		sendError(462, client, message, "");
		return ;
	}
	if (message.parameters.size() < 4) {
		sendError(461, client, message, "");
		return ;
	}

	client.setUsername(message.parameters[0]);
	client.setHostname(message.parameters[1]);
	client.setUsered();
}