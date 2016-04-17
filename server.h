#ifndef __SERVER_
#define __SERVER_

#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

typedef struct sockaddr addr_type;
typedef struct sockaddr_in inetAddr_type;

void setAddr(struct sockaddr_in& sv_addr, const char* ip, const int& port) {
	sv_addr.sin_family = AF_INET;
	sv_addr.sin_port = htons(port);
	inet_aton(ip, &sv_addr.sin_addr);
}

void setAddrAndBind(int fd, const char* ip, const int& port) {
	struct sockaddr_in addr;
	setAddr(addr, ip, port);
	bind(fd, (struct sockaddr*)&addr, sizeof(addr));
}

int inetFd_and_bind(const char* ip, const int& port) {
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	setAddrAndBind(fd, ip, port);
	return fd;
}

int getFd_and_connect(inetAddr_type addr) {
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	while(connect(fd, (addr_type*)&addr, sizeof(addr)) == -1)
	{
	//	printf("connect error!\n");
	}
	return fd;
}

int getFd_and_connect(const char* ip, const int& port) {
	inetAddr_type addr;
	setAddr(addr, ip, port);
	return getFd_and_connect(addr);
}

class server
{
public:
	typedef struct sockaddr_in Addr;
	server(int domain, int type, int protocol) : listenFd(socket(domain, type, protocol))
	{ memset(&sv_addr, 0, sizeof(sv_addr));}
	
	void setSvAddr(const Addr& addr) {
		sv_addr = addr;
	}
	void setSvAddr(const char* ip, const int& port) {
		sv_addr.sin_family = AF_INET;
		sv_addr.sin_port = htons(port);
		inet_aton(ip, &sv_addr.sin_addr);
	}
	inline int __bind() {
		return bind(listenFd, (struct sockaddr*)(&sv_addr), sizeof(Addr));
	}
	int setSvAddr_and_bind(const char* ip, const int& port) {
		setSvAddr(ip, port);
		return __bind();
	} 
	inline int getListenFd() {
		return listenFd;
	}
	Addr getSvAddr() {
		return sv_addr;
	}
	int openListen() {
		return listen(listenFd, 5);
	}
	int acceptClent(struct sockaddr* claddr, socklen_t* len) {
		return accept(listenFd, claddr, len);
	}
protected:
	Addr sv_addr;
	int listenFd;
};

#endif
