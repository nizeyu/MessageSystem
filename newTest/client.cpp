#include <stdio.h>
#include <WinSock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")  //load  ws2_32.dll

#define BUF_SIZE 100

int main(){
        printf("==========================================================\n");

    printf("|This program is wirten by Xuan Li, Zeyu Ni, Shuo Zhang  |\n");
    printf("|This program  using socket lib and tcp/ip portocol      |\n");
    printf("==========================================================\n");
    //initialization dll;
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //send request to server program
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");// the server ip address is localhost
    sockAddr.sin_port = htons(5000);

    char bufSend[BUF_SIZE] = {0};
    char bufRecv[BUF_SIZE] = {0};

    while(1){
        //create socket
        SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
        //get what the user input and send is to server
        printf("Input a string: ");
        gets(bufSend);
        send(sock, bufSend, strlen(bufSend), 0);
        //recieve what the server send back
        recv(sock, bufRecv, BUF_SIZE, 0);
        //print out the buffer
        printf("accepted client IP:[%s],port:[%d]\n",inet_ntoa(sockAddr.sin_addr),ntohs(sockAddr.sin_port));
        printf("Message form server: %s\n", bufRecv);

        memset(bufRecv, 0, BUF_SIZE);  //reset the buffer area
        closesocket(sock);  //close socket
    }

    WSACleanup();  //stop using dll
    return 0;
}
