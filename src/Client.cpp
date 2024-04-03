#include "Client.hpp"

Client::Client() : _nickname("*"), _registered(false), _quitting(false), _nicked(false), _usered(false), _welcomeSent(false), _buffer("") {}

Client::Client(int socket) : _socket(socket), _nickname("*"), _registered(false), _quitting(false), _nicked(false), _usered(false), _welcomeSent(false), _buffer("") {}

Client::~Client() {}

int Client::getSocket() const {return _socket;}
bool Client::getRegistered() const {return _registered;}
bool Client::isQuitting() const {return _quitting;}
std::string Client::getUsername() const {return _username;}
std::string Client::getNickname() const {return _nickname;}
std::string Client::getHostname() const {return _hostname;}
std::string Client::getIp() const {return _ip;}

void Client::setUsername(std::string username) {_username = username;}
void Client::setNickname(std::string nickname) {_nickname = nickname;}
void Client::setRegistered() {_registered = true;}
void Client::setHostname(std::string hostname) {_hostname = hostname;}
void Client::quitting() {_quitting = true;}
void Client::setIp(std::string ip) {_ip = ip;}

bool Client::getNicked() {return _nicked;}
bool Client::getUsered() {return _usered;}
bool Client::getWelcomeSent() {return _welcomeSent;}

void Client::setNicked() {_nicked = true;}
void Client::setUsered() {_usered = true;}
void Client::setWelcomeSent() {_welcomeSent = true;}

Client& Client::operator=(const Client& src) {
    _socket = src._socket;
    _username = src._username;
    _nickname = src._nickname;
    _registered = src._registered;
    _quitting = src._quitting;
    _buffer = src._buffer;
    _nicked = src._nicked;
    _usered = src._usered;
    _welcomeSent = src._welcomeSent;
    return *this;
}

void Client::clear() {
    *this = Client();
}

std::string Client::getSource() const {
    return _nickname + "!" + _username + "@" + _ip;
}