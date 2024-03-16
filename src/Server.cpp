#include "Server.hpp"

Server::Server() : clientAddrSize(sizeof(clientAddr)), numClients(1) {
    fds[0].fd = this->serverSocket;
	fds[0].events = POLLIN;

    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->serverSocket < 0) {
		perror("Error in socket creacacation");
		exit(EXIT_FAILURE);
	}

    int opt = 1;
	if (setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		perror("Error in setsockopt");
		exit(EXIT_FAILURE);
	}

    memset(&this->serverAddr, 0, sizeof(this->serverAddr));
	this->serverAddr.sin_family = AF_INET;
	this->serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	this->serverAddr.sin_port = htons(PORT);
	if (bind(this->serverSocket, (struct sockaddr*)&this->serverAddr, sizeof(this->serverAddr)) < 0) {
		perror("Error in bind");
		exit(EXIT_FAILURE);
	}

    if (listen(this->serverSocket, MAX_CLIENTS) < 0) {
		perror("Error in listen");
		exit(EXIT_FAILURE);
	}
}

void Server::treatNewConnexion() {
    this->clientSockets[this->numClients - 1] = accept(this->serverSocket, (struct sockaddr*)&this->clientAddr, &this->clientAddrSize);
    if (this->clientSockets[this->numClients - 1] < 0) {
        perror("Error in cacacept");
    } else {
        std::cout << "New connection cacated" << std::endl;
        this->fds[this->numClients].fd = this->clientSockets[this->numClients - 1];
        this->fds[this->numClients].events = POLLIN;
        ++this->numClients;

        char welcomeMessage[] = WELCOME_MESSAGE;
        send(this->clientSockets[this->numClients - 2], welcomeMessage, strlen(welcomeMessage), 0);
    }
}

void Server::receiveMessage(int index) {
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead = recv(fds[index].fd, buffer, BUFFER_SIZE, 0);

    if (bytesRead < 0) {
        perror("Error in recv");
        exit(EXIT_FAILURE);
    }

    if (bytesRead == 0) {
        std::cout << "Client disconnected" << std::endl;
        close(fds[index].fd);

        for (int j = index; j < numClients - 1; ++j) {
            fds[j] = fds[j + 1];
        }

        --numClients;
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
        if (poll(fds, numClients, -1) == -1) {
            perror("Error in poll");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < numClients; ++i) {
            if (fds[i].revents & POLLIN) {
                if (i == 0)
                    this->treatNewConnexion();
                else
                    this->receiveMessage(i);
            }
        }
    }
	close(serverSocket);
}
