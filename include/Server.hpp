#pragma once
#include "header.hpp"
#include "Client.hpp"
#include <map>

class Client;

class Server {
    private:
        typedef void (Server::*Funcs) (Client &, Message);
        socklen_t _clientAddrSize;
        int _socket;
        std::string _password;
        Client _clients[MAX_CLIENTS];
        std::vector<Channel*> _allChannels;
	    struct sockaddr_in _serverAddr;
        struct sockaddr_in _clientAddr;

        std::map<std::string, Funcs> _stringToFunc;

	    struct pollfd _fds[MAX_CLIENTS + 1];
	    int _numClients;
        std::vector<Channel*> _channels;
        std::string _buffer;

        void treatNewConnexion();
        void receiveMessage(int index);
        bool commandsIsImplemented(std::string str);
        void disconnectClient(int index);
        void disconnectEveryone();

        // ===== Command Handlers ===== //

        // == PING == //
        void handlePING(Client &client, Message message);

        // == Client command == //
        void handleCAP(Client &client, Message message);
        void handlePASS(Client &client, Message message);
        void handleNICK(Client &client, Message message);
        void handleUSER(Client &client, Message message);
        void handleQUIT(Client &client, Message message);

        // == Channel command == //
        void handlePRIVMSG(Client &client, Message message);
        void handleJOIN(Client &client, Message message);
        void handlePART(Client &client, Message message);
        void handleLIST(Client &client, Message message);

        // == Operators command == //
        void handleKICK(Client &client, Message message);
        void handleINVITE(Client &client, Message message);
        void handleTOPIC(Client &client, Message message);
        void handleMODE(Client &client, Message message);

        Channel& getChannel(std::string name);
        Client& getClient(std::string nickname);
        bool clientExist(std::string nickname);
        bool channelExist(std::string channel);
        void reply(Client &client, Message message);
        
        void initFuncs();
    public:
        ~Server();
        Server(int port, std::string password);
        void run();
};
