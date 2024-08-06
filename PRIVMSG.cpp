#include "server.hpp"

<<<<<<< HEAD
void Server::privmsgCommand(Client *client, std::vector<std::string> &parameters)
{
    if (client->isRegistered)
    {
        if (parameters.size() == 3)
        {
            std::vector<std::string>    targets;
            std::istringstream          ss(parameters[1]);
            std::string                 target;
            while (getline(ss, target, ','))
                targets.push_back(target);
    
            for (int i = 0; i < (int)targets.size(); i++)
            {
                if (targets[i][0] == '#' && server_channels.find(targets[i].substr(1)) != server_channels.end()) // if target is a channel and exist in server
                {
                    Channel &channel = *server_channels[targets[i].substr(1)];

                    if (channel.clientExist(client->nickname) != -1)
                    {
                        std::map<int, Client *> members = channel.getmembers();
                        std::map<int, Client *>::iterator it;
                        for (it = members.begin(); it != members.end(); it++)
                        {
                            if (it->second->get_client_socket() != client->get_client_socket())
                                sendData(it->second->get_client_socket(), \
                                PRIVMSG(client->nickname, client->username, getIp(client->clientAdress), targets[i], parameters[2]));
                        }
                    }
                    else
                        sendData(client->get_client_socket(), ERR_CANNOTSENDTOCHAN(client->nickname, targets[i]));
                }
                else
                {
                    sendData(getClientFd(parameters[1]), \
                    PRIVMSG(client->nickname, client->username, getIp(client->clientAdress), targets[i], parameters[2]));
=======
void Server::sendData(int fd, std::string data)
{
    send(fd, data.c_str(), data.size(), 0);
}

int getClientFd(std::string target, std::map<int, Client>& allClients)
{
    for (int i = 0; i < (int)allClients.size(); i++)
    {
        if (allClients[i].nickname == target)
            return (allClients[i].clientSocket);
    }
    return -1;
}

void Server::privmsgCommand(int clientSocket, std::map<int, Client>& allClients, std::string params)
{
    Client &client = allClients[clientSocket];
    if (client.passSet /*&& client.nickSet && client.userSet*/)
    {
        std::vector<std::string> parameters;
        parameters.push_back(params.substr(0, params.find(' '))); // Target
        parameters.push_back(params.substr(params.find(' '))); // Msg
        if (parameters[1][0] == ':' && parameters[1].find(' ') != std::string::npos)
            parameters[1] = parameters[1].substr(1);
        if (parameters.size() == 2)
        {
            std::vector<std::string> targets;
            std::istringstream ss(parameters[0]);
            std::string target;
            while (getline(ss, target, ','))
                targets.push_back(target);
            parameters[1] = parameters[1].substr(1);
            for (int i = 0; i < (int)targets.size(); i++)
            {
                if (targets[i][0] == '#' && server_channels.find(targets[i].substr(1)) != server_channels.end()) // if target is a channel
                {
                    std::map<std::string, Channel>::iterator channel = server_channels.find(targets[i].substr(1));
                    std::map<int, Client *> *members = channel->second.getmembers();
                    std::map<int, Client *>::iterator it;
                    for (it = members->begin(); it != members->end(); it++)
                    {
                        if (it->second->get_client_socket() != clientSocket)
                            sendData(it->second->get_client_socket(), PRIVMSG(client.nickname, client.username, "localhost", targets[i], parameters[1]));
                    }
                }
                else
                {
                    sendData(getClientFd(parameters[0], allClients), PRIVMSG(client.nickname, client.username, "localhost", targets[i], parameters[1]));
>>>>>>> 858be78f1c14a9e55cae2f94e6ebc520112464a2
                }
            }
        }
        else
<<<<<<< HEAD
            sendData(client->get_client_socket(), ERR_NOTENOUGHPARAM(client->nickname));
    }
    else
    {
        sendData(client->get_client_socket(), ERR_NOTREGISTERED(client->nickname));
=======
            sendError(clientSocket, ERR_NOTENOUGHPARAM(client.nickname));
    }
    else
    {
        sendError(clientSocket, ERR_NOTREGISTERED(client.nickname));
>>>>>>> 858be78f1c14a9e55cae2f94e6ebc520112464a2
    }
}