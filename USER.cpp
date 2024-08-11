#include "server.hpp"

void Server::userCommand(Client *client, std::vector<std::string> &parameters)
{
    if (parameters.size() > 1)
    {
        if (client->passSet)
        {
            if (client->userSet)
            {
                sendData(client->get_client_socket(), ERR_ALREADYREGISTERED(client->nickname));
                return ;
            }
            if (parameters.size() < 5)
            {
                sendData(client->get_client_socket(), ERR_NEEDMOREPARAMS(client->nickname, "USER"));
                return ;
            }
            client->username = parameters[1];
            client->realname = parameters[4];
            client->userSet = true;
            if (client->userSet && client->nickSet)
            {
                client->isRegistered = true;
                welcomeMessage(client->get_client_socket(), client);
            }
        }
        else
        {
            sendData(client->get_client_socket(), ERROR(std::string("access denied: bad password")));
        }
    }
    else
        sendData(client->get_client_socket(), ERR_NEEDMOREPARAMS(client->nickname, "USER"));
}