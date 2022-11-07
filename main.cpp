#define WIN32_LEAN_AND_MEAN
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <iostream>
#include <ws2tcpip.h>
#include <string>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

#define PORT 8888


int main()
{
    string ipAddress = "127.0.0.1"; //IP address of server
    int port = PORT;

    // Initialize Winsock
    WSAData data;
    WORD version = MAKEWORD(2, 2);
    int wsResults = WSAStartup(version, &data);

    if (wsResults != 0) { // WSA startup unsuccessful
        cerr << "Winsock not started. Error #" << wsResults << endl;
        return -1;
    }

    // Create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        cerr << "Socket creation unsuccessful. Error #" << WSAGetLastError() << endl;
        WSACleanup();
        return -1;
    }

    // Fill in to hint structure
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(PORT);
    hint.sin_addr.S_un.S_addr = inet_addr(ipAddress.c_str());

    // Connect to a server
    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR) {
        cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }
    // Do-while loop to send and receive data
    char buf[4096];
    string userInput;

    do {
        cout << "Enter your file path" << endl;
        getline(cin, userInput);

        // Send text
        if (userInput.size() > 0) {
            int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
            if (sendResult != SOCKET_ERROR) {
                ZeroMemory(buf, 4096);
                cout << "Before server" << endl;
                int bytesReceived = recv(sock, buf, 4096, 0);
                if (bytesReceived > 0) {
                    cout << "SERVER " << string(buf, 0, bytesReceived) << endl;
                }
            }
        }

    } while (userInput.size() > 0);
    // Close down program
    closesocket(sock);
    WSACleanup();
    std::cout<<"Hello, World!"<<std::endl;
    return 0;
}
