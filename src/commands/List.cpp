#include "Server.hpp"

void Server::handleLIST(Client &client, Message message) {
	(void) message;
	std::string res = ":" + std::string(ADDRESS) + " 321 " + client.getNickname() + " Channel :Users  Topic" + CRLF;
	send(client.getSocket(), res.c_str(), res.length(), 0);

	for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		res =  ":" + std::string(ADDRESS) +  " 322 " + client.getNickname() + " " + (*it)->getName() + " " + toString((*it)->getNbClients()) + " :" + (*it)->getTopic() + CRLF;
		send(client.getSocket(), res.c_str(), res.length(), 0);
	}

	res = ":" + std::string(ADDRESS) + " 323 " + client.getNickname() + " :End of /LIST" + CRLF;
	send(client.getSocket(), res.c_str(), res.length(), 0);
}