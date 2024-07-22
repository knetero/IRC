#include "client.hpp"
#include "server.hpp"
#include <cstddef>

Client::Client(){
    this->passSet = false;
    this->nickSet = false;
    this->userSet = false;
    this->isRegistered = false;
    this->welcome_msg = 1;
}

Client::Client(int clientSocket){
    this->clientSocket = clientSocket;
}

Client::Client(const Client& other){
    this->clientSocket = other.clientSocket;
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
