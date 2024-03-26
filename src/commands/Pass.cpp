#include "Server.hpp"

void Server::handlePASS(Client &client, Message message) {
	client.tryToRegister();
	if (message.parameters.size() == 0) {
		sendError(461, client, message, "");
		client.quitting();
		return ;
	}
	if (client.getRegistered() && client.getNickname().length() && client.getUsername().length() && client.getHostname().length()) {
		sendError(462, client, message, "");
		client.quitting();
		return ;
	}
	if (message.parameters[0] != _password) {
		sendError(464, client, message, "");
		client.quitting();
		return ;
	}

	client.setRegistered();
}