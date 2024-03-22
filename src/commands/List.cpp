#include "Server.hpp"

void Server::handleLIST(Client &client, Message message) {
	(void) message;
	std::string res = ":" + std::string(ADDRESS) + " 321 " + client.getNickname() + " Channel :Users  Name" + CRLF;
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