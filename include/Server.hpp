#pragma once
#include "header.hpp"

class Server {
    private:
        int serverSocket;
        int clientSockets[MAX_CLIENTS];
	    struct sockaddr_in serverAddr;
        struct sockaddr_in clientAddr;
	    socklen_t clientAddrSize;

	    struct pollfd fds[MAX_CLIENTS + 1];
	    int numClients;

        void treatNewConnexion();
        void receiveMessage(int index);
        void reply(Message message, std::string target);

        void handlePING(Client client, Message message);
        void handleKICK(Client client, Message message);
        void handleINVITE(Client client, Message message);
        void handleTOPIC(Client client, Message message);
        void handleMODE(Client client, Message message, char flag);
    public:
        Server();
        void run();
};