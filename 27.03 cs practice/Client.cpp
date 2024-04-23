#include <winsock2.h>
#include <iostream>
using namespace std;

#pragma comment(lib, "ws2_32.lib") // Winsock library
#pragma warning(disable:4996)

int main() {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        cout << "WSAStartup failed with error: " << iResult << endl;
        return 1;
    }

    // Create a socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        cout << "Failed to create socket: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    // Connect to server
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(8888);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "Connect failed with error: " << WSAGetLastError() << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    cout << "Connected to server.\n";

    // Input a number
    int number;
    cout << "Enter a number: ";
    cin >> number;

    // Send number to server
    int bytesSent = send(clientSocket, (const char*)&number, sizeof(number), 0);
    if (bytesSent == SOCKET_ERROR) {
        cout << "Send failed with error: " << WSAGetLastError() << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Receive response from server
    int responseNumber;
    int bytesReceived = recv(clientSocket, (char*)&responseNumber, sizeof(responseNumber), 0);
    if (bytesReceived == SOCKET_ERROR) {
        cout << "Receive failed with error: " << WSAGetLastError() << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    cout << "Received response from server: " << responseNumber << endl;

    // Close socket and cleanup
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
