#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <iostream>
#include <windows.h>
#include <string>
#pragma warning(disable:4996)
#pragma comment (lib, "ws2_32.lib")  //load ws2_32.dll
using namespace std;
class Client
{
private:
    SOCKET clientSOCKET;
    int id;
    string username;

public:
    SOCKET getClientSOCKET(){
        return clientSOCKET;
    }
    void setClientSOCKET(SOCKET s){
        clientSOCKET = s;
    }
   int  getId(){
       return id;
    }
    void setId(int ids){
        id = ids;

    }
    string getUsername(){
        return username;
    }
    void setUserName(string name)
    {
        username = name;
    }
    Client(SOCKET s,string name,int ids) {
        clientSOCKET = s;
        username = name;
        id = ids;
    }
       Client() {

    }
         ~Client() {
         }

};

#endif // CLIENT_H
