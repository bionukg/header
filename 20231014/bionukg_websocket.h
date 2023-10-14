#pragma once
// 参考文献
// 完成 Winsock 服务器代码 微软[EB/OL] 2022年11月14日
// https://learn.microsoft.com/zh-cn/windows/win32/winsock/complete-server-code
// 完成 Winsock 客户端代码 微软[EB/OL] 2022年11月14日
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
#include <vector>
#include <mutex>

#include "./bionukg_string.h"
// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")
#include <stdarg.h> // For va_start, etc.
#ifdef namespace_bionukg
namespace bionukg
{
#endif // namespace_bionukg

#define DEFAULT_BUFLEN 512
#define print_and_pause(...)  \
    do                        \
    {                         \
        printf(__VA_ARGS__);  \
        /*system("pause"); */ \
    } while (0);

#define check_return(expression, expected, returned, ...) \
    do                                                    \
    {                                                     \
        if ((expression) != (expected))                   \
        {                                                 \
            print_and_pause(__VA_ARGS__);                 \
            return returned;                              \
        }                                                 \
    } while (0);

    enum protocol_type
    {
        pt_tcp,
        pt_udp,
    };

    class WSA
    {
    public:
        WSADATA wsaData;
        int iResult = 0;
        WSA()
        {
            iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
            if (iResult != 0)
            {
                print_and_pause("WSAStartup failed with error: %d\n", iResult);
            }
        };
        ~WSA()
        {
            if (iResult == 0)
                WSACleanup();
        }
    };
    static WSA wsa_instance;
    class addrinfoobj
    {
    public:
        struct addrinfo *result = NULL;
        struct addrinfo hints = {0};
        int iResult = 0;
        void init_hints(
            decltype(hints.ai_family) ai_family = AF_INET,
            decltype(hints.ai_socktype) ai_socktype = SOCK_STREAM,
            decltype(hints.ai_protocol) ai_protocol = IPPROTO_TCP,
            decltype(hints.ai_flags) ai_flags = 0)
        {
            hints.ai_family = ai_family;
            hints.ai_socktype = ai_socktype;
            hints.ai_protocol = ai_protocol;
            hints.ai_flags = ai_flags;
        };
        addrinfoobj(const char *hostname, const char *port = 0,
                    decltype(hints.ai_family) ai_family = AF_INET,
                    decltype(hints.ai_socktype) ai_socktype = SOCK_STREAM,
                    decltype(hints.ai_protocol) ai_protocol = IPPROTO_TCP,
                    decltype(hints.ai_flags) ai_flags = 0)
        {
            init_hints(ai_family, ai_socktype, ai_protocol, ai_flags);
            iResult = getaddrinfo(hostname, port, &hints, &result);
            check_return(iResult, 0, , "getaddrinfo failed with error: %d\n", iResult);
        };
        void printip()
        {
            char ipstr[32];
            auto ipv4 = [&]()
            {
                for (struct addrinfo *ptr = result; ptr != NULL; ptr = ptr->ai_next)
                {
                    struct sockaddr_in *sockaddr_ip = (struct sockaddr_in *)ptr->ai_addr;
                    inet_ntop(AF_INET, &sockaddr_ip->sin_addr, ipstr, sizeof(ipstr));
                    printf("%s\n", ipstr);
                }
            };
            auto ipv6 = [&]()
            {
                for (struct addrinfo *ptr = result; ptr != NULL; ptr = ptr->ai_next)
                {
                    struct sockaddr_in6 *sockaddr_ip = (struct sockaddr_in6 *)ptr->ai_addr;
                    inet_ntop(AF_INET6, &sockaddr_ip->sin6_addr, ipstr, sizeof(ipstr));
                    printf("%s\n", ipstr);
                }
            };
            switch (hints.ai_family)
            {
            case AF_INET:
                printf("IPv4\n");
                ipv4();
                break;
            case AF_INET6:
                printf("IPv6\n");
                ipv6();
                break;

            default:
                break;
            };
        };
        ~addrinfoobj()
        {
            if (result != NULL)
                freeaddrinfo(result);
        }
    };

    class ip_v4
    {
    public:
        uint8_t ip[4];
        enum ip_type
        {
            localhost,
            this_computer,
            self_ip
        };

        ip_v4(uint8_t ip0 = 127, uint8_t ip1 = 0, uint8_t ip2 = 0, uint8_t ip3 = 1)
        {
            ip[0] = ip0;
            ip[1] = ip1;
            ip[2] = ip2;
            ip[3] = ip3;
        }
        ip_v4(const ip_v4 &ip_) = default;

        ip_v4(const char *ipstr)
        {
            sscanf_s(ipstr, "%hhu.%hhu.%hhu.%hhu", &ip[0], &ip[1], &ip[2], &ip[3]);
        };

        static void uni_printf(FILE *fp = stdout, const char *format = 0, ...)
        {
            va_list arg;
            va_start(arg, format);
            vfprintf(fp, format, arg);
            va_end(arg);
        }
        static FILE *filecast(FILE *in) { return in; }

        class FILEstr
        {

        public:
            char *str;
            int len;
            int seek = 0;
            template <int l>
            FILEstr(char (&s)[l]) : str(s), len(l - 1){};
            FILEstr(char *s, int l) : str(s), len(l - 1){};
        };

