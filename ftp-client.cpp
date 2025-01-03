#include <iostream>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

class FTPClient {
private:
    int controlSocket;
    std::string serverAddress;
    int serverPort;

    bool connectToServer() {}
};