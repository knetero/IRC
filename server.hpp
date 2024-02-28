#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <sys/time.h>
#include <map>
#include "client.hpp"
#include <poll.h>
#include <string>
#include "client.hpp"
#include <fcntl.h>

#define BUFFER 512

class Server {
    public:
        Server(int port, const std::string& password);
        // Server(const Server& other);
        // Server& operator=(const Server& other);
        ~Server();

        std::vector<struct pollfd> clientSockets;
        int serverSocket; 
        int socketId;
        int port; 
        std::string password;
        bool isRunning;
        bool isServerCommand;
        char buffer[BUFFER];

        bool isSetNick;
        bool isSetPass;
        bool isSetUser;

        static bool signal;

        bool initialize();
        int acceptClient();
        static void signalHandler(int signum);
        void enter_pass(int clientSocket, const std::string& data);
        bool isCommand(const std::string& data);
        void send_private_message(int clientSocket, const std::string& data, std::map<int, Client>& clients_Map);
        void parse_commands(int clientSocket, const std::string& data, std::map<int, Client>& clients_Map);
        void sendError(int clientSocket, const std::string& nick, const std::string& command, int errorCode, const std::string& errorMessage);
};

#endif