        template <int len>
        static FILEstr filecast(char (&str)[len]) { return FILEstr(str); };
        static FILEstr &filecast(FILEstr &fs) { return fs; };
        static void uni_printf(FILEstr &fstr, const char *format, ...)
        {
            va_list arg;
            va_start(arg, format);
            vsnprintf(fstr.str + fstr.seek, fstr.len - fstr.seek, format, arg);
            va_end(arg);
            fstr.seek += strlen(fstr.str + fstr.seek);
        }

        template <int len>
        static void uni_printf(char (&str)[len], const char *format, ...)
        {
            va_list arg;
            va_start(arg, format);
            vsprintf_s(str, format, arg);
            va_end(arg);
        };
        static void uni_printf(char *str, int len, const char *format, ...)
        {
            va_list arg;
            va_start(arg, format);
            vsprintf_s(str, len, format, arg);
            va_end(arg);
        }

        template <int len>
        static void self_ip_info(char (&out)[len])
        {
            return all_self_ip_info(filecast(out));
        }
        template <typename T>
        static void self_ip_info(T out)
        {
            return all_self_ip_info(filecast(out));
        }
        static void all_self_ip_info(auto output)
        {
            // startwsa
            // WSA wsaobj;
            int iResult = 0;
            // get local host name
            char hostname[256];
            iResult = gethostname(hostname, 256);
            check_return(iResult, 0, , "gethostname failed with error: %d\n", iResult);

            // get local ip
            // use getaddrinfo
            struct addrinfo *result = NULL;
            addrinfoobj addrinfoobjobj(hostname, NULL);
            result = addrinfoobjobj.result;
            // get ip
            char ipstr[16];
            auto outf = filecast(output);
            for (struct addrinfo *ptr = result; ptr != NULL; ptr = ptr->ai_next)
            {
                struct sockaddr_in *sockaddr_ipv4 = (struct sockaddr_in *)ptr->ai_addr;
                inet_ntop(AF_INET, &sockaddr_ipv4->sin_addr, ipstr, 16);
                uni_printf(outf, "%s\n", ipstr);
            }
            return;
        }
    };

    class ip_v6
    {
    public:
        uint16_t ip[8];
        ip_v6(uint16_t ip0 = 0, uint16_t ip1 = 0, uint16_t ip2 = 0, uint16_t ip3 = 0,
              uint16_t ip4 = 0, uint16_t ip5 = 0, uint16_t ip6 = 0, uint16_t ip7 = 0)
            : ip{ip0, ip1, ip2, ip3, ip4, ip5, ip6, ip7} {};
        ip_v6(const ip_v6 &ip_) = default;
    };

    // 包含客户端和服务端的公有部分
    class websocket
    {
    public:
        // WSA wsaobj;
        //  比如127,0,0,1
        void ipv4(uint8_t ipaddr0, uint8_t ipaddr1, uint8_t ipaddr2, uint8_t ipaddr3)
        {
            this->sitestr = sitestr_in;
            uint8_t ipaddr[4] = {ipaddr0, ipaddr1, ipaddr2, ipaddr3};
            hints.ai_family = AF_INET;
            sprintf_s(sitestr_in, "%d.%d.%d.%d", ipaddr0, ipaddr1, ipaddr2, ipaddr3);
            return;
        }
        void ipv4(ip_v4 ipaddr)
        {
            this->sitestr = sitestr_in;
            hints.ai_family = AF_INET;
            sprintf_s(sitestr_in, "%d.%d.%d.%d", ipaddr.ip[0], ipaddr.ip[1], ipaddr.ip[2], ipaddr.ip[3]);
            return;
        }
        void ipv4(const char *ipaddrstr)
        {
            this->sitestr = sitestr_in;
            hints.ai_family = AF_INET;
            for (int i = 0; i < 16; i++)
            {
                sitestr_in[i] = ipaddrstr[i];
                if (ipaddrstr[i] == '\0')
                {
                    sitestr_in[i] = '\0';
                    break;
                }
            }
            return;
        }

        void ipv6(uint16_t ipaddr0 = 0, uint16_t ipaddr1 = 0, uint16_t ipaddr2 = 0, uint16_t ipaddr3 = 0,
                  uint16_t ipaddr4 = 0, uint16_t ipaddr5 = 0, uint16_t ipaddr6 = 0, uint16_t ipaddr7 = 0)
        {
            this->sitestr = sitestr_in;
            uint16_t ipaddr[8] = {ipaddr0, ipaddr1, ipaddr2, ipaddr3, ipaddr4, ipaddr5, ipaddr6, ipaddr7};
            hints.ai_family = AF_INET6;
            sprintf_s(sitestr_in, "%x:%x:%x:%x:%x:%x:%x:%x", ipaddr0, ipaddr1, ipaddr2, ipaddr3, ipaddr4, ipaddr5, ipaddr6, ipaddr7);
            return;
        };
        void ipv6(ip_v6 ipaddr)
        {
            this->sitestr = sitestr_in;
            hints.ai_family = AF_INET6;
            sprintf_s(sitestr_in, "%x:%x:%x:%x:%x:%x:%x:%x", ipaddr.ip[0], ipaddr.ip[1], ipaddr.ip[2], ipaddr.ip[3], ipaddr.ip[4], ipaddr.ip[5], ipaddr.ip[6], ipaddr.ip[7]);
            return;
        };
        void ipv6(const char *ipaddrstr)
        {
            this->sitestr = sitestr_in;
            hints.ai_family = AF_INET6;
            for (int i = 0; i < 64; i++)
            {
                sitestr_in[i] = ipaddrstr[i];
                if (ipaddrstr[i] == '\0')
                {
                    sitestr_in[i] = '\0';
                    break;
                }
            }
            return;
        }

