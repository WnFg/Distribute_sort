#ifndef __IO_
#define __IO_

#include <string>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

std::string getStrFromInt(const int& x) {
	std::ostringstream out;
	out << x;
	return out.str();
}

int getIntFromStr(const std::string& s) {
	std::istringstream in(s);
	int ans;
	in >> ans;
	return ans;
}

template <typename T>
std::string getStrFromTmp(const T& x) {
	std::ostringstream out;
	out << x;
	return out.str();
}

int __rio_read(int fd, void* usrbuf, int len) {
		int t = 0, num;	
		while(len > 0 && (num = read(fd, (char*)usrbuf + t, len)) <= len) {
			if(num < 0) {
				printf("read Error!\n");
				exit(1);
			}else if(!num) {
				break;
			}else {
				len -= num;
				t += num;
			}
		}
		return t;
	}

#endif
