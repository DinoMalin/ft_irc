#include "Channel.hpp"

Channel::Channel(std::string name, ChannelType type) : _name(name), _topic(""), _type(type), _password(""), _limited(false) {}
Channel::Channel(std::string name, ChannelType type, Client *op) : _name(name), _topic(""), _type(type), _password(""), _limited(false) {
    _operators.push_back(op);
}
Channel::~Channel() {}

std::string Channel::getName() const {return _name;}
ChannelType Channel::getType() const {return _type;}
std::string Channel::getTopic() const {return _topic;}
std::string Channel::getPassword() const {return _password;}

bool Channel::isInviteOnly() {return _inviteOnly;}
bool Channel::isLimited() const {return _limited;}
int Channel::getLimitUsers() const {return _limitUsers;}
int Channel::getNbClients() const {return _clients.size();}

void Channel::setInviteOnly(bool inv) {_inviteOnly = inv;}
void Channel::setName(std::string name) {_name = name;}
void Channel::setTopic(std::string topic) {_topic = topic;}
void Channel::setType(ChannelType type) {_type = type;}
void Channel::setPassword(std::string password) {_password = password;}
void Channel::addClient(Client *client) {_clients.push_back(client);}
void Channel::setLimitUsers(int limitUsers) {_limitUsers = limitUsers;}
void Channel::setLimited(bool limited) {_limited = limited;}

void Channel::removeClient(std::string nickname) {
    for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if ((*it)->getNickname() == nickname) {
            _operators.erase(it);
            return;
        }
    }
}

void Channel::addRegistered(Client *client) {
    _registered.push_back(client);
}

void Channel::removeRegistered(std::string nickname) {
    for (std::vector<Client*>::iterator it = _registered.begin(); it != _registered.end(); ++it) {
        if ((*it)->getNickname() == nickname) {
            _registered.erase(it);
            return;
        }
    }
}

void Channel::clearRegistered() {
    for (std::vector<Client*>::iterator it = _registered.begin(); it != _registered.end(); ++it) {
        _registered.erase(it);
        return;
    }
}

bool Channel::isRegistered(std::string nickname) {
    for (std::vector<Client*>::iterator it = _registered.begin(); it != _registered.end(); ++it) {
        if ((*it)->getNickname() == nickname)
            return true;
    }
    return false;
}

bool Channel::isInChannel(std::string nickname) {
    for (size_t i = 0; i < _clients.size(); i++) {
        if (_clients[i]->getNickname() == nickname)
            return true;
    }
    return false;
}
void Channel::addOperator(Client *client) {
    _operators.push_back(client);
}

void Channel::removeOperator(std::string nickname) {
    for (std::vector<Client *>::iterator it = _operators.begin(); it != _operators.end(); ++it) {
        if ((*it)->getNickname() == nickname) {
            _operators.erase(it);
            return;
        }
    }
}

bool Channel::isOperator(std::string nickname) {
    for (std::vector<Client *>::iterator it = _operators.begin(); it != _operators.end(); ++it) {
        if ((*it)->getNickname() == nickname)
            return true;
    }
    return false;
}

void Channel::sendChannel(std::string message) const {
    for (size_t i = 0; i < _clients.size(); i++)
        send(_clients[i]->getSocket(), message.c_str(), message.length(), 0);
}

bool Channel::getCanUseTopic() const {
    return _canUseTopic;
}

void Channel::setCanUseTopic(bool canUseTopic) {
    _canUseTopic = canUseTopic;
}
