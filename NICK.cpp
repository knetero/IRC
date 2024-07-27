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
    if (nickname[0] == '#' || nickname[0] == ':')
        return (false);
    // if (isdigit(nickname[0]))
    //     return (false);
    // if (nickname.find(" "))
    //     return (false);
    return true;
}

void Server::broadcastToChannels(int fd, std::string nickname, Client &c)
{
    std::cout << c.joinedChannels.size() << std::endl;
    for (size_t i = 0; i < c.joinedChannels.size(); i++)
    {
        std::map<int, Client >::iterator it;
        for (it = c.joinedChannels[i].getmembers()->begin(); it != c.joinedChannels[i].getmembers()->end(); it++)
        {
            sendData(it->second.get_client_socket(),\
            NICK(c.nickname, c.username, getIp(adresses[fd]), nickname));
        }
    }
}

void Server::nickCommand(Client *client, std::vector<std::string> &parameters)
{
    if (parameters.size())
    {
        if (client->passSet)
        {
            if (nickAlreadyInUse(parameters[0], serverClients, client->get_client_socket())) // check if the username is already used.
                sendData(client->get_client_socket(), ERR_NICKINUSE(parameters[0]));
            else if (client->nickname == parameters[0]) // check if it is already my nickname
                return ;
            else if (!isValidNick(parameters[0])) // check the synthax of the nickname
                sendData(client->get_client_socket(), ERR_ERRONEUSNICKNAME(parameters[0]));
            else if (client->isRegistered) // check if already registred
            {
                if (client->nickname != parameters[0]) // client wants to change the nick
                {
                    broadcastToChannels(client->get_client_socket(), parameters[0], *client);
                    client->nickname = parameters[0];
                }
            }
            else
            {
                client->nickname = parameters[0];
                client->nickSet = true;
                if (client->nickSet && client->userSet)
                {
                    client->isRegistered = true;
                    welcomeMessage(client->get_client_socket(), *client);
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
}