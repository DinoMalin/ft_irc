#include "Server.hpp"

static bool authorizedChar(std::string str) {
	for (size_t i = 0; i < str.length(); i++) {
		if (i == 0 && str[i] == '#')
			continue;
		if (!((str[i] >= 'a' && str[i] <= 'z')
			|| (str[i] >= '0' && str[i] <= '9')
			|| (str[i] >= 'A' && str[i] <= 'Z')))
			return false;
	}
	return true;
}

void Server::handleJOIN(Client &client, Message message) {
	if (message.parameters.size() < 1) {
		sendError(461, client, message, "");
		return ;
	}

	std::vector<std::string>channelNames = split(message.parameters[0], ',');
	std::vector<std::string>keys;
	if (message.parameters.size() > 1) {
		keys = split(message.parameters[1], ',');
		if (keys.size() != channelNames.size()) {
			sendError(461, client, message, "");
			return ;
		}
	}

	for (size_t i = 0; i < channelNames.size(); i++) {
		if (channelNames[i][0] != '#')
			channelNames[i] = '#' + channelNames[i];
		if (channelNames[i].length() < 2 || !authorizedChar(channelNames[i])) {
			sendError(476, client, message, channelNames[i]);
			return ;
		}
	}

	for (size_t i = 0; i < channelNames.size(); i++) {
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
			send(client.getSocket(), res2.c_str(), res2.length(), MSG_DONTWAIT | MSG_NOSIGNAL);
			send(client.getSocket(), res3.c_str(), res3.length(), MSG_DONTWAIT | MSG_NOSIGNAL);
		}
		else {
			Channel &channel = getChannel(channelNames[i]);
			std::string key;
			if (keys.size())
				key = keys[i];
			
			if (channel.getPassword().length() && !keys.size() && !channel.isRegistered(client.getNickname())) {
				sendError(475, client, message, channelNames[i]);
				continue ;
			}
			if (channel.getPassword().length() && channel.getPassword() != key && !channel.isRegistered(client.getNickname())) {
				sendError(475, client, message, channelNames[i]);
				continue ;
			}
			if (channel.isInviteOnly() && !channel.isRegistered(client.getNickname())) {
				sendError(473, client, message, channelNames[i]);
				continue ;
			}
			if (channel.isLimited() && channel.getLimitUsers() <= channel.getNbClients()) {
				sendError(471, client, message, channelNames[i]);
				continue ;
			}
			channel.addClient(client.getNickname());
			channel.addRegistered(client.getNickname());
			std::string broadcast = ":" + client.getSource() + " JOIN :" + channel.getName() + CRLF;
			std::string res2 = ":" + std::string(ADDRESS) + " 353 " + client.getNickname() + " = " + channel.getName() + " :" + channel.getUserList() + CRLF;
			std::string res3 = ":" + std::string(ADDRESS) + " 366 " + client.getNickname() + " " + channel.getName() + " :End of /NAMES list" + CRLF;
			channel.sendChannel(broadcast, client, _clients, false);
			send(client.getSocket(), res2.c_str(), res2.length(), MSG_DONTWAIT | MSG_NOSIGNAL);
			send(client.getSocket(), res3.c_str(), res3.length(), MSG_DONTWAIT | MSG_NOSIGNAL);
		}
	}
}