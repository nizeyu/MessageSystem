#include "Client.h"


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
		byte = recv(sockClient, revBuf, BUF_SIZE , 0);//receive the data from Server  
		printf("Message form server: %s\n", revBuf);
		if (byte <= 0)
		{
			break;
		}
		memset(revBuf, 0, BUF_SIZE);  //reset the buffer area
		ReleaseMutex(hMutex);
	}
	printf("thread closed");
	closesocket(sockClient);//close socket for Client  
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
	HANDLE WINAPI hThread = CreateThread(NULL, 0, Rec, (LPVOID)sock, 0, NULL);//open a thread for receive message from server
	/*
	1.ask to input username and send to server.
	2.receive a clientnumber from server
	3.inpunt 1/2/3 represent send message or ask for broad cast or file transfer
	while(2)
	while(3)
	*/
	while (1) {
		printf("Input a string: (input 'quit' to quit)\n ");
		gets_s(sendBuf);
		if (sendBuf[0] == 'q'&&sendBuf[1] == 'u'&&sendBuf[2] == 'i'&&sendBuf[3] == 't') {  break; }//user input quit to quit the message system
		send(sock, sendBuf, strlen(sendBuf), 0);//get what the user input and send it to server 
	}
	/*the following two close function should happen when the client want to end the connect with the server*/
	CloseHandle(hThread);//close the receive thread .
	closesocket(sock); //close socket. because hThread uses sock as socket, so if we should close hThread sock first
	WSACleanup();  //stop using dll
	return 0;
}
