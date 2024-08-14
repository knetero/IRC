#include "client.hpp"
#include "channel.hpp"
#include <cctype>
#include <cstddef>
#include "server.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "replies.hpp"
#include "convert.hpp"

Server::Server(int port, const std::string& password)
{
    this->port = port;

    this->password = password;
    if(!initialize())
    {
        std::cerr << "Failed to initialize the server" << std::endl;
        exit(1);
    }
    std::cout << "Server IP: " << GREEN << getServerIp() << RESET << std::endl;
    std::cout << "Server is listening on port: " << GREEN << port << RESET << std::endl;
    this->isServerCommand = false;
    this->isSetNick  = false;
    this->isSetPass = false;
    this->isSetUser = false;

}

bool Server::initialize()
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create a socket. errno: " << std::endl;
        return false;
    }
    struct sockaddr_in serverAddress1;
    serverAddress1.sin_family = AF_INET; 
    serverAddress1.sin_addr.s_addr = INADDR_ANY; 
    serverAddress1.sin_port = htons(port); 
    int enable = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1) { //
        perror("setsockopt");
        exit(1);
    }
    if(fcntl(serverSocket, F_SETFL, O_NONBLOCK) < 0)
    { 
        perror("fcntl");
        exit(1);
    }
    if (bind(serverSocket, (struct sockaddr*)&serverAddress1, sizeof(serverAddress1)) == -1) { 
        std::cerr << "Failed to bind to port " << port << ". errno: " << std::endl;
        return false;
    }
    if (listen(serverSocket, 5) == -1)
    {
        std::cerr << "Failed to listen on socket. errno: " << std::endl;
        return false;
    }
    this->startdate = new char[100];
    getDate(&startdate);
    return true;
}

void Server::run_server(void)
{
    struct pollfd serverSocket;
    serverSocket.fd = this->serverSocket;
    serverSocket.events = POLLIN;
    this->clientSockets.push_back(serverSocket);

    while(true)
    {
        int pollCount = poll(this->clientSockets.data(), this->clientSockets.size(), -1);
        if (pollCount == -1)
        {
            std::cerr << "Poll failed" << std::endl;
            exit(1);
        }
        for(size_t i = 0; i < this->clientSockets.size(); i++)
        {
            if (this->clientSockets[i].revents & POLLIN)
            {
                if(this->clientSockets[i].fd == this->serverSocket)
                    acceptClient();
                else
                    ReceiveData(i);
            }
        }
    }
}

void Server::acceptClient() 
{
    struct sockaddr_in clientAddress; 
    socklen_t clientAddressLength = sizeof(clientAddress); 
    this->clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
    this->clientAdress = clientAddress;
    if (clientSocket == -1) 
    {
        std::cerr << "Failed to accept client connection. errno: " << std::endl;
    }
    if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) < 0) {
        std::cerr << "Failed to set client socket to non-blocking mode." << std::endl;
        close(clientSocket);
    }
    std::cout << GREEN << "[+] Client connected, client fd: " << RESET << clientSocket << std::endl;

    struct pollfd clientSocket;
    clientSocket.fd = this->clientSocket ;
    clientSocket.events = POLLIN;
    this->clientSockets.push_back(clientSocket);
    // create a new client object, and push it to the map;
    Client *client = new Client(this->clientSocket);
    this->serverClients.insert(std::make_pair(this->clientSocket, client));
    this->serverClients[this->clientSocket]->clientAdress = this->clientAdress;
}

void Server::ReceiveData(size_t i)
{
    int bytesRead = recv(clientSockets[i].fd, buffer, sizeof(buffer), 0);
    if(bytesRead == -1)
    {
        std::cerr << "Failed to read from client" << clientSockets[i].fd << std::endl;
        return;
    }
    if(bytesRead == 0)
    {
        std::cout << RED << "[-] Client disconnected, client fd: " << RESET << clientSockets[i].fd << std::endl;
        removeClientFromServer(this->serverClients[clientSockets[i].fd]);
        return;
    }
    Client *client = serverClients[clientSockets[i].fd];
    client->buffer.append(buffer, bytesRead);
    size_t newlinepos;
    while((newlinepos = client->buffer.find("\n")) != std::string::npos) 
    {
        std::string command = client->buffer.substr(0, newlinepos);
        parse_commands(client, command);
        client->buffer.erase(0, newlinepos + 2);
    }
}

void Server::parse_commands(Client *client, std::string& data)
{
    data.erase(std::remove(data.begin(), data.end(), '\r'), data.end());
    data.erase(std::remove(data.begin(), data.end(), '\n'), data.end());
    std::vector<std::string> parameters = splitParameters(data);
    if (parameters.size() == 0)
        return ;
    parameters[0] = toUpperCase(parameters[0]);
    ////
    if (parameters[0] == "PASS")
        passCommand(client, parameters);
    else if (parameters[0] == "NICK")
        nickCommand(client, parameters);
    else if (parameters[0] == "USER")
        userCommand(client, parameters);
    else if(parameters[0] == "PRIVMSG")
        privmsgCommand(client, parameters);
    else if (parameters[0] == "INVITE")
        inviteCommand(client, parameters);
    else if (parameters[0] == "KICK")
        kickCommand(client, parameters);
    else if (parameters[0] == "TOPIC")
        topicCommand(client, parameters);
    else if (parameters[0] == "JOIN")
        join(data.substr(4), client);
    else if (parameters[0] == "MODE")
        mode(data.substr(4), client);
    else if (parameters[0] == "BOT")
        bot(client);
    else if (parameters[0] == "PONG")
        return ;
    else if (parameters[0] == "PART")
        part(client, parameters);
    else if (parameters[0] == "QUIT")
        quit(client, parameters);
    else
        sendData(client->get_client_socket(), ERR_CMDNOTFOUND(client->nickname, parameters[0]));
}

Server::~Server()
{
    // shutdown();
}
