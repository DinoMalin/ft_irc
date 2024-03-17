#include "Channel.hpp"

Channel::Channel(std::string name, ChannelType type) : _name(name), _type(type), _password("") {
}

Channel::Channel(std::string name, ChannelType type, std::string password) : _name(name), _type(type), _password(password) {
}

Channel::~Channel() {
}

std::string Channel::getName() const {
    return _name;
}

std::string Channel::getName() const {
    return _name;
}

std::string Channel::getTopic() const {
    return _topic;
}

std::string Channel::getPassword() const {
    return _password;
}

void Channel::setName(std::string name) {
    _name = name;
}

void Channel::setTopic(std::string topic) {
    _topic = topic;
}

void Channel::setType(ChannelType type) {
    _type = type;
}

void Channel::setPassword(std::string password) {
    _password = password;
}

void Channel::addClient(Client& client) {
    _clients.push_back(client);
}

void Channel::removeClient(std::string username) {
    for (std::vector<Client&>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->getUsername() == username) {
            _operators.erase(it);
            return;
        }
    }
}

void Channel::addOperator(Client& client) {
    _operators.push_back(client);
}

void Channel::removeOperator(std::string username) {
    for (std::vector<Client&>::iterator it = _operators.begin(); it != _operators.end(); ++it) {
        if (it->getUsername() == username) {
            _operators.erase(it);
            return;
        }
    }
}

bool Channel::isOperator(std::string username) const {
    for (std::vector<Client&>::iterator it = _operators.begin(); it != _operators.end(); ++it) {
        if (it->getUsername() == username)
            return true;
    }
    return false;
}

void Channel::sendChannel(std::string message) const {
    for (int i = 0; i < _clients.size(); i++)
        send(_clients[i].getSocket(), message.c_str(), message.length(), 0);
}