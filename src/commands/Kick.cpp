#include "Server.hpp"

void Server::handleKICK(Client &client, Message message) {
	std::string res;

	if (message.parameters.size() < 2) {
		sendError(461, client, message, "");
		return ;
	}
	if (!channelExist(message.parameters[0])) {
		sendError(403, client, message, message.parameters[0]);
		return ;
	}

	Channel channel = getChannel(message.parameters[0]);
	if (!channel.isInChannel(client.getNickname()) || !clientExist(message.parameters[1])
		|| !channel.isInChannel(getClient(message.parameters[1]).getNickname())) {
		sendError(442, client, message, message.parameters[0]);
		return ;
	}
	if (!channel.isOperator(client.getNickname())) {
		sendError(482, client, message, message.parameters[0]);
		return ;
	}

	getChannel(message.parameters[0]).removeClient(getClient(message.parameters[1]).getNickname());
	getChannel(message.parameters[0]).removeRegistered(getClient(message.parameters[1]).getNickname());
	
	res = ":" + std::string(ADDRESS) + " KICK " + message.parameters[0] + " " + message.parameters[1] + CRLF;
	std::string kick = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + " KICK "
		+ message.parameters[0] + " " + message.parameters[1] + CRLF;
	if (message.parameters.size() == 3) {
		res += " :" + message.remainder;
		kick += " :" + message.remainder;
	}
	send(client.getSocket(), res.c_str(), res.length(), 0);
	send(getClient(message.parameters[1]).getSocket(), kick.c_str(), kick.length(), 0);
}