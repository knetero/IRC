#include "server.hpp"

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
    }
}