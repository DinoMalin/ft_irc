#pragma once

#include <iostream>
#include "Channel.hpp"

class Channel;

class Client {
	private:
		int _socket;
		std::string _username;
		std::string _nickname;
		std::string _password;
		Channel *_channel;

		bool _registered;
	public:
		Client();
		Client(int socket);
		~Client();
		int getSocket() const;
		std::string getUsername() const;
		std::string getNickname() const;
		std::string getPassword() const;
		std::string getRegister() const;
		void setUsername(std::string username);
		void setNickname(std::string nickname);
		void setPassword(std::string password);
		void setRegistered();
		Client& operator=(const Client& src);
};