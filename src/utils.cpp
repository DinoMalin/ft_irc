#include "header.hpp"

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
		result.commandParameters.push_back(currentParam);
	}

	if (result.commandParameters.size() > 15) {
		while (result.commandParameters.size() != 15) {
			result.commandParameters.pop_back();
		}
		return result;
	}

	if (currentParam[0] == ':') {
		std::string remainder;
		std::getline(ss, remainder);
		currentParam.erase(0, 1);
	
		result.remainder = currentParam + remainder;
		result.commandParameters.push_back(result.remainder);
	}
	return result;
}