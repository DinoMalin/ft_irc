#include "header.hpp"
#include "Client.hpp"

Message getParsedCommand(std::string str) {
	Message result;

	if (str.substr(str.length() - 2) != CRLF)
		result.errorCode = 421;
	else {
		str.erase(str.length() - 2, str.length() - 1);
	}

	if (str.length() > 512 && result.errorCode != 421) {
		str.erase(510, str.length() - 1);
	}

	result.fullCmd = str;
	std::stringstream ss(str);
	std::string currentParam;

	if (str[0] == ':') {
		ss >> result.prefix;
		result.prefix.erase(0, 1);
	}

	ss >> result.command;

	while (ss >> currentParam && currentParam[0] != ':') {
		result.parameters.push_back(currentParam);
	}

	if (result.parameters.size() > 15) {
		while (result.parameters.size() != 15) {
			result.parameters.pop_back();
		}
		return result;
	}

	if (currentParam[0] == ':') {
		std::string remainder;
		std::getline(ss, remainder);
		currentParam.erase(0, 1);
	
		result.remainder = currentParam + remainder;
		result.parameters.push_back(result.remainder);
	}
	return result;
}

void sendError(int code, Client client, Message message, std::string opt) {
    std::string res;

    if (code == 461)
        res = ":" + std::string(ADDRESS) + " 461 " + client.getNickname() + " " + message.fullCmd + " :Not enough parameters\r\n";
    else if (code == 462)
        res = ":" + std::string(ADDRESS) + " 462 " + client.getNickname() + " :You may not reregister\r\n";
    else if (code == 464)
        res = ":" + std::string(ADDRESS) + " 464 " + client.getNickname() + " :Password incorrect\r\n";
    else if (code == 431)
        res = ":" + std::string(ADDRESS) + " 431 " + client.getNickname() + " :No nickname given\r\n";
    else if (code == 433)
        res = ":" + std::string(ADDRESS) + " 433 " + client.getNickname() + " :Nickname is already in use\r\n";
    else if (code == 411)
        res = ":" + std::string(ADDRESS) + " 411 " + client.getNickname() + " ::No recipient given\r\n";
    else if (code == 412)
        res = ":" + std::string(ADDRESS) + " 412 " + client.getNickname() + " ::No text to send\r\n";
    else if (code == 401)
        res = ":" + std::string(ADDRESS) + " 401 " + client.getNickname() + " " + opt + " :No such nick/channel\r\n"; // i
    else if (code == 403)
        res = ":" + std::string(ADDRESS) + " 403 " + client.getNickname() + " " + opt + " :Not such channel\r\n"; // 0
    else if (code == 442)
        res = ":" + std::string(ADDRESS) + " 442 " + client.getNickname() + " " + opt + " :You're not on that channel\r\n"; // 0
    else if (code == 442)
        std::string res =  ":irc.server.com 321 " + client.getNickname() + "Channel :Users  Name";

    send(client.getSocket(), res.c_str(), res.length(), 0);
}