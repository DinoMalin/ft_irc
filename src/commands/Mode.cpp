#include "Server.hpp"

void Server::handleMODE(Client &client, Message message) {
	if (channelExist(message.parameters[0]) && message.parameters.size() == 1)
		return ;

	if (message.parameters.size() < 2) {
		sendError(461, client, message, "");
		return ;
	}
    if (clientExist(message.parameters[0])) {
        return ;
    }
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
	bool flag[127] = {};

	for (size_t  i = 1; i < message.parameters[1].length(); i++)
		flag[int(message.parameters[1][i])] = i;

	if (operation != '+' && operation != '-') {
		sendError(472, client, message, std::string() + message.parameters[1][1]);
		return ;
	}

	size_t nbParamsNecessary = 2;
	if (flag['k'])
		nbParamsNecessary++;
	if (flag['o'])
		nbParamsNecessary++;
	if (flag['l'])
		nbParamsNecessary++;
	if (message.parameters.size() < nbParamsNecessary) {
		sendError(461, client, message, "");
		return ;	
	}

	if (flag['i']) {
		if (operation == '+')
			channel.setInviteOnly(true);
		else
			channel.setInviteOnly(false);
	} else if (flag['t']) {
		if (operation == '+')
			channel.setCanUseTopic(true);
		else
			channel.setCanUseTopic(false);
	} else if (flag['k']) {
		if (operation == '+') {
			if (channel.getPassword().length()) {
				sendError(467, client, message, channel.getName());
				return ;
			} else
				channel.setPassword(message.parameters[1 + flag['k']]);
		} else
			channel.setPassword("");
	} else if (flag['o']) {
			if (operation == '+') {
				channel.addOperator(&getClient(message.parameters[1 + flag['o']]));
			}
			else
				channel.removeOperator(getClient(message.parameters[1 + flag['o']]).getNickname());
	} else if (flag['l']) {  
		if (operation == '+') {
			channel.setLimited(true);
			channel.setLimitUsers(std::atoi(message.parameters[1 + flag['l']].c_str()));
		}
		else
			channel.setLimited(false);
	}

	std::string res = ":" + client.getSource() + " MODE " + channel.getName() + " " + message.parameters[1] + CRLF;
	send(client.getSocket(), res.c_str(), res.length(), 0);
}
