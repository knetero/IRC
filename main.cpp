#include "server.hpp"
#include "client.hpp"
#include <cstdlib>
#include <sys/signal.h>
// #include <poll.h>

void close_all(std::map<int, Client>& clients_Map)
{
    for (size_t i = 0; i < clients_Map.size(); i++){
        close(clients_Map[i].clientSocket);
    }
}


void run_server(Server& server, Client& clients)
{
    struct pollfd serverSocket;
    serverSocket.fd = server.serverSocket;
    serverSocket.events = POLLIN;
    server.clientSockets.push_back(serverSocket);

    while(true)
    {
        int pollCount = poll(server.clientSockets.data(), server.clientSockets.size(), -1);
        if (pollCount == -1){
            std::cerr << "Poll failed" << std::endl;
            exit(1);
        }
        for(size_t i = 0; i < server.clientSockets.size(); i++)
        {
            server.isServerCommand = false;
            if (server.clientSockets[i].revents & POLLIN)
            {
                if(server.clientSockets[i].fd == server.serverSocket)
                {
                    server.socketId = server.acceptClient();
                    if(server.socketId == -1){
                        close_all(clients.clients_Map);
                        close(server.serverSocket);
                        // exit(0);
                    }
                    if(server.socketId  != -1){
                        clients.clients_Map[server.socketId] = clients;
                        struct pollfd clientSocket;
                        clientSocket.fd = server.socketId ;
                        clientSocket.events = POLLIN;
                        server.clientSockets.push_back(clientSocket);
                    }
                }
                else
                {
                    int bytesRead = recv(server.clientSockets[i].fd, server.buffer, sizeof(server.buffer), 0);
                    if(bytesRead == -1){
                        std::cerr << "Failed to read from client" << std::endl;
                        continue;
                    }
                    if(bytesRead == 0){
                        std::cout << "Client exiting from the Server" << std::endl;
                        close(server.clientSockets[i].fd);
                        server.clientSockets.erase(server.clientSockets.begin() + i);
                        clients.clients_Map.erase(server.clientSockets[i].fd);
                        continue;
                    }
                    clients.clientBuffers[server.clientSockets[i].fd].append(server.buffer, bytesRead);
                    size_t newlinepos;
                    while((newlinepos = clients.clientBuffers[server.clientSockets[i].fd].find("\n")) != std::string::npos) 
                    {
                        std::string command = clients.clientBuffers[server.clientSockets[i].fd].substr(0, newlinepos);
                        server.parse_commands(server.clientSockets[i].fd, command, clients.clients_Map);
                        clients.clientBuffers[server.clientSockets[i].fd].erase(0, newlinepos + 2);
                    }
                }
            }
        }
    }
}

int main(int ac , char** av)
{
    if (ac != 3) {
        std::cerr << "Usage: " << av[0] << " <port>" << " <password>"<< std::endl;
        exit(1);
    }
    if(!isdigit(*av[1])){
        std::cerr << "Invalid port number" << std::endl;
        exit(1);
    }
    Server server(atoi(av[1]), av[2]);
    Client clients(server.socketId);
    // signal(SIGINT, server.signalHandler);
    run_server(server, clients);
}