#include "Server.hpp"

void Server::handleLIST(Client &client, Message message) {
	(void) message;
	std::string res = ":" + std::string(ADDRESS) + " 321 " + client.getNickname() + " Channel :Users  Topic" + CRLF;
	send(client.getSocket(), res.c_str(), res.length(), MSG_DONTWAIT | MSG_NOSIGNAL);

	for (size_t i = 0; i < _channels.size(); i++) {
		res =  ":" + std::string(ADDRESS) +  " 322 " + client.getNickname() + " " + _channels[i]->getName() + " " + toString(_channels[i]->getNbClients()) + " :" + _channels[i]->getTopic() + CRLF;
		send(client.getSocket(), res.c_str(), res.length(), MSG_DONTWAIT | MSG_NOSIGNAL);
	}

	res = ":" + std::string(ADDRESS) + " 323 " + client.getNickname() + " :End of /LIST" + CRLF;
	send(client.getSocket(), res.c_str(), res.length(), MSG_DONTWAIT | MSG_NOSIGNAL);
}