#include "Server.hpp"

void Server::handlePART(Client &client, Message message) {
	if (message.parameters.size() < 1) {
		sendError(461, client, message, "");
		return ;
	}
	if (!channelExist(message.parameters[0])) {
		sendError(403, client, message, message.parameters[0]);
		return ;
	}

	Channel &channel = getChannel(message.parameters[0]);
	if (!channel.isInChannel(client.getNickname())) {
		sendError(402, client, message, message.parameters[0]);
		return ;
	}

	std::string res = ":" + client.getSource() + " PART " + channel.getName() + " :" + client.getNickname() + CRLF;
	channel.sendChannel(res, client, _clients, false);
	channel.removeClient(client.getNickname());

	if (channel.getNbClients() == 0) {
		for (size_t i = 0; i < _channels.size(); i++) {
			if (_channels[i]->getName() == channel.getName())
				_channels.erase(_channels.begin() + i);
		}
	}
}