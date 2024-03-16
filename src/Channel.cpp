#include "Channel.hpp"

Channel::Channel(std::string name, ChannelType type) : _name(name), _type(type), _password("") {
}

Channel::Channel(std::string name, ChannelType type, std::string password) : _name(name), _type(type), _password(password) {
}

std::string Channel::getName() const {
    return _name;
}

std::string Channel::getType() const {
    return _type;
}

std::string Channel::getPassword() const {
    return _password;
}

void Channel::setName(std::string name) {
    _name = name;
}

void Channel::setType(ChannelType type) {
    _type = type;
}

void Channel::setPassword(std::string password) {
    _password = password;
}

void addClient(CLient& client) {
    _clients.push_back(client);
}

void removeClient(std::string username) {
    for (int i = 0; i < _clients.size; i++) {
        if (_clients[i].getUsername() == username) {
            _clients.erase(i);
        }
    }
}

void addOperator(CLient& client) {
    _operators.push_back(client);
}

void removeOperator(std::string username) {
    for (int i = 0; i < _operators.size; i++) {
        if (_operators[i].getUsername() == username) {
            _operators.erase(i);
        }
    }
}

void Channel::sendChannel(std::string message) {
    for (int i = 0; i < _client.size; i++)
        send(_client[i].getSocket(), message, strlen(message), 0);
}