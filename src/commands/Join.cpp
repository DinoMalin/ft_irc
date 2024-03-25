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
		std::string res1 = ":" + std::string(ADDRESS) + " 332 " + client.getNickname() + " " + newChannel->getName() + CRLF;
		std::string res2 = ":" + std::string(ADDRESS) + " 353 " + client.getNickname() + " = " + newChannel->getName() + " :" + newChannel->getUserList() + CRLF;
		std::string res3 = ":" + std::string(ADDRESS) + " 366 " + client.getNickname() + " " + newChannel->getName() + " :End of /NAMES list" + CRLF;
		send(client.getSocket(), res1.c_str(), res1.length(), 0);
		send(client.getSocket(), res2.c_str(), res2.length(), 0);
		send(client.getSocket(), res3.c_str(), res3.length(), 0);
	}
	else {
		Channel &channel = getChannel(message.parameters[0]);
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
		channel.addClient(&client);
		channel.addRegistered(&client);
		std::string res1 = ":" + std::string(ADDRESS) + " 332 " + client.getNickname() + " " + channel.getName() + " :" + channel.getTopic() + CRLF;
		std::string res2 = ":" + std::string(ADDRESS) + " 353 " + client.getNickname() + " = " + channel.getName() + " :" + channel.getUserList() + CRLF;
		std::string res3 = ":" + std::string(ADDRESS) + " 366 " + client.getNickname() + " " + channel.getName() + " :End of /NAMES list" + CRLF;
		std::string broadcast = ":" + client.getSource() + " JOIN :" + channel.getName() + CRLF;
		send(client.getSocket(), res1.c_str(), res1.length(), 0);
		send(client.getSocket(), res2.c_str(), res2.length(), 0);
		send(client.getSocket(), res3.c_str(), res3.length(), 0);
		channel.sendChannel(broadcast, client, true);
	}
}