#include "Server.hpp"

Server::Server(std::string password, int port) : _clientAddrSize(sizeof(_clientAddr)), _password(password), _numClients(1), _buffer("") {
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

	_socket = socket(AF_INET, SOCK_STREAM, 0);
	_fds[0].fd = _socket;
	_fds[0].events = POLLIN;
	if (_socket < 0) {
		perror("Error in socket creation");
		exit(EXIT_FAILURE);
	}

	int opt = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		perror("Error in setsockopt");
		exit(EXIT_FAILURE);
	}

	memset(&_serverAddr, 0, sizeof(_serverAddr));
	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	_serverAddr.sin_port = htons(port);
	if (bind(_socket, (struct sockaddr*)&_serverAddr, sizeof(_serverAddr)) < 0) {
		perror("Error in bind");
		exit(EXIT_FAILURE);
	}

	if (listen(_socket, MAX_CLIENTS) < 0) {
		perror("Error in listen");
		exit(EXIT_FAILURE);
	}
}

Server::~Server() {
	// for (std::vector<Client *>::iterator it = _allClients.begin(); it != _allClients.end(); ++it) {
	//         delete *it;
	//         _allClients.erase(it);
	// }
	for (std::vector<Channel *>::iterator it = _allChannels.begin(); it != _allChannels.end(); ++it) {
		delete *it;
		// _allChannels.erase(it);
	}
}

void Server::treatNewConnexion() {
	_clients[_numClients - 1] = Client(accept(_socket, (struct sockaddr*)&_clientAddr, &_clientAddrSize));
	if (_clients[_numClients - 1].getSocket() < 0) {
		perror("Error in accept");
	} else {
		std::cout << "New connection accepted" << std::endl;
		_fds[_numClients].fd = _clients[_numClients - 1].getSocket();
		_fds[_numClients].events = POLLIN;
		++_numClients;
	}
}

void Server::receiveMessage(int index) {
	char buff[BUFFER_SIZE] = {};
	ssize_t bytesRead = recv(_fds[index].fd, buff, BUFFER_SIZE, 0);

	if (bytesRead < 0) {
		perror("Error in recv");
		exit(EXIT_FAILURE);
	}

	if (bytesRead == 0) {
		eraseClient(index);
	} else if (bytesRead > 0) {
		std::cout << "index " << index << std::endl;
		_buffer += std::string(buff, bytesRead);
		std::cout << _buffer << std::endl;
	
		size_t pos = _buffer.find("\r\n");
		while (pos != std::string::npos) {
			std::string line = _buffer.substr(0, pos);
			Message res = getParsedCommand(line);
			if (commandsIsImplemented(res.command) && (_clients[index - 1].getRegistered()
				|| (res.command == "PASS" || res.command == "NICK" || res.command == "USER" || res.command == "PING" || res.command == "CAP"))) {
				std::cout << "SENDING REPLY" << std::endl;
				(this->*_stringToFunc[res.command])(_clients[index - 1], res);
			}
			_buffer.erase(0, pos + 2);
			pos = _buffer.find("\r\n");
		}
	}

	if (_clients[index - 1].isQuitting()) {
		eraseClient(index);
	}
}

void Server::run() {
	while (true) {
		if (poll(_fds, _numClients, -1) == -1) {
			perror("Error in poll");
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i < _numClients; ++i) {
			if (_fds[i].revents & POLLIN) {
				if (i == 0)
					treatNewConnexion();
				else
					receiveMessage(i);
			}
		}
	}
	close(_socket);
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

void Server::eraseClient(int index) {
	std::cout << "Client disconnected" << std::endl;
	close(_fds[index].fd);
	_clients[index - 1].clear();
	for (int j = index; j < _numClients - 1; ++j) {
		_fds[j] = _fds[j + 1];
		_clients[j - 1] = _clients[j];
	}
	--_numClients;
}