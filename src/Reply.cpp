#include "Server.hpp"

void Server::handlePING(Client client, Message message) {
	(void) message;
	std::string res = "PONG " + std::string(ADDRESS) + CRLF;
	send(client.getSocket(), res.c_str(), res.length(), 0);
}

void Server::handlePASS(Client client, Message message) {
	if (message.parameters.size() == 0) {
		sendError(461, client, message, "");
		return ;
	}
	if (client.getRegistered() && client.getRegistered() && client.getNickname().length() && client.getUsername().length() && client.getHostname().length()) {
		sendError(462, client, message, "");
		return ;
	}
	if (message.parameters[0] != _password) {
		sendError(464, client, message, "");
		return ;
	}

	std::string res = ":" + std::string(ADDRESS) + " 001 " + client.getNickname() + " :Password correct" + CRLF;
	client.setRegistered();
	send(client.getSocket(), res.c_str(), res.length(), 0);
}

void Server::handleNICK(Client client, Message message) {
	std::string res;

	if (message.parameters.size() == 0) {
		sendError(431, client, message, "");
		return ;
	}

	std::string newNick = message.parameters[0];
	for (int i = 0; i < _numClients; i++) {
		if (_clients[i].getNickname() == newNick) {
			sendError(433, client, message, "");
			return ;
		}
	}

	client.setNickname(newNick);
	res = ":" + std::string(ADDRESS) + " 001 " + client.getNickname() + CRLF;
	send(client.getSocket(), res.c_str(), res.length(), 0);
}

void Server::handleUSER(Client client, Message message) {
	if (client.getRegistered() && client.getRegistered() && client.getNickname().length() && client.getUsername().length() && client.getHostname().length()) {
		sendError(462, client, message, "");
		return ;
	}
	if (message.parameters.size() < 4) {
		sendError(461, client, message, "");
		return ;
	}

	client.setUsername(message.parameters[0]);
	client.setHostname(message.parameters[1]);
	std::string res = ":" + std::string(ADDRESS) + " 001 " + client.getNickname() + ":Welcome to the Internet Relay Network "
		+ client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + CRLF;
	send(client.getSocket(), res.c_str(), res.length(), 0);
}

void Server::handlePRIVMSG(Client client, Message message) {
	std::string res;

	if ((message.parameters.size() < 1) || (message.parameters.size() == 1 && message.remainder.length())) {
		sendError(411, client, message, "");
		return ;
	}
	if (!message.remainder.length()) {
		sendError(412, client, message, "");
		return ;
	}

	for (size_t i = 0; i < message.parameters.size(); i++) {
		bool keepGoing = false;
		for (size_t j = 0; j < _channels.size(); j++) {
			if (_channels[i]->getName() == message.parameters[i])
				keepGoing = true;
		}
		for (int i = 0; i < _numClients; i++) {
			if (_clients[i].getNickname() == message.parameters[i])
				keepGoing = true;
		}
		if (!keepGoing) {
			sendError(401, client, message, message.parameters[i]);
			return ;
		}
	}

	for (size_t i = 0; i < message.parameters.size(); i++) {
		for (size_t j = 0; j < _channels.size(); j++) {
			if (_channels[i]->getName() == message.parameters[i]) {
				res = ":" + client.getNickname() + " PRIVMSG " + client.getNickname() + " :" + message.remainder + CRLF;
				send(client.getSocket(), res.c_str(), res.length(), 0);
				continue ;
			}
		}
		for (int i = 0; i < _numClients; i++) {
			if (_clients[i].getNickname() == message.parameters[i]) {
				res = ":" + client.getNickname() + " PRIVMSG " + client.getNickname() + " :" + message.remainder + CRLF;
				send(client.getSocket(), res.c_str(), res.length(), 0);
				continue ;
			}
		}
	}
}

