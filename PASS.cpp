#include "server.hpp"

<<<<<<< HEAD
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
=======
void Server::passCommand(int fd, Client &client, std::string passwd)
{
    if (passwd.empty())
        sendData(fd, ERR_NOTENOUGHPARAM(client.nickname));
    else if (this->isSetPass)
        sendData(fd, ERR_NOTREGISTERED(client.nickname));
    else
    {
        if (passwd == this->password)
        {
            client.passSet = true;
        }
        else
            sendData(fd, ERR_PASSWDMISMATCH(client.nickname));
>>>>>>> 858be78f1c14a9e55cae2f94e6ebc520112464a2
    }
}