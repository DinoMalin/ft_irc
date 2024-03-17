#pragma once
#include "header.hpp"
#include "Client.hpp"

class Client;

class Server {
    private:
        int _socket;
        std::string _password;
        Client _clients[MAX_CLIENTS];
	    struct sockaddr_in _serverAddr;
        struct sockaddr_in _clientAddr;
	    socklen_t _clientAddrSize;

	    struct pollfd _fds[MAX_CLIENTS + 1];
	    int _numClients;
        std::vector<Channel&> _channels;

        void treatNewConnexion();
        void receiveMessage(int index);
        void reply(Message message, std::string target);

        // ===== Command Handlers ===== //

        // == PING == //
        void handlePING(Client client, Message message);

        // == Client command == //
        void handlePASS(Client client, Message message);
        void handleNICK(Client client, Message message);
        void handleUSER(Client client, Message message);

        // == Channel command == //
        void handlePRIVMSG(Client client, Message message);
        void handleJOIN(Client client, Message message);
        void handlePART(Client client, Message message);
        void handleLIST(Client client, Message message);

        // == Operators command == //
        void handleKICK(Client client, Message message);
        void handleINVITE(Client client, Message message);
        void handleTOPIC(Client client, Message message);
        void handleMODE(Client client, Message message, char flag);

        Channel& getChannel(std::string name);
        Client& getClient(std::string nickname);
        bool clientExist(std::string nickname);
        bool channelExist(std::string channel);
    public:
        Server(std::string password);
        void run();
};