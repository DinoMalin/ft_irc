#include "Server.hpp"

void Server::handleTOPIC(Client &client, Message message) {
	if (message.parameters.size() < 1) {
		sendError(461, client, message, "");
		return ;
	}
	if (!channelExist(message.parameters[0])) {
		sendError(403, client, message, message.parameters[0]);
		return ;
	}

	Channel channel = getChannel(message.parameters[0]);
	if (!channel.isInChannel(client.getNickname())) {
		sendError(442, client, message, message.parameters[0]);
		return ;
	}

	if (message.parameters.size() == 2) {
		if (channel.isOperator(client.getNickname()) && channel.getCanUseTopic()) {
			channel.setTopic(message.parameters[1]);
			std::string res = ":" + std::string(ADDRESS) + " TOPIC " + message.parameters[0] + " :"
				+ message.parameters[1] + CRLF;
			channel.sendChannel(res, client, false);
		} else
			sendError(482, client, message, message.parameters[0]);
	} else {
		if (channel.getTopic() != "") {
			std::string res = ":" + std::string(ADDRESS) + " 332 " + client.getNickname() + " " + channel.getName() + " :"
				+ channel.getTopic() + CRLF;
			send(client.getSocket(), res.c_str(), res.length(), 0);
		} else {
			std::string res = ":" + std::string(ADDRESS) + " 331 " + client.getNickname() + " " + channel.getName()
				+ " :No topic is set" + CRLF;
			send(client.getSocket(), res.c_str(), res.length(), 0);
		}
	}
}