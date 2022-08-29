#include <iostream>
#include <winsock2.h>
using namespace std;

#pragma comment(lib,"ws2_32.lib")

#define BUFLEN 512
#define PORT 8888

int main()
{
    sockaddr_in server, client;

    WSADATA wsa;
    printf("Initialising Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed with error code: %d", WSAGetLastError());
        exit(1);
    }
    printf("Winsock Initialised.\n");

    SOCKET server_socket;
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
    {
        printf("Could not create socket: %d", WSAGetLastError());
    }
    printf("socket created OK.\n");

    u_short requested_port = 0;
    printf("Please enter port number: ");
    std::cin >> requested_port;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(requested_port);

    if (bind(server_socket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code: %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Ready to receive messages\n");
    while (1)
    {
        printf("Listening for messages on port %u", requested_port);
        char message[BUFLEN] = {};
        int message_len;
        int slen = sizeof(sockaddr_in);

        if (message_len = recvfrom(server_socket, message, BUFLEN, 0, (sockaddr*)&client, &slen) == SOCKET_ERROR)
        {
            printf("recvfrom() failed with error code: %d", WSAGetLastError());
            exit(0);
        }

        printf("Received packet from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        printf("Data: %s\n", message);
    }

    closesocket(server_socket);
    WSACleanup();
}