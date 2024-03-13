#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <algorithm>
#include <sstream>
#include <vector>

#define PORT 6667
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

// enum Type {
//     PING,
//     PONG,
//     USER,
//     NICK,
//     JOIN,
//     PART,
//     PRIVMSG,
//     QUIT,
//     MODE,
//     WHOIS,
//     TOPIC,
//     NOTICE
// };

struct Response {
    std::string prefix;
    std::string command;
    std::vector<std::string> commandParameters;
};

Response getParsedCommand(std::string str) {
	Response result;
    std::stringstream ss(str);
    std::string currentParam;

    ss >> result.command;
    if (result.command == "PRIVMSG") {
        ss >> currentParam;
        result.commandParameters.push_back(currentParam);

        std::string msg;
        std::getline(ss, msg);
        result.commandParameters.push_back(msg);
        result.commandParameters[1].erase(0, 2);
    } else {
        while (ss >> currentParam) {
            result.commandParameters.push_back(currentParam);
        }
    }

	return result;
}

void answer(Response res, int clientSocket) {
    std::string sendMessage;
    if (res.command == "PING") {
        sendMessage = "PONG " + res.commandParameters[0];
        std::cout << "answer : " << sendMessage << std::endl;
        send(clientSocket, sendMessage.c_str(), sendMessage.length(), 0);
    }
}

int main() {
	int serverSocket, clientSockets[MAX_CLIENTS];
	struct sockaddr_in serverAddr, clientAddr;
	socklen_t clientAddrSize = sizeof(clientAddr);
	char buffer[BUFFER_SIZE];

	// Create socket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0) {
		perror("Error in socket creation");
		exit(EXIT_FAILURE);
	}

	// Set socket options
	int opt = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		perror("Error in setsockopt");
		exit(EXIT_FAILURE);
	}

	// Bind socket
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(PORT);
	if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
		perror("Error in bind");
		exit(EXIT_FAILURE);
	}

	// Listen for connections
	if (listen(serverSocket, MAX_CLIENTS) < 0) {
		perror("Error in listen");
		exit(EXIT_FAILURE);
	}

	// Accept connections
    struct pollfd fds[MAX_CLIENTS + 1];
    fds[0].fd = serverSocket;
    fds[0].events = POLLIN;
    int numClients = 1;

    // Accept connections and handle data
    while (true) {
        // Use poll to monitor events
        if (poll(fds, numClients, -1) == -1) {
            perror("Error in poll");
            exit(EXIT_FAILURE);
        }

        // Check events for each file descriptor
        for (int i = 0; i < numClients; ++i) {
            if (fds[i].revents & POLLIN) {
                if (i == 0) {
                    // New incoming connection
                    clientSockets[numClients - 1] = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
                    if (clientSockets[numClients - 1] < 0) {
                        perror("Error in accept");
                    } else {
                        std::cout << "New connection accepted" << std::endl;
                        fds[numClients].fd = clientSockets[numClients - 1];
                        fds[numClients].events = POLLIN;
                        ++numClients;

						char welcomeMessage[] = ":irc.server.com 001 user :Welcome to the Internet Relay Network user!user@hostname\n";
        				send(clientSockets[numClients - 2], welcomeMessage, strlen(welcomeMessage), 0);
                    }
                } else {
                    // Data received from a client
                    ssize_t bytesRead = recv(fds[i].fd, buffer, BUFFER_SIZE, 0);
                    if (bytesRead > 0) {
                        // Process IRC commands here

                        buffer[bytesRead] = '\0';
                        Response res = getParsedCommand(buffer);
                        std::cout << res.command << std::endl;
                        for (size_t i = 0; i < res.commandParameters.size(); i++) {
                            std::cout << res.commandParameters[i] << std::endl;
                        }
                        std::cout << std::endl;
                        
                        answer(res, clientSockets[numClients - 2]);

                    } else if (bytesRead == 0) {
                        std::cout << "Client disconnected" << std::endl;
                        // Remove the disconnected client from the poll set
                        close(fds[i].fd);
                        for (int j = i; j < numClients - 1; ++j) {
                            fds[j] = fds[j + 1];
                        }
                        --numClients;
                    } else {
                        perror("Error in recv");
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
    }


	// Close server socket
	close(serverSocket);

	return 0;
}
