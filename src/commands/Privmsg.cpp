#include "Server.hpp"

void Server::handlePRIVMSG(Client &client, Message message) {
	std::string res;

	if ((message.parameters.size() < 1) || (message.parameters.size() == 1 && message.remainder.length())) {
		sendError(411, client, message, "");
		return ;
	}
	if (!message.remainder.length()) {
		sendError(412, client, message, "");
		return ;
	}

	for (size_t i = 0; i < message.parameters.size() - 1; i++) {
		bool keepGoing = false;
		for (size_t j = 0; j < _channels.size(); j++) {
			if (_channels[j]->getName() == message.parameters[i])
				keepGoing = true;
		}
		for (int j = 0; j < _numClients; j++) {
			if (_clients[j].getNickname() == message.parameters[i])
				keepGoing = true;
		}
		if (!keepGoing) {
			sendError(401, client, message, message.parameters[i]);
			return ;
		}
	}

	for (size_t i = 0; i < message.parameters.size(); i++) {
		if (channelExist(message.parameters[i])) {
			Channel channel = getChannel(message.parameters[i]);
			std::string res = ":" + client.getNickname() + " PRIVMSG " + channel.getName() + " " + message.remainder + CRLF;
            channel.sendChannel(res, client, true);
			continue;
		}
		if (clientExist(message.parameters[i])) {
			Client clientTarget = getClient(message.parameters[i]);
			std::string res = ":" + client.getNickname() + " PRIVMSG " + clientTarget.getNickname() + " " + message.remainder + CRLF;
			send(client.getSocket(), res.c_str(), res.length(), 0);
			continue;
		}
	}
}