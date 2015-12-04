/*********************************************************************************
  *Copyright    Zeyu Ni
  *FileName:  client.cpp
  *Author:  Zeyu Ni
  *Version:  1.0
  *Date:  2015.12.3
  *Description:  // The server program can:
                        1. sent message to server
                        2. sent broadcasting request with messasge to server
                        4. sent file to sevrver and request for resenting to another client
  *Others:  none
**********************************************************************************/
#include <stdio.h>
#include <WinSock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")  //load  ws2_32.dll

#define BUF_SIZE 100

int main(){
    cout << "==========================================================\n";
    cout << "|This program is wirten by Xuan Li, Zeyu Ni, Shuo Zhang  |\n";
    cout << "|This program  using socket lib and tcp/ip portocol      |\n";
    cout << "==========================================================\n";
    //initialization dll;
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    int n = 0;

    //send request to server program
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");// the server ip address is localhost
    sockAddr.sin_port = htons(5000);

    char bufSend[BUF_SIZE] = {0};
    char bufRecv[BUF_SIZE] = {0};
    
    cout << "Please choose which work you want to do:\n";
    cout << "1--Send Message to Server; 2--Sent Broadcast Request; 3--Send Files\n";
    cout << "Please input the number: ";
    cin >> n;
    
    while(1){
        //create socket
        SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
        if(n == 1){
           //get what the user input and send is to server
           printf("Input a string: ");
           gets(bufSend);
           send(sock, bufSend, strlen(bufSend), 0); 
        }
        
        //recieve what the server send back
        recv(sock, bufRecv, BUF_SIZE, 0);
        //print out the buffer
        printf("accepted client IP:[%s],port:[%d]\n",inet_ntoa(sockAddr.sin_addr),ntohs(sockAddr.sin_port));
        printf("Message form server: %s\n", bufRecv);
        
        //Sending data in a loop until reach the end of file
         char buffer[BUF_SIZE] = {0};  //Buffer Area for file
         int nCount;
         while( (nCount = recv(sock, buffer, BUF_SIZE, 0)) > 0 ){
             fwrite(buffer, nCount, 1, fp);
         }
         

        memset(bufRecv, 0, BUF_SIZE);  //reset the buffer area
        fclose(fp);
        closesocket(sock);  //close socket
    }

    WSACleanup();  //stop using dll
    return 0;
}
