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
#include <sstream>
#include "replies.hpp"
#include "channel.hpp"


#define BUFFER 1024
class Client;
class Channel;
class Server {
    public:
        Server(int port, const std::string& password);
        std::map<std::string, Channel> server_channels;
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
        bool isRegistred;
        static bool signal;


        bool initialize();
        bool sendMessage(int fd, const std::string& message);
        int acceptClient();
        static void signalHandler(int signum);
        bool isCommand(const std::string& data);
        void send_private_message(int clientSocket, const std::string& data, std::map<int, Client>& clients_Map);
        void parse_commands(int clientSocket, const std::string& data, std::map<int, Client>& clients_Map);
        bool sendError(int clientSocket, const std::string& errorMessage);
        void notifyClients(int clientSocket, std::map<int, Client>& clients_Map, std::string oldNick);
        std::string toUpperCase(std::string& str);
        std::vector<std::string> split(const std::string &s, char delim);
        void check_Quit(int clientSocket, const std::string& data, std::map<int, Client>& clients_Map);
        void sendWelcomeMessages(int clientSocket, std::map<int, Client>& clients_Map);
        void check_user(int clientSocket, const std::string&data , std::map<int, Client>& clients_Map, Client& client);
        bool check_Nick(int clientSocket, std::string value,  std::map<int, Client>& clients_Map);
    
        void join(std::string value, int clientsocket, std::map<int, Client>& clients_Map);
        void mod(std::string value, int clientsocket, std::map<int, Client>& clients_Map);
        int get_nick(std::string chName, std::string nickname);

        int check_properties(Channel channel, std::string mdp, int clientsocket, std::map<int, Client>& clients_Map);
        //
        void sendData(int fd, std::string data);
        void passCommand(int fd, Client &client, std::string passwd);
        void nickCommand(int fd, std::map<int, Client>& allClients, std::string param);
        void privmsgCommand(int clientSocket, std::map<int, Client>& allClients, std::string params);
        void broadcastToChannels(int fd, std::vector<Channel *> joinedChannels);

};

#endif
