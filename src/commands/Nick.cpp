#include "Server.hpp"

static bool authorizedChar(std::string str) {
	for (size_t i = 0; i < str.length(); i++) {
		if (!((str[i] >= 'a' && str[i] <= 'z')
			|| (str[i] >= '0' && str[i] <= '9')
			|| (str[i] >= 'A' && str[i] <= 'Z')
			|| str[i] == '[' || str[i] == ']'
			|| str[i] == '{' || str[i] == '}'
			|| str[i] == '[' || str[i] == ']'
			|| str[i] == '\\' || str[i] == '|'
			|| str[i] == '_'))
			return false;
	}
	return true;
}

void Server::handleNICK(Client &client, Message message) {
	if (message.parameters.size() == 0) {
		sendError(431, client, message, "");
		return ;
	}
	if (client.getNickname() == message.parameters[0])
		return ;
	if (!authorizedChar(message.parameters[0]) || message.remainder != "" || message.parameters[0].size() > 10) {
		sendError(432, client, message, message.parameters[0]);
		return ;
	}

	std::string newNick(message.parameters[0]);
	if (clientExist(newNick)) {
		sendError(433, client, message, newNick);
		return ;
	}
	if (client.getUsername().length()) {
		std::string res = ":" + client.getSource() + " NICK " + newNick + CRLF;
		send(client.getSocket(), res.c_str(), res.length(), MSG_DONTWAIT | MSG_NOSIGNAL);
	}
	for (size_t i = 0; i < _channels.size(); i++) {
		if (_channels[i].isInChannel(client.getNickname()))
			_channels[i].renameClient(client.getNickname(), newNick);
	}
	client.setNickname(newNick);
	client.setNicked();
}
