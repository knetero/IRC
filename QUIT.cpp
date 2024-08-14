#include "server.hpp"

void Server::brodcastQuit(Client *client, Channel *channel, std::string &message)
{
    std::map<int, Client *>::iterator it;
    for (it = channel->getmembers().begin(); it != channel->getmembers().end(); it++)
    {
        if (it->first != client->clientSocket)
            sendData(it->first, QUIT(client->nickname, client->username, getIp(clientAdress), message));
    }
}

void    Server::quit(Client *client, std::vector<std::string> &parameters)
{
    // int ClientFd = client->clientSocket;
    std::string message = "";
    int size ;
    if (parameters.size() == 2)
        message = ":Quit: " + parameters[1];
    else
        message = "Quit";
    for (size_t i = 0; i < client->joinedChannels.size(); i++)
    {
        brodcastQuit(client, client->joinedChannels[i], message);
        size = server_channels.find(client->joinedChannels[i]->getName())->second->getSize() - 1;
        server_channels.find(client->joinedChannels[i]->getName())->second->setSize(size);
    }
    sendData(client->clientSocket, QUIT(client->nickname, client->username, getIp(client->clientAdress), message));
    sendData(client->clientSocket, ERROR(std::string("Quit")));
    removeClientFromServer(client);
    std::cout << RED << "[-] Client disconnected, client fd: " << RESET << client->clientSocket << std::endl;
}
