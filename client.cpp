/*********************************************************************************
  *Copyright    Zeyu Ni, Xuan Li, Shuo Zhang
  *FileName:  client.cpp
  *Author:  Zeyu Ni, Xuan Li, Shuo Zhang
  *Version:  1.0
  *Date:  2015.12.2
  *Description:  // The server program can:
                        1. sent message to server
                        2. sent broadcasting request with messasge to server
                        4. sent file to sevrver and request for resenting to another client
  *Others:  none
**********************************************************************************/
#include <stdio.h>
#include <WinSock2.h>
#include <windows.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")  //loading  ws2_32.dll

#define BUF_SIZE 100
using namespace std;

HANDLE hMutex;
DWORD WINAPI Rec(LPVOID ipParameter)
{
	SOCKET sockClient = (SOCKET)ipParameter;
	char bufRecv[BUF_SIZE] = {0};//creating receiving buffer

	int byte = 0;
	while (1)
	{
		WaitForSingleObject(hMutex, INFINITE);
		memset(bufRecv, 0, BUF_SIZE);
		byte = recv(sockClient, bufRecv, BUF_SIZE , 0);//receiving the data from Server

		cout << "Message form server:" << bufRecv << '\n';
		if (byte <= 0)
		{
			break;
		}
		memset(bufRecv, 0, BUF_SIZE);  //resetting the buffer area
		ReleaseMutex(hMutex);
	}
	printf("thread closed");
	closesocket(sockClient);//closing socket for Client
	return 0;


}

int main(){
    cout << "==========================================================\n";
    cout << "|This program is wirten by Zeyu Ni, Xuan Li, Shuo Zhang    |\n";
    cout << "|This program  using socket lib and tcp/ip portocol       |\n";
    cout << "==========================================================\n";
    //initializing DLL;
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //Originating the request to server
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));//filling every byte with 0
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");// the server's IP address is local host
    sockAddr.sin_port = htons(5000);//setting port number

    char bufSend[BUF_SIZE] = {0};//creating sending buffer

    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);//creating socket
    connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));//connecting
    HANDLE WINAPI hThread = CreateThread(NULL, 0, Rec, (LPVOID)sock, 0, NULL);//open a thread for receiving message from server

    while(1){

        cout << "Input a string(input 'quit' to quit): \n ";
        gets(bufSend);//getting data from client and sending to server
        if (sendBuf[0] == 'q'&&sendBuf[1] == 'u'&&sendBuf[2] == 'i'&&sendBuf[3] == 't') {  break; }//user input quit to quit the message system
        send(sock, bufSend, strlen(bufSend), 0);

        cout << "accepted client IP:" << inet_ntoa(sockAddr.sin_addr) << '\t';
        cout << "port:" << ntohs(sockAddr.sin_port)  << '\n';
        closesocket(sock);  //closing socket
   }
   CloseHandle(hThread);//close the receive thread.
   closesocket(sock); //close socket. because hThread uses sock as socket, so if we should close hThread sock first
   WSACleanup();  //stopping using DLL
   return 0;
        /*//Sending data in a loop until reach the end of file
         char buffer[BUF_SIZE] = {0};  //Buffer Area for file
         int nCount;
         while( (nCount = recv(sock, buffer, BUF_SIZE, 0)) > 0 ){
             fwrite(buffer, nCount, 1, fp);
         }
         fclose(fp);
         */


        /*int n;
    cout << "Please choose which work you want to do:\n";
    cout << "1--Send Message to Server; 2--Sent Broadcast Request; 3--Send Files\n";
    cout << "Please input the number: ";
    cin >> n;
    cout << endl;*/

    //printing the buffer
        //printf("accepted client IP:[%s],port:[%d]\n",inet_ntoa(sockAddr.sin_addr),ntohs(sockAddr.sin_port));
        //printf("Message form server: %s\n", bufRecv);

        //memset(bufRecv, 0, BUF_SIZE);  //reseting the buffer area

}
