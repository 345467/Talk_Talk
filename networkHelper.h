#pragma once

//#include "WinSock2.h"

namespace Web {

    static WSADATA wsaData;

    inline void use_net() {
        WSAStartup(MAKEWORD(2, 2), &wsaData);
    }

    inline void unuse_net() {
        WSACleanup();
    }

    inline LPWSADATA getWSADATA() {
        return &wsaData;
    }

    enum SocketState {uninit,canuse,closed,error};

    class ServerSocket;

    class Socket {
            SOCKET so;
            SocketState state=uninit;
            SOCKADDR_IN ServerAddr;
            friend class ServerSocket;
            Socket() {}
        public:
            Socket(const char *ipaddr,int port) {
                so = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                ServerAddr.sin_family = AF_INET;
                ServerAddr.sin_port = htons(port);
                ServerAddr.sin_addr.s_addr = inet_addr(ipaddr);
                int result = connect(so, (SOCKADDR *)&ServerAddr,
                                     sizeof(ServerAddr));
                if(result!=0) state=error;
                else state=canuse;
            }
            const SocketState *const getState() {
                return &state;
            }
            ///已弃用
            const SOCKET *const getSocket() {
                return &so;
            }
            const SOCKADDR_IN *const getAddr() {
                return &ServerAddr;
            }
            void send(string msg) {
                int len=msg.size();
                ::send(so,(char *)&len,(int)sizeof(int),0);
                ::send((SOCKET)so,(char *)msg.c_str(),len,0);
            }
            ///不推荐使用，有使程序崩溃的风险
            void send(char *buf,int size) {
                ::send((SOCKET)so,(char *)&size,(int)sizeof(int),0);
                ::send((SOCKET)so,buf,size,0);
            }
            string recv() {
                int len2;
                ::recv(so,(char *)&len2,sizeof(int),0);
                char *buf=new char[len2+1];
                ::recv(so,buf,len2,0);
                buf[len2]=0;
                string str=buf;
                delete[] buf;
                return str;
            }
            ///不推荐使用，有使程序崩溃的风险
            void recv(char *buf,int *size) {
                ::recv(so,(char *)size,sizeof(int),0);
                ::recv(so,buf,*size,0);
            }
            ///不推荐使用，有使程序崩溃的风险
            void recv(char *buf) {
                int size;
                ::recv(so,(char *)&size,sizeof(int),0);
                ::recv(so,buf,size,0);
            }
            void close() {
                if(state!=closed)
                    closesocket(so);
                state=closed;
            }
            ~Socket() {
                close();
            }
    };

    class ServerSocket {
            SocketState state=uninit;
            Socket listener;
        public:
            ServerSocket(int port) {
                listener.so = ::socket(AF_INET, SOCK_STREAM,
                                       IPPROTO_TCP);
                listener.ServerAddr.sin_family = AF_INET;
                listener.ServerAddr.sin_port = htons(port);
                listener.ServerAddr.sin_addr.s_addr = htonl(
                        INADDR_ANY);
                ::bind(listener.so, (SOCKADDR *)&listener.ServerAddr,
                       sizeof(listener.ServerAddr));
                listen(listener.so, 2);
                state=canuse;
            }
            Socket accept() {
                Socket so;
                int len = sizeof(so.ServerAddr);
                so.so = ::accept(listener.so,
                                 (SOCKADDR *)&so.ServerAddr, &len);
                so.state=canuse;
                return so;
            }
            void close() {
                if(state!=closed)
                    closesocket(listener.so);
                state=closed;
            }
            const SocketState *const getState() {
                return &state;
            }
            ~ServerSocket() {
                close();
            }
    };

}
