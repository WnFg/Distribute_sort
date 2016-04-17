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


/*class readObject
{
public:
	readObject() : buf(new char[512]), size(512) {}

	readObject(int size) : buf(new char[size]), size(size){}
	
	readObject(int a, int size) : buf(new char[size]), fd(a), size(size) {}

	~readObject() { delete[] buf; }

	inline int getFd() {
		return fd;
	}

	inline void setFd(const int& a) {
		fd = a;
	}
	
	
	int rio_read(int len) {
		return __rio_read(buf, len);	
	}
	
	bool read_line() {
		int i;
		for(i = 0; buf[i] != '\n' && i < size; i++) {
			if(__rio_read(buf + i, 1) == 0)
				break;
		}
		if(i == size)
			return false;

		buf[i] = '\0';
		return true;
	} 

protected:
	char* buf;
	int fd;
	int size;
};
*/
class writeObject
{
public:
	writeObject() : buf(new char[512]), size(512) {}

	writeObject(int size) : buf(new char[size]), size(size){}
	
	writeObject(int a, int size) : buf(new char[size]), fd(a), size(size) {}

	~writeObject() { delete[] buf; }

	inline int getFd() {
		return fd;
	}

	inline void setFd(const int& a) {
		fd = a;
	}
	
	int __rio_write(char* usrbuf, int len) {
		int t = 0, num;	
		if(len > size)
			len = size;
		while(len > 0 && (num = write(fd, usrbuf + t, len)) <= len) {
			if(num < 0) {
				printf("write Error!\n");
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

	int rio_write(int len) {
		return __rio_write(buf, len);	
	}
	
/*	bool read_line() {
		int i;
		for(i = 0; buf[i] != '\n' && i < size; i++) {
			if(__rio_write(buf + i, 1) == 0)
				break;
		}
		if(i == size)
			return false;

		buf[i] = '\0';
		return true;
	} 
*/
protected:
	char* buf;
	int fd;
	int size;
};



#endif
