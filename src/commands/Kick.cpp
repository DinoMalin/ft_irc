#include "Server.hpp"

void Server::handleKICK(Client &client, Message message) {
	if (message.parameters.size() < 2) {
		sendError(461, client, message, "");
		return ;
	}
	if (!channelExist(message.parameters[0])) {
		sendError(403, client, message, message.parameters[0]);
		return ;
	}

	Channel &channel = getChannel(message.parameters[0]);
	if (!channel.isInChannel(client.getNickname()) || !clientExist(message.parameters[1])
		|| !channel.isInChannel(getClient(message.parameters[1]).getNickname())) {
		sendError(442, client, message, channel.getName());
		return ;
	}
	if (!channel.isOperator(client.getNickname())) {
		sendError(482, client, message, channel.getName());
		return ;
	}

	Client &ClientTarget = getClient(message.parameters[1]);

	std::string res = ":" + client.getSource() + " KICK " + channel.getName() + " " + ClientTarget.getNickname() + " :" + message.remainder + CRLF;
	channel.sendChannel(res, client, _clients, false);

	channel.removeClient(ClientTarget.getNickname());
	channel.removeRegistered(ClientTarget.getNickname());

	if (channel.getNbClients() == 0) {
		for (size_t i = 0; i < _channels.size(); i++) {
			if (_channels[i].getName() == channel.getName())
				_channels.erase(_channels.begin() + i);
		}
	}
}