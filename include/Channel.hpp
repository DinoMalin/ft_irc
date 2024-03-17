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
        std::vector<Client&> _registered;
        std::vector<Client&> _operators;
        bool _canUseTopic;

        int _limitUsers;
        bool _limited;
        bool _inviteOnly;
	public:
		Channel(std::string name, ChannelType type);
        Channel(std::string name, ChannelType type, Client& op);
        ~Channel();
		std::string getName() const;
        std::string getTopic() const;
        ChannelType getType() const;
        std::string getPassword() const;
        int getNbClients() const;
        int getLimitUsers() const;
        bool getCanUseTopic() const;
        bool isLimited() const;
        void setName(std::string name);
        void setTopic(std::string topic);
        void setType(ChannelType type);
        void setCanUseTopic(bool canUseTopic);
        void setPassword(std::string password);
        void setLimitUsers(int limitUsers);
        void setLimited(bool limited);
        void addClient(Client& client);
        void removeClient(std::string username);
        void addRegistered(Client& client);
        void removeRegistered(std::string username);
        void clearRegistered();
        bool isRegistered(std::string username);
        bool isInChannel(std::string nickname);
        void addOperator(Client& client);
        void removeOperator(std::string username);
        bool isOperator(std::string username) const;
		void sendChannel(std::string message) const;
        void setInviteOnly(bool inv);
        bool isInviteOnly();
};