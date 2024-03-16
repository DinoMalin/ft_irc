#pragma once

#include <iostream>
#include "Channel.hpp"

class Channel;

class Client {
	private:
		const int _socket;
		std::string _username;
		std::string _nickname;
		Channel *_channel;
	public:
		Client(int socket);
		const int getSocket() const;
		std::string getUsername() const;
		std::string getNickname() const;
		void setUsername(std::string username);
		void setNickname(std::string nickname);
};