        /// <summary>
        /// 以域名形式访问，应该是自动DNS了罢
        /// </summary>
        /// <param name="url">：域名，如learn.microsoft.com</param>
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
        // 端口号,理论上是0~65535
        void port(uint16_t port)
        {
            sprintf_s(portstr_in, "%d", port);
            return;
        }
        // 按照协议类型自动找端口，比如http,ftp等等
        void port(const char *protocol_str)
        {
            struct servent *serv = getservbyname(protocol_str, NULL);
            if (serv == NULL)
            {
                print_and_pause("error: getservbyname failed.\n");
                return;
            }
            sprintf_s(portstr_in, "%d", ntohs(serv->s_port));
            return;
        }

        // 解析地址
        void resolve()
        {
            // iResult = getaddrinfo("127.0.0.1", "80", &hints, &result);
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
        // 阻塞直到填满缓冲区或者连接中断
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
            if (!writeable)
                print_and_pause("warning: write after no_write");

            return send(this->ClientSocket, (char *)buffer, len, 0);
        }

        int printbuf(const char *buf, int l, int byte_per_line = 16, FILE *fp = stdout)
        {

            // show in hex and ascii
            for (int j = 0; j < l; j++)
            {
                printf("%02x ", (uint8_t)buf[j]);
                if (j % byte_per_line == (byte_per_line - 1))
                {
                    printf("  ");
                    for (int k = j - (byte_per_line - 1); k <= j; k++)
                    {
                        if (buf[k] >= 32 && buf[k] <= 126)
                            printf("%c", buf[k]);
                        else
                            printf(".");
                    }
                    printf("\n");
                }
                if (j == l - 1)
                {
                    for (int fill = j; fill % byte_per_line != byte_per_line - 1; fill++)
                    {
                        printf("   ");
                    }
                    printf("  ");

                    for (int k = j - j % byte_per_line; k <= j; k++)
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
        };

        int printread(int byte_per_line = 16, FILE *fp = stdout)
        {
            char buf[1024];
            int l = read(buf, 1024);
            if (l <= 0)
            {
                return l;
            }

            // show in hex and ascii
            for (int j = 0; j < l; j++)
            {
                printf("%02x ", (uint8_t)buf[j]);
                if (j % byte_per_line == (byte_per_line - 1))
                {
                    printf("  ");
                    for (int k = j - (byte_per_line - 1); k <= j; k++)
                    {
                        if (buf[k] >= 32 && buf[k] <= 126)
                            printf("%c", buf[k]);
                        else
                            printf(".");
                    }
                    printf("\n");
                }
                if (j == l - 1)
                {
                    for (int fill = j; fill % byte_per_line != byte_per_line - 1; fill++)
                    {
                        printf("   ");
                    }
                    printf("  ");

                    for (int k = j - j % byte_per_line; k <= j; k++)
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
            return l;
        };

        template <int len>
        int write(const char (&buffer)[len])
        {
            return write(buffer, len);
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
        int socket_off(SOCKET &s)
        {
            int ret = 0;
            if (s != INVALID_SOCKET)
            {
                ret = closesocket(s);
            }
            s = INVALID_SOCKET;
            return ret;
        };

        websocket()
        {

            return;
        };
        ~websocket()
        {

            if (ClientSocket != INVALID_SOCKET)
            {
                no_read();
                no_write();
                socket_off(ClientSocket);
            }

            return;
        };
        /// <summary>
        /// 默认指向内部字符串，其值仅由内置函数处理，外部不要修改。
        /// 可以在外部修改好，然后把这个指针重定向到外部字符串
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

    // 服务端
    class webserver : public websocket
    {
    private:
    public:
        protocol_type ptype = protocol_type::pt_tcp;
        SOCKET ListenSocket = INVALID_SOCKET;

        void protocol(protocol_type pt)
        {
            ptype = pt;
            switch (pt)
            {
            case pt_tcp:
                TCP();
                break;
            case pt_udp:
                UDP();
                break;
            default:
                break;
            }
        }
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
        // 绑定端口
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
                print_and_pause("socket failed with wsa error: %d\n", WSAGetLastError());
                goto socbinend;
            }

            // Setup the TCP listening socket
            iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
            if (iResult == SOCKET_ERROR)
            {
                print_and_pause("bind failed with wsa error: %d\n", WSAGetLastError());
            }
        socbinend:
            freeaddrinfo(result);
            result = NULL;
            return;
        }
        // 此步可能碰到防火墙，也会阻塞，直到客户端连接
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
            if (ptype == pt_udp)
            {
                this->ClientSocket = this->ListenSocket;
                this->ListenSocket = INVALID_SOCKET;
                return;
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
            return;
        };
        void finish()
        {
            // close the connection
            if (ClientSocket != INVALID_SOCKET)
                socket_off(ClientSocket);
        };

        void close()
        {
            return finish();
        }
        ~webserver()
        {
            reset();
        }
        void reset()
        {
            if (ListenSocket != INVALID_SOCKET)
                socket_off(ListenSocket);
            if (ClientSocket != INVALID_SOCKET)
                socket_off(ClientSocket);
            ListenSocket = INVALID_SOCKET;
            ClientSocket = INVALID_SOCKET;
            memset(&this->portstr_in, 0, sizeof(portstr_in));
            memset(&this->sitestr_in, 0, sizeof(sitestr_in));
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

    // 客户端
    class webclient : public websocket
    {
    public:
        void protocol(protocol_type pt)
        {
            switch (pt)
            {
            case pt_tcp:
                TCP();
                break;
            case pt_udp:
                UDP();
                break;
            default:
                break;
            }
        }

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
        // 此步可能碰到防火墙
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
                    socket_off(ClientSocket);
                    continue;
                }
                return;
            }
            return;
        }

    private:
        struct addrinfo *ptr = NULL;
    };

    class HTTP
    {

        enum requestType : uint64_t
        {
            null_request_type = ""_raw64,
            CONNECT_type = "CONNECT"_raw64,
            DELETE_type = "DELETE"_raw64,
            GET_type = "GET"_raw64,
            HEAD_type = "HEAD"_raw64,
            OPTIONS_type = "OPTIONS"_raw64,
            PATCH_type = "PATCH"_raw64,
            POST_type = "POST"_raw64,
            PUT_type = "PUT"_raw64,
            TRACE_type = "TRACE"_raw64
        };
        enum version : uint64_t
        {
            null_version = ""_raw64,
            HTTP_1_0_version = "HTTP/1.0"_raw64,
            HTTP_1_1_version = "HTTP/1.1"_raw64,
            HTTP_2_0_version = "HTTP/2.0"_raw64
        };

        enum responseIndex
        {
            OK_index,
            Created_index,
            Not_Found_index,
            Internal_Server_Error_index,
            Not_Implemented_index,
            Bad_Gateway_index,
            Service_Unavailable_index,
            Gateway_Timeout_index,
            HTTP_Version_Not_Supported_index,
            response_max
        };
        constexpr static const char *responseString[response_max] = {
            "OK",
            "Created",
            "Not Found",
            "Internal Server Error",
            "Not Implemented",
            "Bad Gateway",
            "Service Unavailable",
            "Gateway Timeout",
            "HTTP Version Not Supported",
        };
        constexpr static const uint16_t responseCodeList[response_max] = {
            200,
            201,
            404,
            500,
            501,
            502,
            503,
            504,
            505,
        };
        enum responseCode : uint16_t
        {
            code_OK = 200,
            code_Created = 201,
            code_Not_Found = 404,
            code_Internal_Server_Error = 500,
            code_Not_Implemented = 501,
            code_Bad_Gateway = 502,
            code_Service_Unavailable = 503,
            code_Gateway_Timeout = 504,
            code_HTTP_Version_Not_Supported = 505,
        };
        enum responseType : uint64_t
        {
            null_response_type = ""_base64,
            OK_type = operator_base64(responseString[OK_index]),
            Created_type = operator_base64(responseString[Created_index]),
            Not_Found_type = operator_base64(responseString[Not_Found_index]),
            Internal_Server_Error_type = operator_base64(responseString[Internal_Server_Error_index]),
            Not_Implemented_type = operator_base64(responseString[Not_Implemented_index]),
            Bad_Gateway_type = operator_base64(responseString[Bad_Gateway_index]),
            Service_Unavailable_type = operator_base64(responseString[Service_Unavailable_index]),
            Gateway_Timeout_type = operator_base64(responseString[Gateway_Timeout_index]),
            HTTP_Version_Not_Supported_type = operator_base64(responseString[HTTP_Version_Not_Supported_index]),
        };

        responseIndex responseType2Index(responseType t)
        {
            switch (t)
            {
            case OK_type:
                return OK_index;
            case Created_type:
                return Created_index;
            case Not_Found_type:
                return Not_Found_index;
            case Internal_Server_Error_type:
                return Internal_Server_Error_index;
            case Not_Implemented_type:
                return Not_Implemented_index;
            case Bad_Gateway_type:
                return Bad_Gateway_index;
            case Service_Unavailable_type:
                return Service_Unavailable_index;
            case Gateway_Timeout_type:
                return Gateway_Timeout_index;
            case HTTP_Version_Not_Supported_type:
                return HTTP_Version_Not_Supported_index;
            default:
                return response_max;
            }
        };
        responseCode responseType2Code(responseType t)
        {
            return (responseCode)responseCodeList[responseType2Index(t)];
        };
        enum requestIndex
        {
            CONNECT_index,
            DELETE_index,
            GET_index,
            HEAD_index,
            OPTIONS_index,
            PATCH_index,
            POST_index,
            PUT_index,
            TRACE_index,
            request_max
        };
        const char *requestString[request_max] = {
            "CONNECT",
            "DELETE",
            "GET",
            "HEAD",
            "OPTIONS",
            "PATCH",
            "POST",
            "PUT",
            "TRACE",
        };

    public:
        static uint64_t operator_raw64(const char *str, int len)
        {
            uint64_t ret = 0;
            for (int i = 0; i < len; i++)
            {
                ret = (ret << 8) + (uint8_t)str[i];
            }
            return ret;
        }
        static int operator_raw64_reverse(uint64_t raw64, char *str, int len)
        {
            int idx = 0;
            for (int i = len - 1; i >= 0; i--)
            {
                str[idx++] = (raw64 >> (i * 8)) & 0xff;
            }
            return idx;
        }

        class request_first_line
        {
        public:
            requestType type;
            std::string url;
            version ver;
            int line_len = 0;

        public:
            int init(char *datagram, int len)
            {
                // get the length of first line
                line_len = 0;
                for (; line_len < len; line_len++)
                {
                    if (datagram[line_len] == '\r' && datagram[line_len + 1] == '\n')
                    {
                        line_len += 2;
                        break;
                    }
                }
                // get the length of first token
                auto get_token = [&](int start) -> int
                {
                    int token_len = 0;
                    for (; token_len + start < line_len; token_len++)
                    {
                        if (datagram[start + token_len] == ' ')
                        {
                            break;
                        }
                    }
                    return token_len;
                };
                int idx = 0, token_len = 0;
                // get the type
                token_len = get_token(idx);
                this->type = (requestType)operator_raw64(datagram + idx, token_len);
                idx += token_len + 1;
                // get the url
                token_len = get_token(idx);
                url.resize(token_len + 1);
                memcpy(&url[0], datagram + idx, token_len);
                url[token_len] = 0;
                idx += token_len + 1;
                // get the version
                token_len = get_token(idx);
                this->ver = (version)operator_raw64(datagram + idx, token_len);
                idx += token_len + 2;
                if (idx != line_len)
                    throw std::exception("request line error");

                return line_len;
            }
            request_first_line() : type(null_request_type), ver(null_version){};
            request_first_line(char *datagram, int len, int start = 0)
            {
                line_len = this->init(datagram + start, len);
            }

            template <int len>
            request_first_line(requestType req, const char (&urls)[len], version vers = HTTP_1_1_version)
                : type(req), url(urls), ver(vers){};

            int output(char *datagram, int len)
            {
                int idx = 0;
                // get the type
                idx += operator_raw64_reverse(type, datagram + idx, 8);
                datagram[idx++] = ' ';
                // get the url
                memcpy(datagram + idx, url.data(), url.size());
                idx += url.size();
                datagram[idx++] = ' ';
                // get the version
                idx += operator_raw64_reverse(ver, datagram + idx, 8);
                datagram[idx++] = '\r';
                datagram[idx++] = '\n';
                return idx;
            };

            ~request_first_line()
            {
            }
        };
        class response_first_line
        {
        public:
            responseType type;
            responseCode code;
            version ver;
            int line_len = 0;

        public:
            int init(char *datagram, int len)
            {
                // get the length of first line
                line_len = 0;
                for (; line_len < len; line_len++)
                {
                    if (datagram[line_len] == '\r' && datagram[line_len + 1] == '\n')
                    {
                        line_len += 2;
                        break;
                    }
                }
                // get the length of first token
                auto get_token = [&](int start) -> int
                {
                    int token_len = 0;
                    for (; token_len + start < line_len; token_len++)
                    {
                        if (datagram[start + token_len] == ' ')
                        {
                            break;
                        }
                    }
                    return token_len;
                };
                int idx = 0, token_len = 0;
                // get the version
                token_len = get_token(idx);
                this->ver = (version)operator_raw64(datagram + idx, token_len);
                idx += token_len + 1;
                // get the code
                token_len = get_token(idx);
                this->code = (responseCode)atoi(std::string_view(datagram + idx, token_len).data());
                idx += token_len + 1;
                // get the type
                token_len = line_len - idx - 2;
                this->type = (responseType)operator_raw64(datagram + idx, token_len);
                idx += token_len + 2;
                if (idx != line_len)
                    throw std::exception("response line error");

                return line_len;
            }
            response_first_line() : ver(HTTP_1_1_version), type(OK_type){};
            response_first_line(char *datagram, int len, int start = 0)
            {
                line_len = this->init(datagram + start, len);
            }

            response_first_line(responseType rtype = OK_type, version vers = HTTP_1_1_version)
                : type(rtype), ver(vers){};

            int output(char *datagram, int len){

            };

            ~response_first_line()
            {
            }
        };

        class header_line
        {
        public:
            uint64_t typeb64 = 0;
            std::string line;
            std::string_view type;
            std::string_view value;
            int line_len = 0;

        public:
            int init(char *datagram, int len)
            {
                // get the length of first line
                line_len = 0;
                for (; line_len < len; line_len++)
                {
                    if (datagram[line_len] == '\r' && datagram[line_len + 1] == '\n')
                    {
                        line_len += 2;
                        break;
                    }
                }
                // get the length of first token
                auto get_token = [&](int start) -> int
                {
                    int token_len = 0;
                    for (; token_len + start < line_len; token_len++)
                    {
                        if (datagram[start + token_len] == ':' || datagram[start + token_len] == ' ')
                        {
                            break;
                        }
                    }
                    return token_len;
                };
                auto next_token_start = [&](int start) -> int
                {
                    int next_start = 0;
                    for (; start + next_start < line_len; next_start++)
                    {
                        if (datagram[start + next_start] != ' ' && datagram[start + next_start] != ':')
                        {
                            break;
                        }
                    }
                    return next_start;
                };

                int idx = 0, token_len = 0;
                // get the type
                token_len = get_token(idx);
                this->type = std::string_view(datagram + idx, token_len);
                this->typeb64 = operator_base64(datagram + idx, token_len);

                idx += token_len + next_token_start(idx);
                // the rest to the end of line is value,except '\r\n'
                token_len = line_len - idx - 2;
                this->value = std::string_view(datagram + idx, token_len);
                idx += token_len + 2;
                if (idx != line_len)
                    throw std::exception("header_line error");

                return line_len;
            }
            header_line() = default;
            header_line(char *datagram, int len, int start = 0)
            {
                line_len = this->init(datagram + start, len);
            }
        };

        class request_header
        {
        public:
            request_first_line first_line;
            std::vector<header_line> lines;
            int header_len = 0;

        public:
            request_header() = delete;
            request_header(char *datagram, int len)
            {
                header_len = len;
                int idx = 0;
                // get the first line
                idx = this->first_line.init(datagram, len);
                // get the rest lines
                while (idx < len)
                {
                    lines.emplace_back(datagram, len, idx);
                    idx += lines.back().line_len;
                }
            }
        };

    public:
        char (&buf)[512];
        int bufl = 0;
        int len;
        int idx = 0;
        uint8_t *datagram;

        HTTP(uint8_t *datagram_ = 0, int len_ = 0) : buf(*decltype (&buf)(new char[512])), len(len_), datagram(datagram_)
        {
            return;
        }
        ~HTTP()
        {
            delete[] (char *)buf;
            return;
        }

        // get next piece of datagram, store in buf
        void get_next(char sep = ' ')
        {
            // separated by space
            for (bufl = 0;
                 datagram[idx] != sep &&
                 idx < len &&
                 (bufl < sizeof(buf) - 1);
                 idx++, bufl++)
            {
                buf[bufl] = datagram[idx];
            }
            buf[bufl + 1] = 0;
            // to next non-space
            for (; datagram[idx] == sep && idx < len; idx++)
            {
            }
        }

        uint16_t get_port()
        {
            int ret = 80;

            // search back for ':'
            for (int i = 1; i < 7; i++)
            {
                if (buf[bufl - i] == ':')
                {
                    bufl -= i;
                    sscanf_s(&buf[bufl + 1], "%d", &ret);
                    break;
                }
            }

            return ret;
        }

    public:
        requestType rtype = null_request_type;
        ip_v4 destip;
        uint16_t port = 0;
        version http_version = null_version;
        void parse()
        {
            // get request type
            get_next();

            rtype = requestType(operator_raw64(buf, bufl));

            // get dest site
            get_next();
            // find the port
            port = get_port();
            // get http version
            get_next('\r');
            http_version = version(operator_raw64(buf, bufl));
        }
    };

    const char http_response_example[] = {
        "HTTP/1.1 200 OK\r\n"         // 响应行：HTTP版本，状态码，状态描述
        "Content-Length: 63\r\n"      // 内容长度：从空行开始的长度
        "Content-Type: text/html\r\n" // 内容类型：文本-超文本标记语言
        "Connection: Closed\r\n"      // 关闭连接，keep-alive是保持连接
        "\r\n"                        // 一个空行，表示响应头结束
        "<html>\r\n"
        " <body>\r\n"
        "  <h1>Hello, World!</h1>\r\n"
        " </body>\r\n"
        "</html>\r\n"
        ""};
    const char http_response_404[] = {
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Length: 63\r\n"
        "Content-Type: text/html\r\n"
        "Connection: Closed\r\n"
        "\r\n"
        "<html>\r\n"
        " <body>\r\n"
        "  <h1>404 Not Found</h1>\r\n"
        " </body>\r\n"
        "</html>\r\n"
        ""};
    const char http_response_icon[] = {
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: %d\r\n"
        "Content-Type: image/x-icon\r\n"
        "Connection: Closed\r\n"
        "\r\n"
        ""};

    class HTTP_request
    {
    public:
        typedef int (*send_t)(const char *, int);
        send_t sender = 0;

        enum request_type
        {
            CONNECT_type,
            DELETE_type,
            GET_type,
            HEAD_type,
            OPTIONS_type,
            PATCH_type,
            POST_type,
            PUT_type,
            TRACE_type,
            request_type_max
        } rtype = GET_type;
        inline static const char *request_type_string[request_type_max] = {
            "CONNECT",
            "DELETE",
            "GET",
            "HEAD",
            "OPTIONS",
            "PATCH",
            "POST",
            "PUT",
            "TRACE",
        };

        enum version
        {
            HTTP_1_0_version,
            HTTP_1_1_version,
            HTTP_2_0_version,
            version_max
        } ver = HTTP_1_1_version;
        inline static const char *version_string[version_max] = {
            "HTTP/1.0",
            "HTTP/1.1",
            "HTTP/2.0",
        };

        enum header_type
        {
            Accept_hdtype,
            Accept_Charset_hdtype,
            Accept_Encoding_hdtype,
            Accept_Language_hdtype,
            Accept_Datetime_hdtype,
            Access_Control_Request_Method_hdtype,
            Access_Control_Request_Headers_hdtype,
            Authorization_hdtype,
            Cache_Control_hdtype,
            Connection_hdtype,
            Content_Length_hdtype,
            Content_MD5_hdtype,
            Content_Type_hdtype,
            Cookie_hdtype,
            Date_hdtype,
            Expect_hdtype,
            Forwarded_hdtype,
            From_hdtype,
            Host_hdtype,
            HTTP2_Settings_hdtype,
            If_Match_hdtype,
            If_Modified_Since_hdtype,
            If_None_Match_hdtype,
            If_Range_hdtype,
            If_Unmodified_Since_hdtype,
            Max_Forwards_hdtype,
            Origin_hdtype,
            Pragma_hdtype,
            Proxy_Authorization_hdtype,
            Range_hdtype,
            Referer_hdtype,
            TE_hdtype,
            User_Agent_hdtype,
            Upgrade_hdtype,
            Via_hdtype,
            Warning_hdtype,
            header_type_max
        } htype;
        inline static const char *header_type_string[header_type_max] = {
            "Accept",
            "Accept-Charset",
            "Accept-Encoding",
            "Accept-Language",
            "Accept-Datetime",
            "Access-Control-Request-Method",
            "Access-Control-Request-Headers",
            "Authorization",
            "Cache-Control",
            "Connection",
            "Content-Length",
            "Content-MD5",
            "Content-Type",
            "Cookie",
            "Date",
            "Expect",
            "Forwarded",
            "From",
            "Host",
            "HTTP2-Settings",
            "If-Match",
            "If-Modified-Since",
            "If-None-Match",
            "If-Range",
            "If-Unmodified-Since",
            "Max-Forwards",
            "Origin",
            "Pragma",
            "Proxy-Authorization",
            "Range",
            "Referer",
            "TE",
            "User-Agent",
            "Upgrade",
            "Via",
            "Warning",
        };

        template <header_type ht>
        class headerclass
        {
        };
        template <>
        class headerclass<Accept_hdtype>
        {
        public:
            inline static const header_type hdtype = Accept_hdtype;
            class val
            {
            public:
                enum text
                {
                    text_html,
                    text_plain,
                    text_css,
                    text_xml,
                    text_javascript,
                    text_json,
                    text_max
                };
                inline static const char *text_string[text_max] = {
                    "text/html",
                    "text/plain"
                    "text/css",
                    "text/xml",
                    "text/javascript",
                    "text/json",
                };
                enum image
                {
                    image_x_icon,
                    image_gif,
                    image_jpeg,
                    image_png,
                    image_svg_xml,
                    image_webp,
                    image_max
                };
                inline static const char *image_string[image_max] = {
                    "image/x-icon",
                    "image/gif",
                    "image/jpeg",
                    "image/png",
                    "image/svg+xml",
                    "image/webp",
                };
                enum audio
                {
                    audio_mpeg,
                    audio_wav,
                    audio_ogg,
                    audio_max
                };
                inline static const char *audio_string[audio_max] = {
                    "audio/mpeg",
                    "audio/wav",
                    "audio/ogg"};
                enum video
                {
                    video_mp4,
                    video_avi,
                    video_webm,
                    video_max
                };
                inline static const char *video_string[video_max] = {
                    "video/mp4",
                    "video/avi",
                    "video/webm"};
                enum application
                {
                    application_x_www_form_urlencoded,
                    application_octet_stream,
                    application_pdf,
                    application_zip,
                    application_java_archive,
                    application_javascript,
                    application_json,
                    application_xml,
                    application_max
                };
                inline static const char *application_string[application_max] = {
                    "application/x-www-form-urlencoded",
                    "application/octet-stream",
                    "application/pdf",
                    "application/zip",
                    "application/java-archive",
                    "application/javascript",
                    "application/json",
                    "application/xml"};
            };

            static const char *value(val::text t)
            {
                return (t >= 0 && t < val::text_max) ? val::text_string[t] : "text/*";
            }
            static const char *value(val::image t)
            {
                return (t >= 0 && t < val::image_max) ? val::image_string[t] : "image/*";
            }
            static const char *value(val::audio t)
            {
                return (t >= 0 && t < val::audio_max) ? val::audio_string[t] : "audio/*";
            }
            static const char *value(val::video t)
            {
                return (t >= 0 && t < val::video_max) ? val::video_string[t] : "video/*";
            }
            static const char *value(val::application t)
            {
                return (t >= 0 && t < val::application_max) ? val::application_string[t] : "application/*";
            }
            template <typename T>
            static const char *value(T t)
            {
                return "*/*";
            }
        };

        template <>
        class headerclass<Accept_Charset_hdtype>
        {
        public:
            inline static const header_type hdtype = Accept_Charset_hdtype;
            class val
            {
            public:
                enum charset
                {
                    charset_utf_8,
                    charset_iso_8859_1,
                    charset_us_ascii,
                    charset_max,
                    charset_default = charset_utf_8
                };
                inline static const char *charset_string[charset_max] = {
                    "utf-8",
                    "iso-8859-1",
                    "us-ascii",
                };
            };

            static const char *value(val::charset t)
            {
                return (t >= 0 && t < val::charset_max) ? val::charset_string[t] : val::charset_string[val::charset_default];
            };
        };

        template <>
        class headerclass<Accept_Encoding_hdtype>
        {
        };
        template <typename T0, typename... Args>
        int send(T0 arg0, Args... args)
        {
            int first = send_single(arg0);
            if (first < 0)
                return first;
            else
                return first + send(args...);
        }

        int send()
        {
            return 0;
        }

        int send_single(const char *str)
        {
            if (sender == 0)
                return -1;
            return sender(str, strlen(str));
        }
        int send_single(const char *str, int len)
        {
            if (sender == 0)
                return -1;
            return sender(str, len);
        }

        template <int len>
        int send_single(const char (&str)[len])
        {
            if (sender == 0)
                return -1;
            return sender(str, len - 1);
        }
        int send_single()
        {
            return 0;
        }

        void make_request()
        {
            // send request line
            send(request_type_string[rtype], " ", "url", " ", version_string[ver], "\r\n");
            // send headers

            // send empty line
            send("\r\n");
        }
    };

    class web_example
    {
    public:
        ip_v4 ip;
        uint16_t port;
        web_example(uint8_t ip0 = 127, uint8_t ip1 = 0, uint8_t ip2 = 0, uint8_t ip3 = 1, uint16_t por = 80) : ip(ip0, ip1, ip2, ip3), port(por){};
        web_example(ip_v4 ip_, uint16_t por = 80) : ip(ip_), port(por){};
        std::mutex server_f_localhost80;
        void server_f(protocol_type pt = pt_tcp, int serverid = 0)
        {
            Sleep(1000);
            const int server_count_limit = 16;

            // if (serverid == 0)
            //{
            //     for (int i = 0; i < server_count_limit; i++)
            //     {
            //         std::thread t(&web_example::server_f, this, i);
            //         t.detach();
            //     }
            // }
            char buf[1024];
            static int server_count = 0;

            static webserver ws;
            // uint8_t ipaddr[4] = {127, 0, 0, 1};

        repeat:
            if (serverid == 0)
            {
                ws.protocol(pt);

                ws.ipv4(ip);
                ws.port(port);
            }
            // ws.resolve();
            server_f_localhost80.lock();
            if (serverid == 0)
                ws.sockct_bind();
            else
            {
                printf("next");
            }

            // wait:
            printf("%d waiting for client...\n", serverid);
            ws.wait_client();
            server_f_localhost80.unlock();
            printf("%d client connected\n", serverid);
            if (serverid < server_count_limit - 1 && ws.ptype == pt_tcp)
            {
                std::thread t(&web_example::server_f, this, pt, serverid + 1);
                printf("%d th new server enabled\n", serverid + 1);
                t.detach();
            }
            else if (ws.ptype == pt_udp)
            {
                printf("udp uses one thread\n");
            }
            else
            {
                printf("server count limit reached\n");
            }
            for (int i = 0;; i++)
            {
                Sleep(500);
                memset(buf, 0, sizeof(buf));
                int l = ws.read(buf, 1024);
                if (l <= 0)
                {
                    printf("%d client disconnected\n", serverid);
                    goto repeat;
                    break;
                }
                printf("%d server %d th received %d bytes\n", serverid, i, l);

                const int byte_per_line = 16;

                // show in hex and ascii
                for (int j = 0; j < l; j++)
                {
                    printf("%02x ", (uint8_t)buf[j]);
                    if (j % byte_per_line == (byte_per_line - 1))
                    {
                        printf("  ");
                        for (int k = j - (byte_per_line - 1); k <= j; k++)
                        {
                            if (buf[k] >= 32 && buf[k] <= 126)
                                printf("%c", buf[k]);
                            else
                                printf(".");
                        }
                        printf("\n");
                    }
                    if (j == l - 1)
                    {
                        for (int fill = j; fill % byte_per_line != byte_per_line - 1; fill++)
                        {
                            printf("   ");
                        }
                        printf("  ");

                        for (int k = j - j % byte_per_line; k <= j; k++)
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

        void client_f(protocol_type pt = pt_tcp)
        {
            char buf[1024];
            std::mutex m;
            webclient wc;
            wc.protocol(pt);
            wc.ipv4(ip);
            wc.port(port);
            printf("connecting...\n");
            wc.socket_connect();
            printf("connected\n");
            // send a rand number

            for (int i = 0; i < 100; i++)
            {
                int r = rand();
                int slen = sprintf_s(buf, "%d", r);
                int w = wc.write(buf, slen);
                printf("%d th sent,number of string is %d, returned %d.\n", i, r, w);
                if (w == -1)
                {
                    printf("connection is off");
                    break;
                }
                Sleep(1000);
                memset(buf, 0, 1024);
            }
            system("pause");
        }

        void run(protocol_type pt = pt_tcp)
        {
            std::cout << "0 for server, 1 for client\n";
            int i;
            std::cin >> i;
            switch (i)
            {
            case 0:
                server_f(pt);
                break;
            case 1:
                client_f(pt);
                break;
            default:
                break;
            }
        }

#ifdef _bionukg_graphics_h
        void server_helloworld()
        {
            bmp pic(bmphd(32, 32, 24));
            pic.write_each(
                [](uint32_t x, uint32_t y) -> uint32_t
                {
                    uint32_t x0 = x * 8;
                    uint32_t y0 = y * 8;
                    return (x0 << 16) | (y0 << 8) | (x0 | y0);
                });
            HTTP h;
            for (;;)
            {
                webserver ws;
                ws.TCP();
                ws.ipv4(ip);
                ws.port(port);
                ws.resolve();
                ws.sockct_bind();
                ws.wait_client();
                int r = ws.printread(32);
                int x = 1;
                while (r > 0)
                {

                    if (x == 1)
                    {
                        r = ws.write(http_response_example);
                        x = 0;
                    }
                    else
                    {
                        char buffer[4096];
                        int l = sprintf_s(buffer, http_response_icon, pic.size);
                        pic.save_as_mem(buffer + l);
                        r = ws.write(buffer, l + pic.size);
                        x = 1;
                    }

                    // if (x == 1)
                    //{
                    //     char buffer[4096];
                    //     int idx = 0;
                    //     memcpy(buffer, http_response_example, sizeof(http_response_example));
                    //     idx += sizeof(http_response_example);
                    //     int l = sprintf_s(buffer + idx, sizeof(buffer) - idx, http_response_icon, pic.size);
                    //     pic.save_as_mem(buffer + idx + l);
                    //     r = ws.write(buffer, idx + l + pic.size);
                    //     x = 0;
                    // }
                    r = ws.printread(32);
                }
                printf("send\nread\n");

                ws.close();
            }
        }

#endif
    };
#ifdef namespace_bionukg
};
#endif // namespace_bionukg

#endif // !_bionukg_websocket_h
