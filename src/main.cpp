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

#define PORT 6667
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

enum Type {
    PING,
    PONG,
    USER,
    NICK,
    JOIN,
    PART,
    PRIVMSG,
    QUIT,
    MODE,
    WHOIS,
    TOPIC,
    NOTICE
};

struct response {
    Type type;
    std::string command;
};

response getParsedCommand(char *str) {
	response result;
	std::string stringCommand = std::string(str);
	std::string type = stringCommand.substr(0, stringCommand.find(" "));
	result.command = stringCommand.substr(stringCommand.find(" ") + 1, stringCommand.length());
	if (type == "PING")
		result.type = PING;
	else if (type == "PONG")
		result.type = PONG;
	else if (type == "USER")
		result.type = USER;
	 else if (type == "NICK")
		result.type = NICK;
	 else if (type == "JOIN")
		result.type = JOIN;
	 else if (type == "PART")
		result.type = PART;
	 else if (type == "PRIVMSG")
		result.type = PRIVMSG;
	 else if (type == "QUIT")
		result.type = QUIT;
	 else if (type == "MODE")
		result.type = MODE;
	 else if (type == "WHOIS")
		result.type = WHOIS;
	 else if (type == "TOPIC")
		result.type = TOPIC;
	 else if (type == "NOTICE")
		result.type = NOTICE;
	return result;
}

int main() {
	int serverSocket, clientSocket;
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
	while (1) {
		clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
		if (clientSocket < 0) {
			perror("Error in accept");
			exit(EXIT_FAILURE);
		}

		char welcomeMessage[] = ":irc.server.com 001 user :Welcome to the Internet Relay Network user!user@hostname\n";
        send(clientSocket, welcomeMessage, strlen(welcomeMessage), 0);

		// Receive data from client
		ssize_t bytesRead;
		while ((bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0)) > 0) {
			buffer[bytesRead] = '\0'; // Null terminate the received data
			response res = getParsedCommand(buffer);
			std::cout << res.type << std::endl;
			std::cout << res.command << std::endl;
			// std::cout << "Received: " << buffer << std::endl;
			// Process IRC commands here
		}
		if (bytesRead == 0) {
			std::cout << "Client disconnected" << std::endl;
		} else if (bytesRead < 0) {
			perror("Error in recv");
			exit(EXIT_FAILURE);
		}

		// Close client socket
		close(clientSocket);
	}

	// Close server socket
	close(serverSocket);

	return 0;
}
