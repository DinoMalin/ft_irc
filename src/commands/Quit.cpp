#include "Server.hpp"

void Server::handleQUIT(Client &client, Message message) {
    (void)message;
    client.quitting();
}