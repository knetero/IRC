#include "client.hpp"
#include "server.hpp"
#include <cstddef>

Client::Client(){
    this->passSet = false;
    this->nickSet = false;
    this->userSet = false;
    this->isRegistered = false;
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

bool Client::checkRegistration()
{
    if(this->nickSet && this->userSet && this->passSet && !this->isRegistered)
        isRegistered = true;
    return isRegistered;
}


void Client::close_fd(){
    for (size_t i = 0; i < clients_Map.size(); i++){
        close(clients_Map[i].clientSocket);
    }
    for (size_t i = 0; i < clients_Map.size(); i++){
        clients_Map.clear();
    }
}


int Client::get_client_socket(){
    return clientSocket;
}

Client::~Client(){
    close(clientSocket);
}