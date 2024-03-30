#include "Server.hpp"

Server::Server(int port, std::string password) : _clientAddrSize(sizeof(_clientAddr)), _password(password), _numClients(1), _buffer("") {
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
		std::cout << "Error in socket creation" << std::endl;
		exit(EXIT_FAILURE);
	}

	int opt = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		std::cout << "Error in setsockopt" << std::endl;
		close(_socket);
		exit(EXIT_FAILURE);
	}

	memset(&_serverAddr, 0, sizeof(_serverAddr));
	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	_serverAddr.sin_port = htons(port);
	if (bind(_socket, (struct sockaddr*)&_serverAddr, sizeof(_serverAddr)) < 0) {
		std::cout << "Error in bind" << std::endl;
		close(_socket);
		exit(EXIT_FAILURE);
	}

	if (listen(_socket, MAX_CLIENTS) < 0) {
		std::cout << "Error in listen" << std::endl;
		close(_socket);
		exit(EXIT_FAILURE);
	}
}

Server::~Server() {disconnectEveryone();}

void Server::treatNewConnexion() {
	_clients[_numClients - 1] = Client(accept(_socket, (struct sockaddr*)&_clientAddr, &_clientAddrSize));
	if (_clients[_numClients - 1].getSocket() < 0) {
		std::cout << "Error in accept" << std::endl;
	} else {
		std::cout << "New connection accepted" << std::endl;
		_clients[_numClients - 1].setIp(inet_ntoa(_clientAddr.sin_addr));
		_fds[_numClients].fd = _clients[_numClients - 1].getSocket();
		_fds[_numClients].events = POLLIN;
		++_numClients;
	}
}

void Server::receiveMessage(int index) {
	char buff[BUFFER_SIZE] = {};
	ssize_t bytesRead = recv(_fds[index].fd, buff, BUFFER_SIZE, 0);

	if (bytesRead < 0) {
		std::cout << "Error in recv" << std::endl;
	}
	if (bytesRead == 0) {
		disconnectClient(index);
	} else if (bytesRead > 0) {
		std::cout << "Client no." << index << std::endl;
		_clients[index - 1]._buffer += std::string(buff, bytesRead);
		std::cout << _clients[index - 1]._buffer << std::endl;
	
		size_t pos = _clients[index - 1]._buffer.find("\r\n");
		while (pos != std::string::npos) {
			std::string line = _clients[index - 1]._buffer.substr(0, pos);
			Message res = getParsedCommand(line);
			if (commandsIsImplemented(res.command) && (_clients[index - 1].getRegistered()
				|| (res.command == "PASS" || res.command == "NICK" || res.command == "USER" || res.command == "PING" || res.command == "CAP"))) {
				(this->*_stringToFunc[res.command])(_clients[index - 1], res);
			}
			_clients[index - 1]._buffer.erase(0, pos + 2);
			pos = _clients[index - 1]._buffer.find("\r\n");
		}
	}

	if (_clients[index - 1].isQuitting()) {
		disconnectClient(index);
	}
}

void Server::run() {
	while (true) {
		if (poll(_fds, _numClients, -1) == -1) {
			std::cout << "Error in poll" << std::endl;
			close(_socket);
			disconnectEveryone();
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

void Server::disconnectClient(int index) {
	std::cout << "Client disconnected" << std::endl;
	close(_fds[index].fd);
	_clients[index - 1].clear();
	for (int i = index; i < _numClients - 1; ++i) {
		_fds[i] = _fds[i + 1];
		_clients[i - 1] = _clients[i];
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