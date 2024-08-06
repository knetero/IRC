#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <unistd.h> 
#include <vector>
#include <map>
#include <cstring> 
#include <iostream>

class Server;
class Channel;
class Client {
    public:
        int                 clientSocket;
        struct sockaddr_in  clientAdress;
        std::string         nickname;
        std::string         username;
        std::string         realname;
        std::string         buffer;
        bool                passSet;
        bool                nickSet;
        bool                userSet;
        bool                isRegistered;
        std::vector<Channel> joinedChannels;

        Client();
        Client(int clientSocket);
        Client(const Client& other);
        Client& operator=(const Client& other);
        ~Client();



        int get_client_socket();
};

#endif
