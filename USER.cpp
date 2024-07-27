#include "server.hpp"

std::string getRealName(std::vector<std::string> params)
{
    std::string realName;
    for (size_t i = 3; i < params.size(); i++)
    {
        realName += params.at(i);
        if (i + 1 == params.size())
            break ;
        realName += " ";
    }
    realName = realName.substr(1);
    return realName;
}

void Server::userCommand(Client *client, std::vector<std::string> &parameters)
{
    if (parameters.size())
    {
        if (client->passSet)
        {
            if (client->userSet)
            {
                sendData(client->get_client_socket(), ERR_ALREADYREGISTERED(client->nickname));
                return ;
            }
            if (parameters.size() < 4)
            {
                sendData(client->get_client_socket(), ERR_NOTENOUGHPARAM(client->nickname));
                return ;
            }
            client->username = parameters[0];
            if (parameters[3][0] != ':')
                client->realname = parameters[3];
            else
                client->realname = getRealName(parameters);
            client->userSet = true;
            if (client->userSet && client->nickSet)
            {
                client->isRegistered = true;
                welcomeMessage(client->get_client_socket(), *client);
            }
        }
        else
        {
            sendData(client->get_client_socket(), ERROR(std::string("access denied: bad password")));
        }
    }
    else
        sendData(client->get_client_socket(), ERR_NOTENOUGHPARAM(client->nickname));
}