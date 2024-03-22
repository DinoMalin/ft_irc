#include "Server.hpp"

void Server::handleJOIN(Client &client, Message message) {
	if (message.parameters.size() < 1) {
		sendError(461, client, message, "");
		return ;
	}
	if (!channelExist(message.parameters[0])) {
		Channel *newChannel = new Channel(message.parameters[0], &client);
		_channels.push_back(newChannel);
		_allChannels.push_back(newChannel);
		std::string res = ":" + std::string(ADDRESS) + " 332 " + client.getNickname() + " " + newChannel->getName() + CRLF;
		send(client.getSocket(), res.c_str(), res.length(), 0);
	}
	else {
		Channel channel = getChannel(message.parameters[0]);
		if (channel.getPassword().length() && message.parameters.size() < 2 && !channel.isRegistered(client.getNickname())) {
			sendError(475, client, message, message.parameters[0]);
			return ;
		}
		if (channel.getPassword().length() && channel.getPassword() != message.parameters[1] && !channel.isRegistered(client.getNickname())) {
			sendError(475, client, message, message.parameters[0]);
			return ;
		}
		if (channel.isInviteOnly() && !channel.isRegistered(client.getNickname())) {
			sendError(473, client, message, message.parameters[0]);
			return ;
		}
		if (channel.isLimited() && channel.getLimitUsers() >= channel.getNbClients()) {
			sendError(471, client, message, message.parameters[0]);
			return ;
		}
		getChannel(message.parameters[0]).addClient(&client);
		getChannel(message.parameters[0]).addRegistered(&client);
		std::string res = ":" + std::string(ADDRESS) + " 332 " + client.getNickname() + " " + channel.getName() + " :" + channel.getTopic() + CRLF;
		send(client.getSocket(), res.c_str(), res.length(), 0);
	}
}