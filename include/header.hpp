#pragma once

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

#define CRLF "\r\n"

#define WELCOME_MESSAGE ":irc.server.com 001 user :Welcome to the Internet Relay Network user!user@hostname\n"
#define ADDRESS "irc.example.com"

struct Reply {
	std::string prefix;
	int         code;
	std::string target;
};

struct Message {
	std::string prefix;
	std::string command;
	std::vector<std::string> commandParameters;
	std::string remainder;
	std::string fullCmd;
	int         errorCode;
};

Message getParsedCommand(std::string str);

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
