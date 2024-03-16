#include "Server.hpp"

void Server::handlePING(Client client, Message message) {
	std::string sendMessage;
	if (message.command == "PING") {
		sendMessage = "PONG " + message.commandParameters[0];
		std::cout << "answer : " << sendMessage << std::endl;
		send(clientSocket, sendMessage.c_str(), sendMessage.length(), 0);
	}
}

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