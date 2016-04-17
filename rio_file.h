#ifndef __RIO_FILE_
#define __RIO_FILE_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <string>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <unistd.h>
#include "IO.h"

typedef int fileFd_type;
typedef int sockFd_type;

int getFileSize(char* path) {
	FILE* file = fopen(path, "a+");
	fseek(file, 0, SEEK_END);
	int tmp = ftell(file);
	fclose(file);
	return tmp;
}

void sendFileByNet(fileFd_type file_fd, sockFd_type sock_fd, int count = -1, off_t *offset = NULL) {
	if(count == -1) {
		count = lseek(file_fd, 0, SEEK_END); 
		if(count == -1) {
			printf("文件指针定位错误\n");
			exit(-1);
		}

		if(lseek(file_fd, 0, SEEK_SET) == -1) 
			exit(-1);
	}	
	
	off_t pos = 0;

	if(offset == NULL)
		offset = &pos;
	
	while(count > 0) {
		int ret = sendfile(sock_fd, file_fd, offset, count);
		if(ret == -1) 
			exit(-1);

		count -= ret;
	}
}

FILE* bufToFile(void* buf, char* path, int count) {
	FILE* file = fopen(path, "a+");
	fwrite(buf, 1, count, file);
	return file;
}

inline char getCh(int x) {
	if(x < 10)
		return '0' + x;

	if(x < 36)
		return 'a' + x - 10;

	return 'A' + x - 36;
}

std::string getRandomSign(int len, int seed = -1) {
	time_t t;
	if(seed == -1)
		time(&t);
	else
		t = seed;
	srand((unsigned) t);
	
	std::string ans;
	
	for(int i = 0; i < len; i++) {
		ans.push_back(getCh(rand() % 62));
	}
	return ans;
}

FILE* getRandomFile(std::string& fileName, const char* mode) {
	fileName = getRandomSign(20);
	FILE* p = fopen(fileName.c_str(), "a+");
	if(p == NULL)
		return NULL;

	fclose(p);
	p = fopen(fileName.c_str(), mode);
	return p;
}

FILE* getRandomFile(std::string& fileName, const char* mode, std::string s) {
	int seed = 0;
	for(int i = 0; i < s.size(); i++) {
		seed *= 3;
		seed += s[i];
	}

	fileName = getStrFromInt(seed);
	FILE* p = fopen(fileName.c_str(), "a+");
	
	if(p == NULL)
		return NULL;

	fclose(p);
	p = fopen(fileName.c_str(), mode);
	return p;
}

#endif
