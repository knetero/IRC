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

class Client {
    public:
        std::map<int , Client> clients_Map;
        int clientSocket;
        std::map<int, std::string> clientBuffers;
        std::string nickname;
        std::string username;
        std::string realname;

        Client();
        Client(int clientSocket);
        Client(const Client& other);
        Client& operator=(const Client& other);
        ~Client();


        bool passSet;
        bool nickSet;
        bool userSet;
        bool isRegistered;
        int welcome_msg;

        int get_client_socket();
};

#endif