void Server::handleJOIN(Client client, Message message) {
	if (message.parameters.size() < 2) {
		sendError(461, client, message, "");
		return ;
	}

	bool channelDoesNotExist = false;
	if (!channelExist(message.parameters[0])) {
		channelDoesNotExist = true;
		return ;
	}

	Channel channel = getChannel(message.parameters[0]);
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

	if (channelDoesNotExist) {
		Channel *newChannel = new Channel(message.parameters[0], PUBLIC, &client);
		_channels.push_back(newChannel);
		_allChannels.push_back(newChannel);
	}

	getChannel(message.parameters[0]).addClient(&client);
	getChannel(message.parameters[0]).addRegistered(&client);
	std::string res = ":" + std::string(ADDRESS) + " 332 " + client.getNickname() + " " + channel.getName() + " :" + channel.getTopic() + CRLF;
	send(client.getSocket(), res.c_str(), res.length(), 0);
}

void Server::handlePART(Client client, Message message) {
	std::string res;

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
		sendError(402, client, message, message.parameters[0]);
		return ;
	}

	std::string sendToChannel = ":" + client.getNickname() + " PART " + message.parameters[0] + CRLF;
	channel.removeClient(client.getNickname());
	channel.sendChannel(sendToChannel);
	
	res = ":" + std::string(ADDRESS) + " PART " + message.parameters[0] + CRLF;
	send(client.getSocket(), res.c_str(), res.length(), 0);

	if (channel.getNbClients() == 0) {
		for (size_t i = 0; i < _channels.size(); i++) {
			if (_channels[i]->getName() == channel.getName())
				_channels.erase(_channels.begin() + i);
		}
	}
}

void Server::handleLIST(Client client, Message message) {
	(void) message;
	std::string res = ":" + std::string(ADDRESS) + " 321 " + client.getNickname() + "Channel :Users  Name" + CRLF;
	send(client.getSocket(), res.c_str(), res.length(), 0);

	for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		if ((*it)->getType() & PUBLIC || (*it)->getType() & PRIVATE) {
			if ((*it)->getType() & PRIVATE && !(*it)->isInChannel(client.getNickname()))
				res =  ":" + std::string(ADDRESS) +  " 322 " + client.getNickname() + " Prv " + toString((*it)->getNbClients()) + CRLF;
			else
				res =  ":" + std::string(ADDRESS) +  " 322 " + client.getNickname() + " " + (*it)->getName() + " " + toString((*it)->getNbClients()) + " :" + (*it)->getTopic() + CRLF;
			send(client.getSocket(), res.c_str(), res.length(), 0);
		}
	}

	res = ":" + std::string(ADDRESS) + " 323 " + client.getNickname() + " :End of /LIST" + CRLF;
	send(client.getSocket(), res.c_str(), res.length(), 0);
}

void Server::handleKICK(Client client, Message message) {
	std::string res;

	if (message.parameters.size() < 2) {
		sendError(461, client, message, "");
		return ;
	}
	if (!channelExist(message.parameters[0])) {
		sendError(403, client, message, message.parameters[0]);
		return ;
	}

	Channel channel = getChannel(message.parameters[0]);
	if (!channel.isInChannel(client.getNickname()) || !clientExist(message.parameters[1])
		|| !channel.isInChannel(getClient(message.parameters[1]).getNickname())) {
		sendError(442, client, message, message.parameters[0]);
		return ;
	}
	if (!channel.isOperator(client.getNickname())) {
		sendError(482, client, message, message.parameters[0]);
		return ;
	}

	getChannel(message.parameters[0]).removeClient(getClient(message.parameters[1]).getNickname());
	getChannel(message.parameters[0]).removeRegistered(getClient(message.parameters[1]).getNickname());
	
	res = ":" + std::string(ADDRESS) + " KICK " + message.parameters[0] + " " + message.parameters[1] + CRLF;
	std::string kick = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + " KICK "
		+ message.parameters[0] + " " + message.parameters[1] + CRLF;
	if (message.parameters.size() == 3) {
		res += " :" + message.remainder;
		kick += " :" + message.remainder;
	}
	send(client.getSocket(), res.c_str(), res.length(), 0);
	send(getClient(message.parameters[1]).getSocket(), kick.c_str(), kick.length(), 0);
}

