#include "Channel.hpp"

Channel::Channel(std::string name) : _name(name), _topic(""), _password(""), _limited(false), _canUseTopic(true), _inviteOnly(false) {}
Channel::~Channel() {}

std::string Channel::getName() const {return _name;}
std::string Channel::getTopic() const {return _topic;}
std::string Channel::getPassword() const {return _password;}

bool Channel::isInviteOnly() {return _inviteOnly;}
bool Channel::isLimited() const {return _limited;}
int Channel::getLimitUsers() const {return _limitUsers;}
int Channel::getNbClients() const {return _clients.size();}

void Channel::setInviteOnly(bool inv) {_inviteOnly = inv;}
void Channel::setName(std::string name) {_name = name;}
void Channel::setTopic(std::string topic) {_topic = topic;}
void Channel::setPassword(std::string password) {_password = password;}
void Channel::addClient(std::string nickname) {_clients.push_back(nickname);}
void Channel::setLimitUsers(int limitUsers) {_limited = true; _limitUsers = limitUsers;}
void Channel::setLimited(bool limited) {_limited = limited;}

std::string Channel::getUserList() {
    std::string result("");

    for (size_t i = 0; i < _clients.size(); i++) {
        if (isOperator(_clients[i]))
            result += "@";
        result += _clients[i];
        if (i != _clients.size() - 1)
            result += " ";
    }

    return result;
}

void Channel::removeClient(std::string nickname) {
    for (size_t i = 0; i < _clients.size(); i++) {
        if (_clients[i] == nickname) {
            removeOperator(_clients[i]);
            removeRegistered(_clients[i]);
            _clients.erase(_clients.begin() + i);
            return;
        }
    }
}

void Channel::addRegistered(std::string nickname) {
    _registered.push_back(nickname);
}

void Channel::removeRegistered(std::string nickname) {
    for (size_t i = 0; i < _registered.size(); ++i) {
        if (_registered[i] == nickname) {
            _registered.erase(_registered.begin() + i);
            return;
        }
    }
}

void Channel::clearRegistered() {
    for (size_t i = 0; i < _registered.size(); ++i) {
        _registered.erase(_registered.begin() + i);
        return;
    }
}

bool Channel::isRegistered(std::string nickname) {
    for (size_t i = 0; i < _registered.size(); ++i) {
        if (_registered[i] == nickname)
            return true;
    }
    return false;
}

bool Channel::isInChannel(std::string nickname) {
    for (size_t i = 0; i < _clients.size(); i++) {
        if (_clients[i] == nickname)
            return true;
    }
    return false;
}
void Channel::addOperator(std::string nickname) {
    _operators.push_back(nickname);
}

void Channel::removeOperator(std::string nickname) {
    for (size_t i = 0; i < _operators.size(); ++i) {
        if (_operators[i] == nickname) {
            _operators.erase(_operators.begin() + i);
            return;
        }
    }
}

bool Channel::isOperator(std::string nickname) {
    for (size_t i = 0; i < _operators.size(); ++i) {
        if (_operators[i] == nickname)
            return true;
    }
    return false;
}

void Channel::sendChannel(std::string message, Client author, std::vector<Client> clients, bool skipAuthor) {
    for (size_t i = 0; i < clients.size(); i++) {
        if (isInChannel(clients[i].getNickname())) {
            if (!(skipAuthor && author.getNickname() == clients[i].getNickname()))
                send(clients[i].getSocket(), message.c_str(), message.length(), MSG_DONTWAIT | MSG_NOSIGNAL);
        }
    }
}

bool Channel::getCanUseTopic() const {
    return _canUseTopic;
}

void Channel::setCanUseTopic(bool canUseTopic) {
    _canUseTopic = canUseTopic;
}

void Channel::renameClient(std::string oldNick, std::string newNick) {
    for (size_t i = 0; i < _clients.size(); i++) {
        if (_clients[i] == oldNick)
            _clients[i] = newNick;
    }
    for (size_t i = 0; i < _registered.size(); i++) {
        if (_registered[i] == oldNick)
            _registered[i] = newNick;
    }
    for (size_t i = 0; i < _operators.size(); i++) {
        if (_operators[i] == oldNick)
            _operators[i] = newNick;
    }
}