#pragma once

#include "header.hpp"
#include <iostream>
#include "Client.hpp"

class Client;

class Channel {
	private:
    	std::string _name;
        std::string _topic;
        std::string _password;
        int _limitUsers;
        bool _limited;
        bool _canUseTopic;
        bool _inviteOnly;
		std::vector<Client*> _clients;
        std::vector<Client*> _registered;
        std::vector<Client*> _operators;

	public:
		Channel(std::string name);
        Channel(std::string name, Client *op);
        ~Channel();
		std::string getName() const;
        std::string getTopic() const;
        std::string getPassword() const;
        std::string getUserList();
        int getNbClients() const;
        int getLimitUsers() const;
        bool getCanUseTopic() const;
        bool isLimited() const;
        void setName(std::string name);
        void setTopic(std::string topic);
        void setCanUseTopic(bool canUseTopic);
        void setPassword(std::string password);
        void setLimitUsers(int limitUsers);
        void setLimited(bool limited);
        void addClient(Client *client);
        void removeClient(std::string username);
        void addRegistered(Client *client);
        void removeRegistered(std::string username);
        void clearRegistered();
        bool isRegistered(std::string username);
        bool isInChannel(std::string nickname);
        void addOperator(Client *client);
        void removeOperator(std::string username);
        bool isOperator(std::string username);
		void sendChannel(std::string message, Client author, bool skipAuthor) const;
        void setInviteOnly(bool inv);
        bool isInviteOnly();
        void rearrangeUsers(Client *disconnected);
};