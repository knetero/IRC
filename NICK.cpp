#include "server.hpp"

bool nickAlreadyInUse(std::string nickname, std::map<int, Client *>& allClients, int fd)
{
    std::map<int, Client *>::iterator it;
    for (it = allClients.begin(); it != allClients.end(); it++)
    {
        if (it->second->nickname == nickname && it->second->get_client_socket() != fd)
            return (true);
    }
    return (false);
}

bool isValidNick(std::string nickname)
{
    if (nickname.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]{}/|-_") != std::string::npos)
        return (false);
    if (!isalpha(nickname[0]))
        return (false);
    return true;
}

void Server::broadcastToChannels(std::string nickname, Client *c)
{
    for (size_t i = 0; i < c->joinedChannels.size(); i++)
    {
        std::map<int, Client *>::iterator it;
        for (it = c->joinedChannels[i]->getmembers().begin(); it != c->joinedChannels[i]->getmembers().end(); it++)
        {
            sendData(it->second->get_client_socket(),\
            NICK(c->nickname, c->username, getIp(c->clientAdress), nickname));
        }
    }
}

void Server::nickCommand(Client *client, std::vector<std::string> &parameters)
{
    if (parameters.size() == 2)
    {
            if (nickAlreadyInUse(parameters[1], serverClients, client->get_client_socket())) // check if the username is already used.
                sendData(client->get_client_socket(), ERR_NICKINUSE(parameters[1]));
            else if (client->nickname == parameters[1]) // check if it is already my nickname
                return ;
            else if (!isValidNick(parameters[1])) // check the synthax of the nickname
                sendData(client->get_client_socket(), ERR_ERRONEUSNICKNAME(parameters[1]));
            else if (client->isRegistered) // check if already registred
            {
                if (client->nickname != parameters[1]) // client wants to change the nick
                {
                    broadcastToChannels(parameters[1], client);
                    client->nickname = parameters[1];
                }
            }
            else
            {
                client->nickname = parameters[1];
                client->nickSet = true;
                if (client->passSet && client->userSet)
                {
                    client->isRegistered = true;
                    welcomeMessage(client);
                }
            }
        
    }
    else
        sendData(client->get_client_socket(), ERR_NONICKNAMEGIVEN);
}