#pragma once
// �ο�����
// ��� Winsock ���������� ΢��[EB/OL] 2022��11��14��
// https://learn.microsoft.com/zh-cn/windows/win32/winsock/complete-server-code
// ��� Winsock �ͻ��˴��� ΢��[EB/OL] 2022��11��14��
// https://learn.microsoft.com/zh-cn/windows/win32/winsock/complete-client-code

#ifndef _bionukg_websocket_h
#define _bionukg_websocket_h
#define WIN32_LEAN_AND_MEAN
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <mutex>
// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define print_and_pause(...)  \
    do                        \
    {                         \
        printf(__VA_ARGS__);  \
        /*system("pause"); */ \
    } while (0);
// �����ͻ��˺ͷ���˵Ĺ��в���
class websocket
{
public:
    // ����127,0,0,1
    void ipv4(uint8_t ipaddr0, uint8_t ipaddr1, uint8_t ipaddr2, uint8_t ipaddr3)
    {
        this->sitestr = sitestr_in;
        uint8_t ipaddr[4] = {ipaddr0, ipaddr1, ipaddr2, ipaddr3};
        hints.ai_family = AF_INET;
        int idx = 0;
        for (int i = 0; i < 4; i++)
        {
            idx += sprintf(sitestr_in + idx, "%d", ipaddr[i]);
            if (i != 3)
                sitestr_in[idx] = '.';
            idx++;
        }
        sitestr_in[idx] = '\0';
        return;
    }

    /// <summary>
    /// ��������ʽ���ʣ�Ӧ�����Զ�DNS�˰�
    /// </summary>
    /// <param name="url">����������learn.microsoft.com</param>
    void bysite(const char *url)
    {
        for (int i = 0; i < 64; i++)
        {
            sitestr_in[i] = url[i];
            if (url[i] == '\0')
            {
                sitestr_in[i] = '\0';
                break;
            }
        }
    }
    // �˿ں�,��������0~65535
    void port(uint16_t port)
    {
        sprintf(portstr_in, "%d", port);
        return;
    }
    // ����Э�������Զ��Ҷ˿ڣ�����http,ftp�ȵ�
    void port(const char *protocol_str)
    {
        struct servent *serv = getservbyname(protocol_str, NULL);
        if (serv == NULL)
        {
            print_and_pause("error: getservbyname failed.\n");
            return;
        }
        sprintf(portstr_in, "%d", ntohs(serv->s_port));
        return;
    }

    // ������ַ
    void resolve()
    {
        iResult = getaddrinfo(sitestr, portstr, &hints, &result);
        if (iResult != 0)
        {
            print_and_pause("getaddrinfo failed with error: %d\n", iResult);
        }
        return;
    }
    void no_read()
    {
        socket_check(ClientSocket);
        iResult = shutdown(this->ClientSocket, SD_RECEIVE);
        if (iResult == SOCKET_ERROR)
        {
            print_and_pause("shutdown failed with error: %d\n", WSAGetLastError());
        }
        readable = false;
    }
    void no_write()
    {
        socket_check(ClientSocket);
        iResult = shutdown(this->ClientSocket, SD_SEND);
        if (iResult == SOCKET_ERROR)
        {
            print_and_pause("shutdown failed with error: %d\n", WSAGetLastError());
        }
        writeable = false;
    }
    int read(void *buffer, int len)
    {
        if (socket_check(ClientSocket) != 0)
            return -1;
        if (!readable)
            print_and_pause("warning: read after no_read");
        return recv(ClientSocket, (char *)buffer, len, 0);
    };
    // ����ֱ���������������������ж�
    int read_full(void *buffer, int len)
    {
        if (socket_check(ClientSocket) != 0)
            return -1;
        if (!readable)
            print_and_pause("warning: read after no_read");
        return recv(ClientSocket, (char *)buffer, len, MSG_WAITALL);
    };
    int write(const void *buffer, int len)
    {
        if (socket_check(ClientSocket) != 0)
            return -1;
        if (!readable)
            print_and_pause("warning: read after no_read");

        return send(this->ClientSocket, (char *)buffer, len, 0);
    }

    int socket_check(SOCKET s)
    {
        if (s == INVALID_SOCKET)
        {
            print_and_pause("error:INVALID_SOCKET\n");
            return -1;
        }
        return 0;
    }
    websocket()
    {
        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0)
        {
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
    const char *sitestr = sitestr_in, *portstr = portstr_in;

protected:
    WSADATA wsaData = {0};
    int iResult = 0;
    char sitestr_in[64] = {0};
    char portstr_in[16] = {0};
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints = {0};

    int iSendResult = 0;
    char recvbuf[DEFAULT_BUFLEN] = {0};
    int recvbuflen = DEFAULT_BUFLEN;
    bool readable = true, writeable = true;
};

// �����
class webserver : public websocket
{
private:
public:
    SOCKET ListenSocket = INVALID_SOCKET;

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
    // �󶨶˿�
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
        if (ListenSocket == INVALID_SOCKET)
        {
            print_and_pause("socket failed with error: %ld\n", WSAGetLastError());
            goto socbinend;
        }

