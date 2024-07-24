#include "server.hpp"

bool nickAlreadyInUse(std::string nickname, std::map<int, Client>& allClients, int fd)
{
    std::map<int, Client>::iterator it;
    for (it = allClients.begin(); it != allClients.end(); it++)
    {
        if (it->second.nickname == nickname && it->second.get_client_socket() != fd)
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
        }
    }
}

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
                }
            }
            else
            {
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
}