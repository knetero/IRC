#include "server.hpp"

void Server::passCommand(Client *client, std::vector<std::string> &parameters)
{
    if (parameters.size() < 2)
        sendData(client->get_client_socket(), ERR_NOTENOUGHPARAM(client->nickname));
    else if (client->isRegistered)
        sendData(client->get_client_socket(), ERR_ALREADYREGISTERED(client->nickname));
    else
    {
        if (parameters[1] == this->password)
        {
            client->passSet = true;
        }
        else
            sendData(client->get_client_socket(), ERR_PASSWDMISMATCH(client->nickname));
    }
}