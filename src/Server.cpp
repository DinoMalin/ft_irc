#include "Server.hpp"

Server::Server(std::string password) : _clientAddrSize(sizeof(_clientAddr)), _numClients(1), _password(password) {
    _fds[0].fd = _socket;
	_fds[0].events = POLLIN;

    _socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket < 0) {
		perror("Error in socket creacacation");
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
	_serverAddr.sin_port = htons(PORT);
	if (bind(_socket, (struct sockaddr*)&_serverAddr, sizeof(_serverAddr)) < 0) {
		perror("Error in bind");
		exit(EXIT_FAILURE);
	}

    if (listen(_socket, MAX_CLIENTS) < 0) {
		perror("Error in listen");
		exit(EXIT_FAILURE);
	}
}

void Server::treatNewConnexion() {
    _clients[_numClients - 1] = Client(accept(_socket, (struct sockaddr*)&_clientAddr, &_clientAddrSize));
    if (_clients[_numClients - 1].getSocket() < 0) {
        perror("Error in cacacept");
    } else {
        std::cout << "New connection cacated" << std::endl;
        _fds[_numClients].fd = _clients[_numClients - 1].getSocket();
        _fds[_numClients].events = POLLIN;
        ++_numClients;

        char welcomeMessage[] = WELCOME_MESSAGE;
        send(_clients[_numClients - 2].getSocket(), welcomeMessage, strlen(welcomeMessage), 0);
    }
}

void Server::receiveMessage(int index) {
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead = recv(_fds[index].fd, buffer, BUFFER_SIZE, 0);

    if (bytesRead < 0) {
        perror("Error in recv");
        exit(EXIT_FAILURE);
    }

    if (bytesRead == 0) {
        std::cout << "Client disconnected" << std::endl;
        close(_fds[index].fd);

        for (int j = index; j < _numClients - 1; ++j) {
            _fds[j] = _fds[j + 1];
        }

        --_numClients;
    } else if (bytesRead > 0) {
        // Handle IRC command
        buffer[bytesRead] = '\0';

        Message res = getParsedCommand(buffer);
        std::cout << res.command << std::endl;

        for (size_t i = 0; i < res.commandParameters.size(); i++) {
            std::cout << res.commandParameters[i] << std::endl;
        }

        std::cout << std::endl;
        // answer(res, clientSockets[numClients - 2]);
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
