#include "Client.hpp";

Client::Client(int socket) : _socket(socket) {
}


const int Client::getSocket() const {
    return _socket;
}

std::string Client::getUsername() const {
    return _username;
}

std::string Client::getNickname() const {
    return _nickname;
}

void Client::setUsername(std::string username) {
    _username = username;
}

void Client::setNickname(std::string nickname) {
    _nickname = nickname;
}