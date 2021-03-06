#include <stdio.h>
#include <WinSock2.h>
#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
using namespace std;
#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib")  //load  ws2_32.dl
#define BUF_SIZE 100
HANDLE hMutex;
DWORD WINAPI Rec(LPVOID ipParameter)
{
	SOCKET sockClient = (SOCKET)ipParameter;
	char revBuf[BUF_SIZE];
	int byte = 0;
	while (1)
	{
		WaitForSingleObject(hMutex, INFINITE);
		memset(revBuf, 0, BUF_SIZE);
		byte = recv(sockClient, revBuf, BUF_SIZE, 0);//???????????  
		printf("Message form server: %s\n", revBuf);
		if (byte <= 0)
		{
			break;
		}
		memset(revBuf, 0, BUF_SIZE);  //reset the buffer area
		ReleaseMutex(hMutex);
	}
	printf("thread closed");
	closesocket(sockClient);//??socket,??????  
	return 0;
}

int main() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);//initialization dll;
	char IP[20];
	printf("input the server IP: (local host is '127.0.0.1')\n");
	gets_s(IP);
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(IP);// the server ip address is localhost
	sockAddr.sin_port = htons(5000);
	char sendBuf[BUF_SIZE];
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);//create socket
	connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));//connect to the server
														  //users choosing the different works
	
	

	
	printf( "Please Input your username:");//Asking for Inputing your Username
	gets_s(sendBuf);//Getting client username from Client and Sending to Server
	send(sock, sendBuf, strlen(sendBuf), 0);
	memset(sendBuf, 0, BUF_SIZE);
		int rec; char bufRecv1[BUF_SIZE];//creating receiving buffer
		rec = recv(sock, bufRecv1, BUF_SIZE, 0);
	printf( "The usernames who are online now: \n ");
	
	printf(bufRecv1);
	cout << endl;
	printf( "Choose a name you want to chat with: \n ");
	HANDLE WINAPI hThread = CreateThread(NULL, 0, Rec, (LPVOID)sock, 0, NULL);//open a thread for receive message from server
	while (1) {
		printf("Input a string: (input 'quit' to quit)\n ");
		gets_s(sendBuf);
		if (sendBuf[0] == 'q'&&sendBuf[1] == 'u'&&sendBuf[2] == 'i'&&sendBuf[3] == 't') { break; }//user input quit to quit the message system
		send(sock, sendBuf, strlen(sendBuf), 0);//get what the user input and send it to server 
	}
	/*the following two close function should happen when the client want to end the connect with the server*/
	CloseHandle(hThread);//close the receive thread .
	closesocket(sock); //close socket. because hThread uses sock as socket, so if we should close hThread sock first
	WSACleanup();  //stop using dll
	return 0;
}
