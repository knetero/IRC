#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <algorithm>
#include <cstdio>
#include <errno.h>  
#include <arpa/inet.h>
#include <netdb.h>
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

<<<<<<< HEAD
#define RESET "\033[0m"
#define RED "\033[31m"                /* Red */
#define GREEN "\033[32m"              /* Green */
#define YELLOW "\033[33m"             /* Yellow */
#define BLUE "\033[34m"               /* Blue */
#define MAGENTA "\033[35m"            /* Magenta */
#define CYAN "\033[36m"               /* Cyan */
=======
>>>>>>> 858be78f1c14a9e55cae2f94e6ebc520112464a2

#define BUFFER 1024
class Client;
class Channel;
class Server {
    public:
        Server(int port, const std::string& password);
<<<<<<< HEAD
        struct sockaddr_in serverAddress;
        struct sockaddr_in clientAdress;
        std::map<int , Client *>            serverClients;
        std::map<std::string, Channel *>    server_channels;
        char    *startdate;
=======
        std::map<std::string, Channel> *server_channels;
>>>>>>> 858be78f1c14a9e55cae2f94e6ebc520112464a2
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
        int acceptClient();
        static void signalHandler(int signum);
<<<<<<< HEAD
        void send_private_message(int clientSocket, const std::string& data, std::map<int, Client>& clients_Map);
        void parse_commands(Client *client, std::string& data);
        void notifyClients(int clientSocket, std::map<int, Client>& clients_Map, std::string oldNick);
        std::string toUpperCase(std::string& str);
        void check_Quit(int clientSocket, const std::string& data, std::map<int, Client>& clients_Map);
        void check_user(int clientSocket, const std::string&data , std::map<int, Client>& clients_Map, Client& client);
        bool check_Nick(int clientSocket, std::string value,  std::map<int, Client>& clients_Map);
    
        void                        welcomeMessage(int fd, Client *client);
        int                         get_nick(std::string chName, std::string nickname);
        int                         check_properties(Channel *channel, std::string mdp, Client *client);
        void                        sendData(int fd, std::string data);
        void                        broadcastToChannels(std::string nickname, Client *c);
        std::string                 getIp(struct sockaddr_in addr);
        std::string                 getServerIp(void);
        int                         getClientFd(std::string target);
        void                        broadcastToChannel(Channel &channel, Client *kicker, std::string target, std::string comment);
        void                        getDate(char **buf);
        std::vector<std::string>    splitParameters(std::string parametersString);
        void send_info(Channel &chName, std::string msg);
        int channelExist(std::string channelName);
        void broadcastTopic(Channel *channel, Client *client, std::string topic);
        void send_info(Channel *chName, std::string msg);



        // Commands
        void                        passCommand(Client *client, std::vector<std::string> &parameters);
        void                        nickCommand(Client *client, std::vector<std::string> &parameters);
        void                        userCommand(Client *client, std::vector<std::string> &parameters);
        void                        privmsgCommand(Client *client, std::vector<std::string> &parameters);
        void                        kickCommand(Client *client, std::vector <std::string> &parameters);
        void                        inviteCommand(Client *client, std::vector<std::string> &parameters);
        void                        topicCommand(Client *client, std::vector<std::string> &parameters);
        void                        join(std::string value, Client *client);
        void                        mode(std::string value, Client *client);
        //
=======
        bool isCommand(const std::string& data);
        void send_private_message(int clientSocket, const std::string& data, std::map<int, Client >& clients_Map);
        void parse_commands(int clientSocket, const std::string& data, std::map<int, Client >& clients_Map);
        bool sendError(int clientSocket, const std::string& errorMessage);
        void notifyClients(int clientSocket, std::map<int, Client >& clients_Map, std::string oldNick);
        std::string toUpperCase(std::string& str);
        std::vector<std::string> split(const std::string &s, char delim);
        void check_Quit(int clientSocket, const std::string& data, std::map<int, Client >& clients_Map);
        void sendWelcomeMessages(int clientSocket, std::map<int, Client >& clients_Map);
        void check_user(int clientSocket, const std::string&data , std::map<int, Client >& clients_Map, Client& client);
        bool check_Nick(int clientSocket, std::string value,  std::map<int, Client >& clients_Map);
    
        void join(std::string value, int clientsocket, std::map<int, Client >& clients_Map);
        void mod(std::string value, int clientsocket, std::map<int, Client >& clients_Map);
        int get_nick(std::string chName, std::string nickname);
        int check_properties(Channel channel, std::string mdp, int clientsocket, std::map<int, Client >& clients_Map);
        void set_mode(std::vector<std::string> & args, char mode, std::map<int, Client >&  clients_Map, std::string ss);

>>>>>>> 858be78f1c14a9e55cae2f94e6ebc520112464a2
};

#endif
