#include "Server.hpp"
#include "header.hpp"

void Server::handlePING(Client client, Message message) {
	std::string res = "PONG irc.server.com\r\n";
    send(client.getSocket(), res.c_str(), res.length(), 0);
}

void Server::handlePASS(Client client, Message message) {
    if (message.parameters.size() == 0) {
        sendError(461, client, message, "");
        return ;
    } else if (client.getRegistered()) {
        sendError(462, client, message, "");
        return ;
    } else if (message.parameters[0] != _password) {
        sendError(464, client, message, "");
        return ;
    }

    std::string res = ":" + std::string(ADDRESS) + " 001 " + client.getNickname() + " :Password correct\r\n";
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
    res = ":" + std::string(ADDRESS) + " 001 " + client.getNickname() + "\r\n";
    send(client.getSocket(), res.c_str(), res.length(), 0);
}

void Server::handleUSER(Client client, Message message) {
    if (client.getRegistered())  {
        sendError(462, client, message, "");
        return ;
    } else if (message.parameters.size() < 4) {
        sendError(461, client, message, "");
        return ;
    }

    client.setUsername(message.parameters[0]);
    std::string res = ":" + std::string(ADDRESS) + " 001 " + client.getNickname() + ":Welcome to the Internet Relay Network user!user@hostname\r\n";
    send(client.getSocket(), res.c_str(), res.length(), 0);
}

void Server::handlePRIVMSG(Client client, Message message) {
    std::string res;

    if ((message.parameters.size() < 1) || (message.parameters.size() == 1 && message.remainder.length())) {
        sendError(411, client, message, "");
        return ;
    } else if (!message.remainder.length()) {
        sendError(412, client, message, "");
        return ;
    }

    for (int i = 0; i < message.parameters.size(); i++) {
        bool keepGoing = false;
        for (int j = 0; j < _channels.size(); j++) {
            if (_channels[i].getName() == message.parameters[i])
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

    for (int i = 0; i < message.parameters.size(); i++) {
        for (int j = 0; j < _channels.size(); j++) {
            if (_channels[i].getName() == message.parameters[i]) {
                res = ":" + client.getNickname() + " PRIVMSG " + client.getNickname() + " :" + message.remainder + "\r\n";
                send(client.getSocket(), res.c_str(), res.length(), 0);
                continue ;
            }
        }
        for (int i = 0; i < _numClients; i++) {
            if (_clients[i].getNickname() == message.parameters[i]) {
                res = ":" + client.getNickname() + " PRIVMSG " + client.getNickname() + " :" + message.remainder + "\r\n";
                send(client.getSocket(), res.c_str(), res.length(), 0);
                continue ;
            }
        }
    }
}

void Server::handleJOIN(Client client, Message message) {
    
}

void Server::handlePART(Client client, Message message) {
    std::string res;
    bool exist = false;

    if (message.parameters.size() < 1) {
        sendError(461, client, message, "");
        return ;
    }

    for (int i = 0; i < _channels.size(); i++) {
        if (_channels[i].getName() == message.parameters[i]) {
            exist = true;
            break;
        }
    }
    if (!exist) {
        sendError(403, client, message, message.parameters[0]);
        return ;
    }

    Channel channel = getChannel(message.parameters[0]);
    if (!channel.isInChannel(client.getNickname())) {
        sendError(402, client, message, message.parameters[0]);
        return ;
    }

    std::string sendToChannel = ":" + client.getNickname() + " PART " + message.parameters[0];
    channel.removeClient(client.getNickname());
    channel.sendChannel(sendToChannel);
    
    res = ":" + std::string(ADDRESS) + " PART " + message.parameters[0];
    send(client.getSocket(), res.c_str(), res.length(), 0);
}

void Server::handleLIST(Client client, Message message) {
    std::string res = ":" + std::string(ADDRESS) + " 321 " + client.getNickname() + "Channel :Users  Name";
    send(client.getSocket(), res.c_str(), res.length(), 0);
    for (std::vector<Channel&>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        if (it->getType() & PUBLIC || it->getType() & PRIVATE) {
            res =  ":" + std::string(ADDRESS) +  " 322 " + client.getNickname() + " " + it->getName() + " " + it->getNbClients() + " :" + it->getTopic();
            send(client.getSocket(), res.c_str(), res.length(), 0);
        }
    }
    res = ":" + std::string(ADDRESS) + " 323 " + client.getNickname() + " :End of /LIST";
    send(client.getSocket(), res.c_str(), res.length(), 0);
}

void Server::handleKICK(Client client, Message message) {
    std::string res;

    if (message.parameters.size() < 2) {
        sendError(461, client, message, "");
        return ;
    }

    bool exist = false;
    for (int i = 0; i < _channels.size(); i++) {
        if (_channels[i].getName() == message.parameters[0])
            exist = true;
    }
    if (!exist) {
        res = ":" + std::string(ADDRESS) + " 403 " + client.getNickname() + " " + message.parameters[0] + " :Not such channel\r\n";
        send(client.getSocket(), res.c_str(), res.length(), 0);
        return ;
    }
    
    if (getChannel(message.parameters[0]).isOperator(client.getNickname()) && !getChannel(message.parameters[0]).isOperator(getClient(message.parameters[1]).getNickname())) {
        std::string res = "KICK " + message.parameters[0] + " " + message.parameters[1];
        if (message.parameters.size() == 3)
            res += " :" + message.parameters[2];
        getChannel(message.parameters[0]).removeClient(getClient(message.parameters[1]).getNickname());
        send(getClient(message.parameters[1]).getSocket(), res.c_str(), res.length(), 0);
    }
}

void Server::handleINVITE(Client client, Message message) {
    std::string res = "PRIVMSG " + message.parameters[0] + " :You have been invited to " + message.parameters[1];
    getChannel(message.parameters[1]).addRegistered(getClient(message.parameters[0]));
    send(getClient(message.parameters[0]).getSocket(), res.c_str(), res.length(), 0);
}

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