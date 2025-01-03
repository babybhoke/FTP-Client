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

    bool connectToServer() {
        struct sockaddr_in serverAddr;

        // Create socket
        controlSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (controlSocket < 0) {
            std::cerr << "Error creating socket!" << std::endl;
            return false;
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(serverPort);
        inet_pton(AF_INET, serverAddress.c_str(), &serverAddr.sin_addr);

        // Connect to server
        if (connect(controlSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Connection to server failed!" << std::endl;
            return false;
        }
        return true;
    }
};