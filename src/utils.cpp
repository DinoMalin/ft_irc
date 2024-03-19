#include "header.hpp"
#include "Client.hpp"

Message getParsedCommand(std::string str) {
	Message result;

	// if (str.substr(str.length() - 2) != CRLF)
	// 	result.errorCode = 421;
	// else {
	// 	str.erase(str.length() - 2, str.length() - 1);
	// }

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

	if (code == 401)
		res = ":" + std::string(ADDRESS) + " 401 " + client.getNickname() + " " + opt + " :No such nick/channel" + CRLF;
	else if (code == 403)
		res = ":" + std::string(ADDRESS) + " 403 " + client.getNickname() + " " + opt + " :No such channel" + CRLF;
	else if (code == 411)
		res = ":" + std::string(ADDRESS) + " 411 " + client.getNickname() + " ::No recipient given" + CRLF;
	else if (code == 412)
		res = ":" + std::string(ADDRESS) + " 412 " + client.getNickname() + " ::No text to send" + CRLF;
	else if (code == 431)
		res = ":" + std::string(ADDRESS) + " 431 " + client.getNickname() + " :No nickname given" + CRLF;
	else if (code == 433)
		res = ":" + std::string(ADDRESS) + " 433 " + client.getNickname() + " :Nickname is already in use" + CRLF;
	else if (code == 442)
		res = ":" + std::string(ADDRESS) + " 442 " + client.getNickname() + " " + opt + " :You're not on that channel" + CRLF;
	else if (code == 443)
		res = ":" + std::string(ADDRESS) + " 443 " + client.getNickname() + " " + opt + " :is already on channel" + CRLF;
	else if (code == 461)
		res = ":" + std::string(ADDRESS) + " 461 " + client.getNickname() + " " + message.fullCmd + " :Not enough parameters" + CRLF;
	else if (code == 462)
		res = ":" + std::string(ADDRESS) + " 462 " + client.getNickname() + " :You may not reregister" + CRLF;
	else if (code == 464)
		res = ":" + std::string(ADDRESS) + " 464 " + client.getNickname() + " :Password incorrect" + CRLF;
	else if (code == 467)
		res = ":" + std::string(ADDRESS) + " 467 " + client.getNickname() + " " + opt + " :Channel key already set" + CRLF;
	else if (code == 471)
		res = ":" + std::string(ADDRESS) + " 471 " + client.getNickname() + " " + opt + " :Cannot join channel (+l)" + CRLF;
	else if (code == 472)
		res = ":" + std::string(ADDRESS) + " 472 " + client.getNickname() + " " + opt + " :is unknown mode char to me" + CRLF;
	else if (code == 473)
		res = ":" + std::string(ADDRESS) + " 473 " + client.getNickname() + " " + opt + " :Cannot join channel (+i)" + CRLF;
	else if (code == 475)
		res = ":" + std::string(ADDRESS) + " 475 " + client.getNickname() + " " + opt + " :Cannot join channel (+k)" + CRLF;
	else if (code == 482)
		res = ":" + std::string(ADDRESS) + " 482 " + client.getNickname() + " " + opt + " :You're not channel operator" + CRLF;
	send(client.getSocket(), res.c_str(), res.length(), 0);
}

std::string toString(int nb) {
	std::stringstream ss;
	ss << nb;
	return ss.str();
}