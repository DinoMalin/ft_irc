#include "Server.hpp"

void Server::handleMODE(Client &client, Message message) {
	if (message.parameters.size() < 1) {
		sendError(461, client, message, "");
		return ;
	}
	if ((channelExist(message.parameters[0]) && message.parameters.size() == 1)
		|| clientExist(message.parameters[0]))
		return ;
	if (!channelExist(message.parameters[0])) {
		sendError(403, client, message, message.parameters[0]);
		return ;
	}
	if (message.parameters[1].length() < 2) {
		sendError(472, client, message, std::string() + message.parameters[1][1]);
		return ;
	}


	Channel &channel = getChannel(message.parameters[0]);
	if (!channel.isInChannel(client.getNickname())) {
		sendError(442, client, message, channel.getName());
		return ;
	}
	if (!channel.isOperator(client.getNickname())) {
		sendError(482, client, message, channel.getName());
		return ;
	}

	int nbParamsNecessary = 0;
	for (size_t i = 0; i < message.parameters[1].length(); i++) {
		if (std::string("kol").find(message.parameters[1][i]) != std::string::npos)
			nbParamsNecessary++;
		if (std::string("itkol+-").find(message.parameters[1][i]) == std::string::npos) {
			sendError(472, client, message, std::string("") + message.parameters[1][i]);
			return ;
		}
		if (message.parameters[1][i] == 'o' && !clientExist(message.parameters[1 + nbParamsNecessary])) {
			sendError(401, client, message, message.parameters[1 + nbParamsNecessary]);
			return;
		}
	}

	if (size_t(nbParamsNecessary + 2) < message.parameters.size()) {
		sendError(461, client, message, "");
		return ;
	}

	char operation = 0;
	int argIndex = 2;
	for (size_t i = 0; i < message.parameters[1].length(); i++) {
		switch(message.parameters[1][i]) {
			case '+':
				operation = '+';
				break;
			case '-':
				operation = '-';
				break;
			case 'i':
				if (operation == '+')
					channel.setInviteOnly(true);
				else
					channel.setInviteOnly(false);
				break;
			case 't':
				if (operation == '+')
					channel.setCanUseTopic(true);
				else
					channel.setCanUseTopic(false);
				break;
			case 'k':
				if (operation == '+')
					channel.setPassword(message.parameters[argIndex++]);
				else
					channel.setPassword("");
				break;
			case 'o':
				if (operation == '+')
					channel.addOperator(getClient(message.parameters[argIndex++]).getNickname());
				else
					channel.removeOperator(getClient(message.parameters[argIndex++]).getNickname());
				break;
			case 'l':
				if (operation == '+')
					channel.setLimitUsers(std::atoi(message.parameters[argIndex++].c_str()));
				else
					channel.setLimited(false);
				break;
		}
	}

	std::string res = ":" + client.getSource() + " MODE " + channel.getName() + " " + message.parameters[1] + CRLF;
	channel.sendChannel(res, client, _clients, false);
}
