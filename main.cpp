
#include "client.hpp"
#include <cstdlib>
#include <sys/signal.h>
#include "server.hpp"
// #include <poll.h>

void close_all(std::map<int, Client>& clients_Map)
{
    for (size_t i = 0; i < clients_Map.size(); i++){
        close(clients_Map[i].clientSocket);
    }
}


void AcceptNewClient(Server& server)
{
    server.socketId = server.acceptClient();
    if(server.socketId == -1){
        // close_all(clients.clients_Map);
        close(server.serverSocket);
        exit(0);
    }
    if(server.socketId  != -1)
    {
        struct pollfd clientSocket;
        clientSocket.fd = server.socketId ;
        clientSocket.events = POLLIN;
        server.clientSockets.push_back(clientSocket);
        // clients.clients_Map[server.socketId] = clients;

        // create a new client object, and push it to the map;
        Client *client = new Client(server.socketId);
        server.serverClients.insert(std::make_pair(server.socketId, client));
        server.serverClients[server.socketId]->clientAdress = server.clientAdress;
    }
}

void ReceiveData(Server& server, size_t i)
{
    int bytesRead = recv(server.clientSockets[i].fd, server.buffer, sizeof(server.buffer), 0);
    if(bytesRead == -1)
    {
        std::cerr << "Failed to read from client" << server.clientSockets[i].fd << std::endl;
        return;
    }
    if(bytesRead == 0)
    {
        std::cout << RED << "[-] Client disconnected, client fd: " << RESET << server.clientSockets[i].fd << std::endl;
        server.serverClients.erase(server.serverClients.find(server.clientSockets[i].fd));
        close(server.clientSockets[i].fd);
        // server.clientSockets.erase(server.clientSockets.begin() + i);
        return;
    }
    // std::cout << "actual server clients" << server.serverClients.size() << std::endl;
    Client *client = server.serverClients[server.clientSockets[i].fd];
    client->buffer.append(server.buffer, bytesRead);
    // clients.clientBuffers[server.clientSockets[i].fd].append(server.buffer, bytesRead);
    size_t newlinepos;
    while((newlinepos = client->buffer.find("\n")) != std::string::npos) 
    {
        std::string command = client->buffer.substr(0, newlinepos);
        server.parse_commands(client, command);
        client->buffer.erase(0, newlinepos + 2);
        // clients.clientBuffers[server.clientSockets[i].fd].erase(0, newlinepos + 2);
    }
}

void run_server(Server& server)
{
    struct pollfd serverSocket;
    serverSocket.fd = server.serverSocket;
    serverSocket.events = POLLIN;
    server.clientSockets.push_back(serverSocket);

    while(true)
    {
        //
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
                // Checks if the file descriptor is the server socket. If so, it call AcceptNewClient() to accept a new client connection
                if(server.clientSockets[i].fd == server.serverSocket)
                    AcceptNewClient(server);
                // Otherwise, it calls ReceiveNewData() to receive new data from a registered client
                else
                    ReceiveData(server, i);
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
    // signal(SIGINT, server.signalHandler);
    run_server(server);
}