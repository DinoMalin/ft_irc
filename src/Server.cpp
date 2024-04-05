#include "Server.hpp"

Server::Server(int port, std::string password) : _clientAddrSize(sizeof(_clientAddr)), _password(password) {
	initFuncs();

	_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct pollfd serverFd;
	serverFd.fd = _socket;
	serverFd.events = POLLIN;
	_fds.push_back(serverFd);

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

void Server::run() {
	while (true) {
		if (poll(&_fds[0], _fds.size(), -1) == -1) {
			std::cout << "Error in poll" << std::endl;
			close(_socket);
			disconnectEveryone();
			exit(EXIT_FAILURE);
		}

		for (size_t i = 0; i < _fds.size(); ++i) {
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

void Server::treatNewConnexion() {
	_clients.push_back(accept(_socket, (struct sockaddr*)&_clientAddr, &_clientAddrSize));
	if (_clients[_clients.size() - 1].getSocket() < 0) {
		std::cout << "Error in accept" << std::endl;
	} else {
		std::cout << "New connection accepted" << std::endl;
		_clients[_clients.size() - 1].setIp(inet_ntoa(_clientAddr.sin_addr));

		struct pollfd newFd;
		newFd.fd = _clients[_clients.size() - 1].getSocket();
		newFd.events = POLLIN;
		newFd.revents = 0;
		_fds.push_back(newFd);
	}
}

void Server::receiveMessage(int index) {
	char buff[BUFFER_SIZE] = {};
	ssize_t bytesRead = recv(_fds[index].fd, buff, BUFFER_SIZE, 0);

	std::cout << "Client no." << index << std::endl;

	Client &client = _clients[index - 1];
	if (bytesRead < 0)
		std::cout << "Error in recv" << std::endl;
	if (bytesRead == 0)
		disconnectClient(index);
	else if (bytesRead > 0) {
		client._buffer += std::string(buff, bytesRead);
		std::cout << client._buffer << std::endl;
	
		size_t pos = client._buffer.find("\r\n");
		while (pos != std::string::npos) {
			std::string line = client._buffer.substr(0, pos);
			Message res = getParsedCommand(line);

			if (commandsIsImplemented(res.command) && (client.getRegistered() || (res.command == "PASS" || res.command == "CAP"))) {
				(this->*_stringToFunc[res.command])(client, res);
				if (client.getRegistered() && client.getUsered() && client.getNicked() && !client.getWelcomeSent()) {
					sendWelcome(client);
					client.setWelcomeSent();
				}
			} else if (commandsIsImplemented(res.command)) {
				sendError(464, client, res, "");
				client.quitting();
			}

			client._buffer.erase(0, pos + 2);
			pos = client._buffer.find("\r\n");
		}
	}

	if (client.isQuitting())
		disconnectClient(index);
}
