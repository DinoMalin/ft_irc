
#include "Server.hpp"

void Server::initFuncs() {
	_stringToFunc["CAP"] = &Server::handleCAP;
	_stringToFunc["PING"] = &Server::handlePING;
	_stringToFunc["PASS"] = &Server::handlePASS;
	_stringToFunc["NICK"] = &Server::handleNICK;
	_stringToFunc["USER"] = &Server::handleUSER;
	_stringToFunc["PRIVMSG"] = &Server::handlePRIVMSG;
	_stringToFunc["JOIN"] = &Server::handleJOIN;
	_stringToFunc["PART"] = &Server::handlePART;
	_stringToFunc["LIST"] = &Server::handleLIST;
	_stringToFunc["KICK"] = &Server::handleKICK;
	_stringToFunc["INVITE"] = &Server::handleINVITE;
	_stringToFunc["TOPIC"] = &Server::handleTOPIC;
	_stringToFunc["MODE"] = &Server::handleMODE;
	_stringToFunc["QUIT"] = &Server::handleQUIT;
}

Channel& Server::getChannel(std::string name) {
	for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		if ((*it)->getName() == name)
			return *(*it);
	}
	return *_channels[0];
}

Client& Server::getClient(std::string nickname) {
	for (int i = 0; i < MAX_CLIENTS; ++i) {
		if (_clients[i].getNickname() == nickname)
			return _clients[i];
	}
	return _clients[0];
}

bool Server::clientExist(std::string nickname) {
	for (int i = 0; i < _numClients; i++) {
		if (_clients[i].getNickname() == nickname)
			return true;
	}
	return false;
}

bool Server::channelExist(std::string channel) {
	for (size_t i = 0; i < _channels.size(); i++) {
		if (_channels[i]->getName() == channel)
			return true;
	}
	return false;
}

bool Server::commandsIsImplemented(std::string str) {
	for (std::map<std::string, Funcs>::iterator it = _stringToFunc.begin(); it != _stringToFunc.end(); it++) {
		if (it->first == str)
			return true;
	}
	return false;
}

void Server::disconnectClient(int index) {
	std::cout << "Client disconnected" << std::endl;
    Client &client = _clients[index - 1];

	for (size_t i = 0; i < _channels.size(); i++) {
        std::string res = ":" + client.getSource() + " QUIT " + _channels[i]->getName() + " :" + client.getNickname() + CRLF;
	    _channels[i]->sendChannel(res, client, false);
		_channels[i]->removeClient(client.getNickname());

        if (_channels[i]->getNbClients() == 0) {
            for (size_t j = 0; j < _channels.size(); j++) {
                if (_channels[j]->getName() == _channels[j]->getName())
                    _channels.erase(_channels.begin() + j);
            }
		}
	}

	close(_fds[index].fd);
	for (int i = index; i < _numClients - 1; ++i) {
		_fds[i] = _fds[i + 1];
		_clients[i - 1] = _clients[i];
	}

    for (size_t i = 0; i < _channels.size(); i++) {
        _channels[i]->rearrangeUsers(&_clients[i - 1]);
    }
	--_numClients;
}

void Server::disconnectEveryone() {
	for (int i = 0; i < _numClients; i++) {
		disconnectClient(i);
	}
	for (std::vector<Channel *>::iterator it = _allChannels.begin(); it != _allChannels.end(); ++it) {
		delete *it;
	}
}

void Server::sendWelcome(Client &client) {
	std::string res1 = ":" + std::string(ADDRESS) + " 001 " + client.getNickname() + " :Welcome to the Internet Relay Network, " + client.getSource() + CRLF;
	std::string res2 = ":" + std::string(ADDRESS) + " 002 " + client.getNickname() + " :Your host is " + client.getSource() + ", running version 0.29.22" + CRLF;
	std::string res3 = ":" + std::string(ADDRESS) + " 003 " + client.getNickname() + " :This server was created on 20th January 2004" + CRLF;
	std::string res4 = ":" + std::string(ADDRESS) + " 004 " + client.getNickname() + " IrcServer 0.29.22  itkol " + CRLF;
	std::string res5 = ":" + std::string(ADDRESS) + " 005 " + client.getNickname() + " NICKLEN=10 :are supported by this server " + CRLF;
	
	send(client.getSocket(), res1.c_str(), res1.length(), 0);
	send(client.getSocket(), res2.c_str(), res2.length(), 0);
	send(client.getSocket(), res3.c_str(), res3.length(), 0);
	send(client.getSocket(), res4.c_str(), res4.length(), 0);
	send(client.getSocket(), res5.c_str(), res5.length(), 0);
}
