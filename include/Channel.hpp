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
		std::vector<std::string> _clients;
        std::vector<std::string> _registered;
        std::vector<std::string> _operators;

	public:
		Channel(std::string name);
        ~Channel();
		std::string getName() const;
        std::string getTopic() const;
        std::string getPassword() const;
        std::string getUserList();
        int getNbClients() const;
        int getLimitUsers() const;
        bool canUseTopic() const;
        bool isLimited() const;
        void setName(std::string name);
        void setTopic(std::string topic);
        void setCanUseTopic(bool canUseTopic);
        void setPassword(std::string password);
        void setLimitUsers(int limitUsers);
        void setLimited(bool limited);

        void addClient(std::string nickname);
        void removeClient(std::string username);
        bool isInChannel(std::string nickname);
    
        void addRegistered(std::string nickname);
        void removeRegistered(std::string username);
        void clearRegistered();
        bool isRegistered(std::string username);
    
    
        void addOperator(std::string nickname);
        void removeOperator(std::string username);
        bool isOperator(std::string username);
    
		void sendChannel(std::string message, Client author, std::vector<Client> clients, bool skipAuthor);
        void setInviteOnly(bool inv);
        bool isInviteOnly();

        void renameClient(std::string oldNick, std::string newNick);

        std::string getModes();
};