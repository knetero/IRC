#include <csignal>
#include <cstddef>
#include <sys/_types/_fd_def.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> 
#include <cstring> 
#include <iostream>
#include <vector>
#include "server.hpp"
#include "client.hpp"
#include <poll.h>



int main(int ac , char** av)
{
    if (ac != 3) {
        std::cerr << "Usage: " << av[0] << " <port>" << " <password>"<< std::endl;
        exit(1);
    }

    Server server(atoi(av[1]), av[2]);
    Client clients(server.socketId);
    // signal(SIGINT, server.signalHandler);
    // signal(SIGQUIT, server.signalHandler);


    struct pollfd serverSocket;
    serverSocket.fd = server.serverSocket;
    serverSocket.events = POLLIN;
    server.clientSockets.push_back(serverSocket);

    while(server.signal == false)
    {
        int pollCount = poll(server.clientSockets.data(), server.clientSockets.size(), -1);
        if(pollCount == -1 && server.signal == false){
            std::cerr << "Poll failed" << std::endl;
            exit(1);
        }
        for(size_t i = 0; i < server.clientSockets.size(); i++)
        {
            server.isServerCommand = false;
            if(server.clientSockets[i].revents & POLLIN)
            {
                if(server.clientSockets[i].fd == server.serverSocket)
                {
                    server.socketId = server.acceptClient();
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
                        std::cout << "Client exiting from the network" << std::endl;
                        close(server.clientSockets[i].fd);
                        server.clientSockets.erase(server.clientSockets.begin() + i);
                        clients.clients_Map.erase(server.clientSockets[i].fd);
                        continue;
                    }
                    server.buffer[bytesRead] = '\0';
                    std::string str(server.buffer);
                    server.parse_commands(server.clientSockets[i].fd, server.buffer, clients.clients_Map);
                }
            }
        }
    }
    // if(server.signal == true)
    // {
    //     clients.close_fd();
    //     close(server.serverSocket);
    //     std::cout << "The Server is Closed!" << std::endl;
    // }
}