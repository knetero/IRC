#include "server.hpp"

void Server::sendData(int fd, std::string data)
{
    send(fd, data.c_str(), data.size(), 0);
}

// void Server::privmsgCommand(Client *client, std::vector<std::string> &parameters)
// {
//     // Client &client = allClients[clientSocket];
//     if (client->isRegistered)
//     {
//         if (parameters.size() == 0)
//         {
//             sendError(client->get_client_socket(), ERR_NOTENOUGHPARAM(client->nickname));
//             return ;
//         }
//         if (parameters.size() == 2)
//         {
//             std::vector<std::string> targets;
//             std::istringstream ss(parameters[0]);
//             std::string target;
//             while (getline(ss, target, ','))
//                 targets.push_back(target);
//             if (parameters[1][0] == ':')
//                 parameters[1] = parameters[1].substr(1);
//             for (int i = 0; i < (int)targets.size(); i++)
//             {
//                 if (targets[i][0] == '#' && server_channels.find(targets[i].substr(1)) != server_channels.end()) // if target is a channel
//                 {
//                     if (server_channels.find(targets[i].substr(1))->second.clientExist(client->nickname) != -1)
//                     {
//                         std::map<std::string, Channel>::iterator channel = server_channels.find(targets[i].substr(1));
//                         std::map<int, Client > *members = channel->second.getmembers();
//                         std::map<int, Client >::iterator it;
//                         for (it = members->begin(); it != members->end(); it++)
//                         {
//                             if (it->second.get_client_socket() != client->get_client_socket())
//                                 sendData(it->second.get_client_socket(), \
//                                 PRIVMSG(client->nickname, client->username, getIp(adresses[client->get_client_socket()]), targets[i], parameters[1]));
//                         }
//                     }
//                     else
//                         sendData(client->get_client_socket(), ERR_CANNOTSENDTOCHAN(client->nickname, targets[i]));
//                 }
//                 else
//                 {
//                     sendData(getClientFd(parameters[0]), \
//                     PRIVMSG(client->nickname, client->username, getIp(adresses[client->get_client_socket()]), targets[i], parameters[1]));
//                 }
//             }
//         }
//         else
//             sendError(client->get_client_socket(), ERR_NOTENOUGHPARAM(client->nickname));
//     }
//     else
//     {
//         sendError(client->get_client_socket(), ERR_NOTREGISTERED(client->nickname));
//     }
// }