#pragma once

#include <iostream>
#include "Channel.hpp"

class Channel;

class Client {
	private:
		int _socket;
		std::string _username;
		std::string _nickname;
		std::string _hostname;
		std::string _ip;
	
		bool _registered;
		bool _quitting;

		bool _nicked;
		bool _usered;
		bool _welcomeSent;

	public:
		std::string		_buffer;

		Client();
		Client(int socket);
		~Client();
		int getSocket() const;

		bool getNicked();
		bool getUsered();
		bool getWelcomeSent();

		void setNicked();
		void setUsered();
		void setWelcomeSent();

		std::string getUsername() const;
		std::string getNickname() const;
		std::string getHostname() const;
		std::string getSource() const;
		std::string getIp() const;
		bool isQuitting() const;
		bool getRegistered() const;
		void quitting();
		void setUsername(std::string username);
		void setNickname(std::string nickname);
		void setHostname(std::string hostname);
		void setRegistered();
		void setIp(std::string ip);
		Client& operator=(const Client& src);
};