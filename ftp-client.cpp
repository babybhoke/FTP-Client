#include <iostream>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// An FTPClient class
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

    std::string receiveResponse() {
        char buffer[1024];
        std::memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(controlSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            return std::string(buffer, bytesReceived);
        }
        return {};
    }

    void sendCommand(const std::string& command) {
        send(controlSocket, command.c_str(), command.size(), 0);
        send(controlSocket, "\r\n", 2, 0); // End of command
    }

public:
    FTPClient(const std::string& server, int port) : serverAddress(server), serverPort(port) {}

    bool connectAndLogin(const std::string& username, const std::string& password) {
        if (!connectToServer()) return false;

        // Wait for server response
        std::cout << receiveResponse();

        // Send username
        sendCommand("USER " + username);
        std::cout << receiveResponse();

        // Send password
        sendCommand("PASS " + password);
        std::cout << receiveResponse();

        return true;
    }

    void listFiles() {
        sendCommand("PASV");
        std::string response = receiveResponse();
        std::cout << "PASV Response: " << response;

        // Extract IP and port from PASV response (e.g., 227 Entering Passive Mode (192,168,1,2,12,34))
        // Use a regex or parsing logic to connect to the data port for LIST
        // Example: Connect to the derived IP:PORT using a data socket
        // Send LIST command and print received data.
    }

    ~FTPClient() {
        if (controlSocket >= 0) {
            close(controlSocket);
        }
    }
};

//app's main function
int main() {
    FTPClient client("127.0.0.1", 21);

    if (client.connectAndLogin("username", "password")) {
        std::cout << "Login successful!" << std::endl;
        client.listFiles();
    } else {
        std::cerr << "Failed to connect or login!" << std::endl;
    }

    return 0;
}