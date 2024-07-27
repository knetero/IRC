#include "server.hpp"

void Server::passCommand(Client *client, std::vector<std::string> &parameters)
{
    if (parameters.size() == 0)
        sendData(client->get_client_socket(), ERR_NOTENOUGHPARAM(client->nickname));
    else if (client->isRegistered)
        sendData(client->get_client_socket(), ERR_ALREADYREGISTERED(client->nickname));
    else
    {
        if (parameters[0] == this->password)
        {
            client->passSet = true;
        }
        else
            sendData(client->get_client_socket(), ERR_PASSWDMISMATCH(client->nickname));
    }
}