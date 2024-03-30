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
		std::string _source;
		Channel *_channel;
		std::string _ip;

		bool _registered;
		bool _triedToRegister;
		bool _quitting;
	public:
		std::string		_buffer;

		Client();
		Client(int socket);
		~Client();
		int getSocket() const;
		std::string getUsername() const;
		std::string getNickname() const;
		std::string getPassword() const;
		std::string getHostname() const;
		std::string getSource() const;
		std::string getIp() const;
		bool isQuitting() const;
		bool getRegistered() const;
		bool triedToRegister() const;
		void quitting();
		void setUsername(std::string username);
		void setNickname(std::string nickname);
		void setPassword(std::string password);
		void setHostname(std::string hostname);
		void setSource(std::string source);
		void tryToRegister();
		void setRegistered();
		void setIp(std::string ip);
		void clear();
		Client& operator=(const Client& src);
};