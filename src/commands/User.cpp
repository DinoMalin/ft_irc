#include "Server.hpp"

void Server::handleUSER(Client &client, Message message) {
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
	client.setSource(client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname());
	std::string res1 = ":" + std::string(ADDRESS) + " 001 " + client.getNickname() + " :Welcome to the Internet Relay Network, " + client.getSource() + CRLF;
	std::string res2 = ":" + std::string(ADDRESS) + " 002 " + client.getNickname() + " :Your host is " + client.getSource() + ", running version 0.29.22" + CRLF;
	std::string res3 = ":" + std::string(ADDRESS) + " 003 " + client.getNickname() + " :This server was created on 1st January 1970" + CRLF;
	std::string res4 = ":" + std::string(ADDRESS) + " 004 " + client.getNickname() + " IrcServer 0.29.22 0 itkol " + CRLF;
	send(client.getSocket(), res1.c_str(), res1.length(), 0);
	send(client.getSocket(), res2.c_str(), res2.length(), 0);
	send(client.getSocket(), res3.c_str(), res3.length(), 0);
	send(client.getSocket(), res4.c_str(), res4.length(), 0);
}