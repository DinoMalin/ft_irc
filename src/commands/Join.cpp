#include "Server.hpp"

void Server::handleJOIN(Client &client, Message message) {
	if (message.parameters.size() < 1) {
		sendError(461, client, message, "");
		return ;
	}

	std::vector<std::string>channelNames = split(message.parameters[0], ',');

	for (size_t i = 0; i < channelNames.size(); i++) {
		if (channelNames[i][0] != '#')
			channelNames[i] = '#' + channelNames[i];
		if (!channelExist(channelNames[i])) {
			Channel *newChannel = new Channel(channelNames[i]);
			newChannel->addClient(client.getNickname());
			newChannel->addOperator(client.getNickname());
			newChannel->addRegistered(client.getNickname());
			_channels.push_back(newChannel);
			_allChannels.push_back(newChannel);
			std::string broadcast = ":" + client.getSource() + " JOIN :" + newChannel->getName() + CRLF;
			std::string res2 = ":" + std::string(ADDRESS) + " 353 " + client.getNickname() + " = " + newChannel->getName() + " :" + newChannel->getUserList() + CRLF;
			std::string res3 = ":" + std::string(ADDRESS) + " 366 " + client.getNickname() + " " + newChannel->getName() + " :End of /NAMES list" + CRLF;
			newChannel->sendChannel(broadcast, client, _clients, false);
			send(client.getSocket(), res2.c_str(), res2.length(), 0);
			send(client.getSocket(), res3.c_str(), res3.length(), 0);
		}
		else {
			Channel &channel = getChannel(channelNames[i]);
			if (channel.getPassword().length() && message.parameters.size() < 2 && !channel.isRegistered(client.getNickname())) {
				sendError(475, client, message, channelNames[i]);
				return ;
			}
			if (channel.getPassword().length() && channel.getPassword() != message.parameters[1] && !channel.isRegistered(client.getNickname())) {
				sendError(475, client, message, channelNames[i]);
				return ;
			}
			if (channel.isInviteOnly() && !channel.isRegistered(client.getNickname())) {
				sendError(473, client, message, channelNames[i]);
				return ;
			}
			if (channel.isLimited() && channel.getLimitUsers() <= channel.getNbClients()) {
				sendError(471, client, message, channelNames[i]);
				return ;
			}
			channel.addClient(client.getNickname());
			channel.addRegistered(client.getNickname());
			std::string broadcast = ":" + client.getSource() + " JOIN :" + channel.getName() + CRLF;
			std::string res2 = ":" + std::string(ADDRESS) + " 353 " + client.getNickname() + " = " + channel.getName() + " :" + channel.getUserList() + CRLF;
			std::string res3 = ":" + std::string(ADDRESS) + " 366 " + client.getNickname() + " " + channel.getName() + " :End of /NAMES list" + CRLF;
			channel.sendChannel(broadcast, client, _clients, false);
			send(client.getSocket(), res2.c_str(), res2.length(), 0);
			send(client.getSocket(), res3.c_str(), res3.length(), 0);
		}
	}
}