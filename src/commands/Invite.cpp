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

	Client &invited = getClient(message.parameters[0]);
	if (!channelExist(message.parameters[1])) {
		sendError(401, client, message, message.parameters[1]);
		return ;
	}

	Channel &channel = getChannel(message.parameters[1]);
	if (!channel.isInChannel(client.getNickname())) {
		sendError(442, client, message, message.parameters[1]);
		return ;
	}
	if (channel.isInChannel(invited.getNickname())) {
		sendError(443, client, message, message.parameters[0] + " " + message.parameters[1]);
		return ;
	}
	if (!channel.isOperator(client.getNickname())) {
		sendError(482, client, message, message.parameters[1]);
		return ;
	}

	channel.addRegistered(invited.getNickname());
	std::string res = ":" + invited.getSource() + " 341 " + client.getNickname() + " " + invited.getNickname() + " " +  channel.getName() + CRLF;
	std::string invite = ":" + client.getSource() + " INVITE " + invited.getNickname() + " " + channel.getName() + CRLF;
	send(client.getSocket(), res.c_str(), res.length(), 0);
	send(invited.getSocket(), invite.c_str(), invite.length(), 0);
}