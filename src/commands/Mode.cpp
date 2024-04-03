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

	for (size_t i = 1; i < message.parameters[1].length(); i++) {
		if (std::string("itkol").find(message.parameters[1][i]) == std::string::npos) {
			sendError(472, client, message, std::string() + message.parameters[1][i]);
			return ;
		}
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

	char operation = message.parameters[1][0];
	int flag[127] = {};

	for (size_t  i = 1; i < message.parameters[1].length(); i++) {
		flag[int(message.parameters[1][i])] = i;
	}

	if (operation != '+' && operation != '-') {
		sendError(472, client, message, std::string() + message.parameters[1][1]);
		return ;
	}

	size_t nbParamsNecessary = 2;
	if (flag[int('k')]  && operation == '+')
		nbParamsNecessary++;
	if (flag[int('o')])
		nbParamsNecessary++;
	if (flag[int('l')] && operation == '+')
		nbParamsNecessary++;
	if (message.parameters.size() < nbParamsNecessary) {
		sendError(461, client, message, "");
		return ;	
	}

	if (flag[int('i')]) {
		if (operation == '+')
			channel.setInviteOnly(true);
		else
			channel.setInviteOnly(false);
	}
	if (flag[int('t')]) {
		if (operation == '+')
			channel.setCanUseTopic(true);
		else
			channel.setCanUseTopic(false);
	}
	if (flag[int('k')]) {
		if (operation == '+')
			channel.setPassword(message.parameters[1 + flag[int('k')]]);
		else
			channel.setPassword("");
	}
	if (flag[int('o')]) {
		if (operation == '+')
			channel.addOperator(&getClient(message.parameters[1 + flag[int('o')]]));
		else
			channel.removeOperator(getClient(message.parameters[1 + flag[int('o')]]).getNickname());
	}
	if (flag[int('l')]) {  
		if (operation == '+')
			channel.setLimitUsers(std::atoi(message.parameters[1 + flag[int('l')]].c_str()));
		else
			channel.setLimited(false);
	}

	std::string res = ":" + client.getSource() + " MODE " + channel.getName() + " " + message.parameters[1] + CRLF;
	channel.sendChannel(res, client, false);
}
