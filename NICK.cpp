#include "server.hpp"

<<<<<<< HEAD
bool nickAlreadyInUse(std::string nickname, std::map<int, Client *>& allClients, int fd)
{
    std::map<int, Client *>::iterator it;
    for (it = allClients.begin(); it != allClients.end(); it++)
    {
        if (it->second->nickname == nickname && it->second->get_client_socket() != fd)
=======
bool nickAlreadyInUse(std::string nickname, std::map<int, Client>& allClients, int fd)
{
    std::map<int, Client>::iterator it;
    for (it = allClients.begin(); it != allClients.end(); it++)
    {
        if (it->second.nickname == nickname && it->second.get_client_socket() != fd)
>>>>>>> 858be78f1c14a9e55cae2f94e6ebc520112464a2
            return (true);
    }
    return (false);
}

bool isValidNick(std::string nickname)
{
<<<<<<< HEAD
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
        for (it = c->joinedChannels[i].getmembers().begin(); it != c->joinedChannels[i].getmembers().end(); it++)
        {
            sendData(it->second->get_client_socket(),\
            NICK(c->nickname, c->username, getIp(c->clientAdress), nickname));
=======
    if (nickname[0] == '#' || nickname[0] == ':')
        return (false);
    // if (isdigit(nickname[0]))
    //     return (false);
    // if (nickname.find(" "))
    //     return (false);
    return true;
}

void Server::broadcastToChannels(int fd, std::vector<Channel *> joinedChannels)
{
    for (int i = 0; i < (int)joinedChannels.size(); i++)
    {
        std::map<int, Client *>::iterator it;
        for (it = joinedChannels[i]->getmembers()->begin(); it != joinedChannels[i]->getmembers()->end(); it++)
        {
            if (it->first != fd)
            {
                std::string s = "test";
                sendData(it->first, NICK(s, "test", "test", "test"));
            }
>>>>>>> 858be78f1c14a9e55cae2f94e6ebc520112464a2
        }
    }
}

<<<<<<< HEAD
void Server::nickCommand(Client *client, std::vector<std::string> &parameters)
{
    if (parameters.size() == 2)
    {
        if (client->passSet)
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
=======
void Server::nickCommand(int fd, std::map<int, Client>& allClients, std::string param)
{
    if (!param.empty())
    {
        if (allClients[fd].passSet)
        {
            if (nickAlreadyInUse(param, allClients, fd)) // check if the username is already used.
                sendData(fd, ERR_NICKINUSE(param));
            else if (allClients[fd].nickname == param) // check if it is already my nickname
                return ;
            else if (!isValidNick(param)) // check the synthax of the nickname
                sendData(fd, ERR_ERRONEUSNICKNAME(param));
            else if (allClients[fd].isRegistered) // check if already registred
            {
                if (allClients[fd].nickname != param) // client wants to change the nick
                {
                    broadcastToChannels(fd, allClients[fd].joinedChannels);
>>>>>>> 858be78f1c14a9e55cae2f94e6ebc520112464a2
                }
            }
            else
            {
<<<<<<< HEAD
                client->nickname = parameters[1];
                client->nickSet = true;
                if (client->nickSet && client->userSet)
                {
                    client->isRegistered = true;
                    welcomeMessage(client->get_client_socket(), client);
                }
            }
        }
        else
        {
            sendData(client->get_client_socket(), ERROR(std::string("access denied: bad password")));
        }
    }
    else
        sendData(client->get_client_socket(), ERR_NONICKNAMEGIVEN);
=======
                allClients[fd].nickname = param;
                if (allClients[fd].passSet /*&& allClients[fd].userSet*/)
                {
                    allClients[fd].isRegistered = true;
                    //welcome
                }
            }

        }
        else
        {
            sendData(fd, ERROR(std::string("access denied: bad password")));
        }
    }
    else
        sendData(fd, ERR_NONICKNAMEGIVEN);
>>>>>>> 858be78f1c14a9e55cae2f94e6ebc520112464a2
}