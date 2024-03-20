#include "Server.hpp"

void Server::handleINVITE(Client &client, Message message) {
	if (message.parameters.size() < 2) {
		sendError(461, client, message, "");
		return ;
	}
	if (!clientExist(message.parameters[0])) {
		sendError(401, client, message, message.parameters[0]);
		return ;
	}
	if (!channelExist(message.parameters[1])) {
		sendError(401, client, message, message.parameters[1]);
		return ;
	}

	Channel channel = getChannel(message.parameters[1]);
	if (!channel.isInChannel(client.getNickname())) {
		sendError(442, client, message, message.parameters[1]);
		return ;
	}
	if (!channel.isInChannel(message.parameters[0])) {
		sendError(443, client, message, message.parameters[0] + message.parameters[1]);
		return ;
	}
	if (!channel.isOperator(client.getNickname())) {
		sendError(482, client, message, message.parameters[1]);
		return ;
	}

	getChannel(message.parameters[1]).addRegistered(&getClient(message.parameters[0]));
	std::string res = ":" + std::string(ADDRESS) + " 341 " + client.getNickname() + " " + message.parameters[1] + " "
		+ client.getNickname() + CRLF;
	std::string invite = ":" + std::string(ADDRESS) + "PRIVMSG " + message.parameters[0] + " :You have been invited to "
		+ message.parameters[1] + CRLF;
	send(client.getSocket(), res.c_str(), res.length(), 0);
	send(getClient(message.parameters[0]).getSocket(), invite.c_str(), invite.length(), 0);
}