        // Setup the TCP listening socket
        iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
        if (iResult == SOCKET_ERROR)
        {
            print_and_pause("bind failed with error: %d\n", WSAGetLastError());
        }
    socbinend:
        freeaddrinfo(result);
        result = NULL;
        return;
    }
    // �˲�������������ǽ��Ҳ��������ֱ���ͻ�������
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
        if (iResult == SOCKET_ERROR)
        {
            print_and_pause("listen failed with error: %d\n", WSAGetLastError());
            goto wtclend;
        }

        // Accept a client socket
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET)
        {
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
    void reset()
    {
        if (ListenSocket != INVALID_SOCKET)
            closesocket(ListenSocket);
        if (ClientSocket != INVALID_SOCKET)
            closesocket(ClientSocket);
        ListenSocket = INVALID_SOCKET;
        ClientSocket = INVALID_SOCKET;
        memset(&this->portstr_in, 0, 16);
        memset(&this->sitestr_in, 0, 64);
        memset(&this->hints, 0, sizeof(struct addrinfo));
        memset(&this->wsaData, 0, sizeof(WSADATA));
        memset(&this->recvbuf, 0, DEFAULT_BUFLEN);
        this->recvbuflen = DEFAULT_BUFLEN;
        this->iResult = 0;
        this->iSendResult = 0;
        this->result = NULL;
        this->readable = true;
        this->writeable = true;
    }
};

// �ͻ���
class webclient : public websocket
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
    // �˲�������������ǽ
    void socket_connect()
    {
        if (result == NULL)
        {
            printf("warning: didn't reslove. auto reslove:\n");
            this->resolve();
        }
        // Attempt to connect to an address until one succeeds
        for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
        {

            // Create a SOCKET for connecting to server
            ClientSocket = socket(ptr->ai_family, ptr->ai_socktype,
                                  ptr->ai_protocol);
            if (ClientSocket == INVALID_SOCKET)
            {
                print_and_pause("socket failed with error: %ld\n", WSAGetLastError());
                return;
            }

            // Connect to server.
            iResult = connect(ClientSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (iResult == SOCKET_ERROR)
            {
                closesocket(ClientSocket);
                ClientSocket = INVALID_SOCKET;
                continue;
            }
            return;
        }
        return;
    }

private:
    struct addrinfo *ptr = NULL;
};
class web_example
{
    std::mutex server_f_localhost80;
    void server_f(int serverid = 0)
    {
        Sleep(1000);
        const int server_count_limit = 16;

        if (serverid == 0)
        {
            for (int i = 0; i < server_count_limit; i++)
            {
                std::thread t(&web_example::server_f, this, i);
                t.detach();
            }
        }
        char buf[1024];
        static int server_count = 0;

        webserver ws;
        // uint8_t ipaddr[4] = {127, 0, 0, 1};
    repeat:

        ws.TCP();

        ws.ipv4(127, 0, 0, 1);
        ws.port(80);
        // ws.resolve();
        server_f_localhost80.lock();
        ws.sockct_bind();

        // wait:
        printf("%d waiting for client...\n", serverid);
        ws.wait_client();
        server_f_localhost80.unlock();
        printf("%d client connected\n", serverid);
        printf("%d th new server enabled\n", serverid + 1);
        if (serverid < server_count_limit - 1)
        {
            std::thread t(&web_example::server_f, this, serverid + 1);
            t.detach();
        }
        else
            printf("server count limit reached\n");
        for (int i = 0;; i++)
        {
            Sleep(500);
            int l = ws.read(buf, 1024);
            if (l <= 0)
            {
                printf("%d client disconnected\n", serverid);
                goto repeat;
                break;
            }
            printf("%d server %d th received %d bytes\n", serverid, i, l);

            // show in hex and ascii
            for (int j = 0; j < l; j++)
            {
                printf("%02x ", buf[j]);
                if (j % 16 == 15)
                {
                    printf("  ");
                    for (int k = j - 15; k <= j; k++)
                    {
                        if (buf[k] >= 32 && buf[k] <= 126)
                            printf("%c", buf[k]);
                        else
                            printf(".");
                    }
                    printf("\n");
                }
            }
            printf("\n");
        }
        ws.reset();
        goto repeat;

        return;
    }

    void client_f()
    {
        char buf[1024];
        std::mutex m;
        webclient wc;
        wc.TCP();
        wc.ipv4(127, 0, 0, 1);
        wc.port(80);
        printf("connecting...\n");
        wc.socket_connect();
        printf("connected\n");
        // send a rand number

        for (int i = 0; i < 100; i++)
        {
            int r = rand();
            sprintf(buf, "%d", r);
            wc.write(buf, 32);
            printf("%d th sent,num is %d\n", i, r);
            Sleep(1000);
            memset(buf, 0, 1024);
        }
    }
};

#endif // !_bionukg_websocket_h
