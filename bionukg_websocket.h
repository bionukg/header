#pragma once
//�ο�����
//��� Winsock ���������� ΢��[EB/OL] 2022��11��14��
//https://learn.microsoft.com/zh-cn/windows/win32/winsock/complete-server-code
//��� Winsock �ͻ��˴��� ΢��[EB/OL] 2022��11��14��
//https://learn.microsoft.com/zh-cn/windows/win32/winsock/complete-client-code

#ifndef _bionukg_websocket_h
#define _bionukg_websocket_h
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include<iostream>
// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define print_and_pause(...)  do{printf(__VA_ARGS__);system("pause");}while(0);
//�����ͻ��˺ͷ���˵Ĺ��в���
class websocket
{
public:

    //����127,0,0,1
    void ipv4(uint8_t ipaddr0, uint8_t ipaddr1, uint8_t ipaddr2, uint8_t ipaddr3)
    {
        this->sitestr = sitestr_in;
        uint8_t ipaddr[4] = { ipaddr0,ipaddr1,ipaddr2,ipaddr3};
        hints.ai_family = AF_INET;
        int idx = 0;
        for (int i = 0; i < 4; i++)
        {
            if (ipaddr[i] >= 100)
            {
                sitestr_in[idx] = ipaddr[i] / 100 + '0';
                idx++;
            }
            if (ipaddr[i] >= 10)
            {
                sitestr_in[idx] = ipaddr[i] / 10 % 10 + '0';
                idx++;
            }
            sitestr_in[idx] = ipaddr[i] % 10 + '0';
            idx++;
            sitestr_in[idx] = '.';
            idx++;
        }
        sitestr_in[idx - 1] = '\0';
        return;
    }
    
    /// <summary>
    /// ��������ʽ���ʣ�Ӧ�����Զ�DNS�˰�
    /// </summary>
    /// <param name="url">����������learn.microsoft.com</param>
    void bysite(const char* url)
    {
        this->sitestr = url;
    }
    //�˿ں�,��������0~65535
    void port(uint16_t port)
    {
        int idx = 0;
        if (port >= 10000)
        {
            portstr_in[idx] = port / 10000;
            idx++;
        }
        if (port >= 1000)
        {
            portstr_in[idx] = port / 1000 % 10;
            idx++;
        }
        if (port >= 100)
        {
            portstr_in[idx] = port / 100 % 10;
            idx++;
        }
        if (port >= 10)
        {
            portstr_in[idx] = port / 10 % 10;
            idx++;
        }
        portstr_in[idx] = port % 10;
        idx++;
        portstr_in[idx] = '\0';
        return;
    }
    //����Э�������Զ��Ҷ˿ڣ�����http,ftp�ȵ�
    void port(const char* protocol_str)
    {
        this->portstr = protocol_str;
    }

    //������ַ
    void resolve()
    {
        iResult = getaddrinfo(sitestr, portstr, &hints, &result);
        if (iResult != 0) {
            print_and_pause("getaddrinfo failed with error: %d\n", iResult);
        }
        return;
    }
    void no_read()
    {
        socket_check(ClientSocket);       
        iResult = shutdown(this->ClientSocket, SD_RECEIVE);
        if (iResult == SOCKET_ERROR) {
            print_and_pause("shutdown failed with error: %d\n", WSAGetLastError());
        }
        readable = false;
    }
    void no_write()
    {
        socket_check(ClientSocket);
        iResult = shutdown(this->ClientSocket, SD_SEND);
        if (iResult == SOCKET_ERROR) {
            print_and_pause("shutdown failed with error: %d\n", WSAGetLastError());
        }
        writeable = false;
    }
    int read(void* buffer, int len)
    {
        socket_check(ClientSocket);
        if (!readable)
            print_and_pause("warning: read after no_read");
        return recv(ClientSocket, (char*)buffer, len, 0);
    };
    //����ֱ���������������������ж�
    int read_full(void* buffer, int len)
    {
        socket_check(ClientSocket);
        if (!readable)
            print_and_pause("warning: read after no_read");
        return recv(ClientSocket, (char*)buffer, len, MSG_WAITALL);
    };
    int write(const void* buffer, int len)
    {
        socket_check(ClientSocket);
        if (!readable)
            print_and_pause("warning: read after no_read");

        return send(this->ClientSocket, (char*)buffer, len, 0);
    }

