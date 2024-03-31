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

#define MAX_CLIENTS 200
#define BUFFER_SIZE 513

#define CRLF	"\r\n"
#define ADDRESS	"BreizhRC"

class Client;

struct Reply {
	std::string prefix;
	int         code;
	std::string target;
};

struct Message {
	std::string prefix;
	std::string command;
	std::vector<std::string> parameters;
	std::string remainder;
	std::string fullCmd;
	int         errorCode;
};

Message						getParsedCommand(std::string str);
void						sendError(int code, Client client, Message message, std::string opt);
std::string 				toString(int nb);
std::vector<std::string>	split(std::string str, char delimiter);
