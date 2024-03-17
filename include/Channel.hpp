#pragma once

#include "header.hpp"
#include <iostream>
#include "Client.hpp"

class Client;

enum ChannelType {
    PUBLIC,
    PRIVATE,
    SECRET
};

class Channel {
	private:
    	std::string _name;
        std::string _topic;
		ChannelType _type;
        std::string _password;
		std::vector<Client&> _clients;
        std::vector<Client&> _operators;
	public:
		Channel(std::string name, ChannelType type);
        Channel(std::string name, ChannelType type, std::string password);
        ~Channel();
		std::string getName() const;
        std::string getTopic() const;
        ChannelType getType() const;
        std::string getPassword() const;
        void setName(std::string name);
        void setTopic(std::string topic);
        void setType(ChannelType type);
        void setPassword(std::string password);
        void addClient(Client& client);
        void removeClient(std::string username);
        void addOperator(Client& client);
        void removeOperator(std::string username);
        bool isOperator(std::string username) const;
		void sendChannel(std::string message) const;
};