    void socket_check(SOCKET s)
    {
        if (s == INVALID_SOCKET)     
            print_and_pause("error:INVALID_SOCKET\n");
        
    }
    websocket()
    {
        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) {
            print_and_pause("WSAStartup failed with error: %d\n", iResult);
        }
        return;
    };
    ~websocket()
    {

        if (ClientSocket != INVALID_SOCKET)
        {
            no_read();
            no_write();
            closesocket(ClientSocket);
        }

        WSACleanup();
        return;
    };
    /// <summary>
    /// Ĭ��ָ���ڲ��ַ�������ֵ�������ú��������ⲿ��Ҫ�޸ġ�
    /// �������ⲿ�޸ĺã�Ȼ������ָ���ض����ⲿ�ַ���
    /// </summary>
    const char* sitestr = sitestr_in,* portstr = portstr_in;
protected:
    WSADATA wsaData = { 0 };
    int iResult = 0;
    char sitestr_in[64] = { 0 };
    char portstr_in[16] = { 0 };
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL;
    struct addrinfo hints = { 0 };

    int iSendResult = 0;
    char recvbuf[DEFAULT_BUFLEN] = { 0 };
    int recvbuflen = DEFAULT_BUFLEN;
    bool readable = true, writeable = true;
};

//�����
class webserver :public websocket
{
public:
    void TCP()
    {
        hints.ai_flags = AI_PASSIVE;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
    }
    void UDP()
    {
        hints.ai_flags = AI_PASSIVE;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_protocol = IPPROTO_UDP;
    }
    //�󶨶˿�
    void sockct_bind()
    {
        if (result == NULL)
        {
            printf("warning: didn't reslove. auto reslove:\n");
            this->resolve();
            if (result == NULL)
            {
                print_and_pause("error: reslove failed.\n");
                return;
            }
        }
        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (ListenSocket == INVALID_SOCKET) {
            print_and_pause("socket failed with error: %ld\n", WSAGetLastError());
            goto socbinend;
        }

        // Setup the TCP listening socket
        iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            print_and_pause("bind failed with error: %d\n", WSAGetLastError());
        }
    socbinend:
        freeaddrinfo(result);
        return;
    }
    //�˲�������������ǽ��Ҳ��������ֱ���ͻ�������
    void wait_client()
    {
        if (ListenSocket == INVALID_SOCKET)
        {
            printf("warning: didn't bind. auto bind:\n");
            this->sockct_bind();
            if (ListenSocket == INVALID_SOCKET)
            {
                print_and_pause("error: bind failed.\n");
                return;
            }


        }
        printf("waiting...\n");
        iResult = listen(ListenSocket, SOMAXCONN);
        if (iResult == SOCKET_ERROR) {
            print_and_pause("listen failed with error: %d\n", WSAGetLastError());
            goto wtclend;
        }

        // Accept a client socket
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            print_and_pause("accept failed with error: %d\n", WSAGetLastError());
        }
    wtclend:
        // No longer need server socket
        closesocket(ListenSocket);

    }
    
    ~webserver()
    {
        if (ListenSocket != INVALID_SOCKET)
            closesocket(ListenSocket);
    }

private:
    SOCKET ListenSocket = INVALID_SOCKET;

};

//�ͻ���
class webclient :public websocket
{
public:
    void TCP()
    {
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
    }
    void UDP()
    {
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_protocol = IPPROTO_UDP;
    }
    //�˲�������������ǽ
    void socket_connect()
    {
        if (result == NULL)
        {
            printf("warning: didn't reslove. auto reslove:\n");
            this->resolve();
        }
        // Attempt to connect to an address until one succeeds
        for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

            // Create a SOCKET for connecting to server
            ClientSocket = socket(ptr->ai_family, ptr->ai_socktype,
                ptr->ai_protocol);
            if (ClientSocket == INVALID_SOCKET) {
                print_and_pause("socket failed with error: %ld\n", WSAGetLastError());
                return;
            }

            // Connect to server.
            iResult = connect(ClientSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (iResult == SOCKET_ERROR) {
                closesocket(ClientSocket);
                ClientSocket = INVALID_SOCKET;
                continue;
            }
            return;
        }
        return;
    }
private:
    struct addrinfo* ptr = NULL;

};








#endif // !_bionukg_websocket_h




