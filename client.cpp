#include "client.hpp"
#include "server.hpp"
#include <cstddef>

Client::Client()
{
    this->passSet = false;
    this->nickSet = false;
    this->userSet = false;
    this->isRegistered = false;
}

Client::Client(int clientSocket)
{
    this->clientSocket = clientSocket;
    this->passSet = false;
    this->nickSet = false;
    this->userSet = false;
    this->isRegistered = false;
    this->totalXps = 0;
}

Client::Client(const Client& other){
    this->clientSocket = other.clientSocket;
    this->nickname = other.nickname;
    this->username = other.username;
    this->realname = other.realname;
}

Client& Client::operator=(const Client& other){
    if(this != &other){
        this->clientSocket = other.clientSocket;
    }
    return *this;
}

int Client::get_client_socket(){
    return clientSocket;
}

Client::~Client(){
}
