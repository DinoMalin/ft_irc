#include "Server.hpp"

void Server::handleCAP(Client &client, Message message) {
    if (message.fullCmd == "CAP LS") {
        std::string res = "CAP * LS :" + std::string(CRLF);
		send(client.getSocket(), res.c_str(), res.length(), 0);
    }
}