void Server::handleINVITE(Client client, Message message) {
	if (message.parameters.size() < 2) {
		sendError(461, client, message, "");
		return ;
	}
	if (!clientExist(message.parameters[0])) {
		sendError(401, client, message, message.parameters[0]);
		return ;
	}
	if (!channelExist(message.parameters[1])) {
		sendError(401, client, message, message.parameters[1]);
		return ;
	}

	Channel channel = getChannel(message.parameters[1]);
	if (!channel.isInChannel(client.getNickname())) {
		sendError(442, client, message, message.parameters[1]);
		return ;
	}
	if (!channel.isInChannel(message.parameters[0])) {
		sendError(443, client, message, message.parameters[0] + message.parameters[1]);
		return ;
	}
	if (!channel.isOperator(client.getNickname())) {
		sendError(482, client, message, message.parameters[1]);
		return ;
	}

	getChannel(message.parameters[1]).addRegistered(&getClient(message.parameters[0]));
	std::string res = ":" + std::string(ADDRESS) + " 341 " + client.getNickname() + " " + message.parameters[1] + " "
		+ client.getNickname() + CRLF;
	std::string invite = ":" + std::string(ADDRESS) + "PRIVMSG " + message.parameters[0] + " :You have been invited to "
		+ message.parameters[1] + CRLF;
	send(client.getSocket(), res.c_str(), res.length(), 0);
	send(getClient(message.parameters[0]).getSocket(), invite.c_str(), invite.length(), 0);
}

void Server::handleTOPIC(Client client, Message message) {
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
			channel.sendChannel(res);
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

void Server::handleMODE(Client client, Message message) {
	if (message.parameters.size() < 2) {
		sendError(461, client, message, "");
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

	for (size_t i = 0; i < message.parameters[1].length(); i++) {
		if (std::string("itkol").find(message.parameters[1][i]) == std::string::npos) {
			sendError(472, client, message, std::string() + message.parameters[1][1]);
			return ;
		}
	}

	Channel channel = getChannel(message.parameters[0]);
	if (!channel.isInChannel(client.getNickname())) {
		sendError(442, client, message, channel.getName());
		return ;
	}
	if (!channel.isOperator(client.getNickname())) {
		sendError(482, client, message, channel.getName());
		return ;
	}

	char operation = message.parameters[1][0];
	bool flag[127] = {false};

	for (size_t  i = 1; i < message.parameters[1].length(); i++)
		flag[int(message.parameters[1][i])] = true;

	if (operation != '+' && operation != '-') {
		sendError(472, client, message, std::string() + message.parameters[1][1]);
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
			if (!channel.getPassword().length()) {
				sendError(467, client, message, channel.getName());
				return ;
			} else {
				if (message.parameters.size() < 3) {
					sendError(461, client, message, "");
					return ;
				}
				else
					channel.setPassword(message.parameters[2]);
			}
		} else
			channel.setPassword("");
	} else if (flag['o']) {
			if (message.parameters.size() < 3) {
				sendError(461, client, message, "");
				return ;
			}
			if (operation == '+') {
				channel.addOperator(&getClient(message.parameters[2]));
			}
			else
				channel.removeOperator(getClient(message.parameters[2]).getNickname());
	} else if (flag['l']) {  
		if (message.parameters.size() < 3) {
			sendError(461, client, message, "");
			return ;
		}
		else if (operation == '+')
		{
			channel.setLimited(true);
			channel.setLimitUsers(std::atoi(message.parameters[2].c_str()));
		}
		else
			channel.setLimited(false);
	}

	std::string res = ":" + std::string(ADDRESS) + " 324 " + client.getNickname() + " " + channel.getName() + " " + message.parameters[2] + CRLF;
	send(client.getSocket(), res.c_str(), res.length(), 0);
}