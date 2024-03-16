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
		std::vector<Client&> clients;
        std::vector<Client&> operators;
	public:
		Channel(std::string name, ChannelType type);
        Channel(std::string name, ChannelType type, std::string password);
		std::string getName() const;
        std::string getType() const;
        std::string getPassword() const;
        void setName(std::string name);
        void setTopic(std::string name);
        void setType(ChannelType type);
        void setPassword(std::string password);
        void addClient(CLient& client);
        void removeClient(std::string username);
        void addOperator(CLient& client);
        void removeOperator(std::string username);
		void sendChannel(std::string message);
};