#include "Client.hpp"

Client::Client() : _registered(false), _triedToRegister(false), _quitting(false), _buffer("") {}

Client::Client(int socket) : _socket(socket), _registered(false), _triedToRegister(false), _quitting(false), _buffer("") {}

Client::~Client() {}

int Client::getSocket() const {return _socket;}
bool Client::getRegistered() const {return _registered;}
bool Client::triedToRegister() const {return _triedToRegister;}
bool Client::isQuitting() const {return _quitting;}
std::string Client::getUsername() const {return _username;}
std::string Client::getNickname() const {return _nickname;}
std::string Client::getPassword() const {return _password;}
std::string Client::getHostname() const {return _hostname;}
std::string Client::getIp() const {return _ip;}

void Client::setUsername(std::string username) {_username = username;}
void Client::setNickname(std::string nickname) {_nickname = nickname;}
void Client::setPassword(std::string password) {_password = password;}
void Client::setRegistered() {_registered = true;}
void Client::setHostname(std::string hostname) {_hostname = hostname;}
void Client::tryToRegister() {_triedToRegister = true;}
void Client::quitting() {_quitting = true;}
void Client::setIp(std::string ip) {_ip = ip;}

Client& Client::operator=(const Client& src) {
    _socket = src.getSocket();
    _username = src.getUsername();
    _nickname = src.getNickname();
    _password = src.getPassword();
    _triedToRegister = src._triedToRegister;
    _registered = src._registered;
    _quitting = src._quitting;
    _buffer = src._buffer;
    return *this;
}

void Client::clear() {
    *this = Client();
}

std::string Client::getSource() const {
    return _nickname + "!" + _username + "@" + _ip;
}