#include "Channel.hpp"

Channel::Channel(std::string name) : _name(name), _topic(""), _password(""), _limited(false), _canUseTopic(true), _inviteOnly(false) {}
Channel::Channel(std::string name, Client *op) : _name(name), _topic(""), _password(""), _limited(false), _canUseTopic(true), _inviteOnly(false) {
    _registered.push_back(op);
    _clients.push_back(op);
    _operators.push_back(op);
}
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
void Channel::addClient(Client *client) {_clients.push_back(client);}
void Channel::setLimitUsers(int limitUsers) {_limitUsers = limitUsers;}
void Channel::setLimited(bool limited) {_limited = limited;}

std::string Channel::getUserList() {
    std::string result("");

    for (size_t i = 0; i < _clients.size(); i++) {
        if (isOperator(_clients[i]->getNickname()))
            result += "@";
        result += _clients[i]->getNickname();
        if (i != _clients.size() - 1)
            result += " ";
    }

    return result;
}

void Channel::removeClient(std::string nickname) {
    for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end();) {
        if ((*it)->getNickname() == nickname) {
            removeOperator((*it)->getNickname());
            it = _clients.erase(it);
        } else {
            ++it;
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

void Channel::sendChannel(std::string message, Client author, bool skipAuthor) const {
    for (size_t i = 0; i < _clients.size(); i++)
        if (!(skipAuthor && author.getNickname() == _clients[i]->getNickname())) {
            std::cout << _clients[i]->getNickname();
            send(_clients[i]->getSocket(), message.c_str(), message.length(), 0);
            std::cout << " | SENDING" << std::endl;
        }
}

bool Channel::getCanUseTopic() const {
    return _canUseTopic;
}

void Channel::setCanUseTopic(bool canUseTopic) {
    _canUseTopic = canUseTopic;
}
