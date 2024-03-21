#include "Client.hpp"

Client::Client() {}

Client::Client(int socket) : _socket(socket), _registered(false), _triedToRegister(false) {}

Client::~Client() {}

int Client::getSocket() const {return _socket;}
bool Client::getRegistered() const {return _registered;}
bool Client::triedToRegister() const {return _triedToRegister;}
std::string Client::getUsername() const {return _username;}
std::string Client::getNickname() const {return _nickname;}
std::string Client::getPassword() const {return _password;}
std::string Client::getHostname() const {return _hostname;}
std::string Client::getSource() const {return _source;}

void Client::setUsername(std::string username) {_username = username;}
void Client::setNickname(std::string nickname) {_nickname = nickname;}
void Client::setPassword(std::string password) {_password = password;}
void Client::setRegistered() {_registered = true;}
void Client::setHostname(std::string hostname) {_hostname = hostname;}
void Client::setSource(std::string source) {_source = source;}
void Client::tryToRegister() {_triedToRegister = true;}

Client& Client::operator=(const Client& src) {
    _socket = src.getSocket();
    _username = src.getUsername();
    _nickname = src.getNickname();
    _password = src.getPassword();
    return *this;
}

void Client::clear() {
    *this = Client();
}
