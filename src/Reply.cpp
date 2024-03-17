#include "Server.hpp"

void Server::handlePING(Client client, Message message) {
	std::string res;
	if (message.command == "PING") {
		res = "PONG irc.server.com\r\n";
		send(client.getSocket(), res.c_str(), res.length(), 0);
	}
}

void Server::handlePASS(Client client, Message message) {
    std::string res;
    if (message.commandParameters.size() == 0)
        res = ":" + std::string(ADDRESS) + " 461 " + client.getNickname() + " :Incorrect password\r\n";
    else if (message.commandParameters[1] != _password)
        res = ":" + std::string(ADDRESS) + " 464 " + client.getNickname() + " :Incorrect password\r\n";
    else
        res = ":" + std::string(ADDRESS) + " 001 " + client.getNickname() + " :Correct password\r\n";
    
}
void Server::handleNICK(Client client, Message message) {}
void Server::handleUSER(Client client, Message message) {}

void Server::handlePRIVMSG(Client client, Message message) {}
void Server::handleJOIN(Client client, Message message) {}
void Server::handlePART(Client client, Message message) {}
void Server::handleLIST(Client client, Message message) {}


void Server::handleKICK(Client client, Message message) {}
void Server::handleINVITE(Client client, Message message) {}
void Server::handleTOPIC(Client client, Message message) {}

void Server::handleMODE(Client client, Message message, char flag) {
    
    switch (flag) {
        case 'i':
            break;
        case 't':
            break;
        case 'k':
            break;
        case 'o':
            break;
        case 'l':
            break;
    }
}