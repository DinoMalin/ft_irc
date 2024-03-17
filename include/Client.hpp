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
		std::string _hostname;
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
		std::string getHostname() const;
		bool getRegistered() const;
		void setUsername(std::string username);
		void setNickname(std::string nickname);
		void setPassword(std::string password);
		void setHostname(std::string hostname);
		void setRegistered();
		Client& operator=(const Client& src);
};