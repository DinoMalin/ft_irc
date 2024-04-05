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

	std::vector<std::string> params = split(message.parameters[0], ',');

	for (size_t i = 0; i < params.size(); i++) {
		if (!clientExist(params[i]) && !channelExist(params[i])) {
			sendError(401, client, message, params[i]);
			return ;
		}
		if (channelExist(params[i]) && !getChannel(params[i]).isInChannel(client.getNickname())) {
			sendError(442, client, message, params[i]);
			return ;
		}
	}

	for (size_t i = 0; i < params.size(); i++) {
		if (channelExist(params[i])) {
			Channel channel = getChannel(params[i]);
			std::string res = ":" + client.getSource() + " PRIVMSG " + channel.getName() + " :" + message.remainder + CRLF;
			channel.sendChannel(res, client, _clients, true);
			continue;
		}
		if (clientExist(params[i])) {
			Client clientTarget = getClient(params[i]);
			std::string res = ":" + client.getSource() + " PRIVMSG " + clientTarget.getNickname() + " :" + message.remainder + CRLF;
			send(clientTarget.getSocket(), res.c_str(), res.length(), MSG_DONTWAIT | MSG_NOSIGNAL);
			continue;
		}
	}
}