#include "rio_file.h"
#include "server.h"
#include "IO.h"
#include "command.h"

#include "rio_file.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <queue>
#include <pthread.h>
#include <string.h>

#define ip "127.0.0.1"
#define port 1111
#define slave_num 4

void* phase_3_2(void* argv);
void readyData(void* argv);
void* phase_sort(void* argv);
int min(int x, int y);
int waitSignal(int fd, int count = 1);

std::queue<int> sortedSlave;
int lived_slave = slave_num;
int slaveFd[slave_num];
inetAddr_type slave_addr[slave_num];

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

char* filePath[slave_num];

int listenFd;

struct cmd_1_2
{
	int sendSlave;
	int recvSlave;
	Cmd_1 cmd1;
	Cmd_2 cmd2;
};

typedef std::pair<int, Cmd_3> sortCmd_type;
pthread_t mark;

void solve() {
	
	while(lived_slave > 1) {
		pthread_mutex_lock(&mtx);
		
		while(sortedSlave.size() < 2)
			pthread_cond_wait(&cond, &mtx);
		
		while(sortedSlave.size() >= 2) {
			cmd_1_2* cmd = new cmd_1_2;
			memset(cmd, 0, sizeof(cmd_1_2));
			cmd->sendSlave = sortedSlave.front();
			sortedSlave.pop();
			cmd->recvSlave = sortedSlave.front();
			sortedSlave.pop();
			
			cmd->cmd2.order = 0;
			strcpy(cmd->cmd2.path, filePath[cmd->sendSlave]);
			
			cmd->cmd1.order = 1;
			cmd->cmd1.addr = slave_addr[cmd->sendSlave];
			strcpy(cmd->cmd1.path, filePath[cmd->recvSlave]);
			
			pthread_create(&mark, NULL, phase_3_2, cmd);
			--lived_slave;
		}
		pthread_mutex_unlock(&mtx);
	}
}

int main(int argc, char** argv)
{
	listenFd = inetFd_and_bind(ip, port);
	listen(listenFd, 5);

	socklen_t len = sizeof(inetAddr_type);
	
	for(int i = 0; i < slave_num; i++) {
		slaveFd[i] = accept(listenFd, NULL, NULL);
		setAddr(slave_addr[i], ip, 2222 + i);
	}
	readyData(argv);

	solve();
	
	while(1);
	return 0;
}

void* phase_sort(void* argv) {
	
	sortCmd_type cmd = *static_cast<sortCmd_type*>(argv);
	
	write(slaveFd[cmd.first], &cmd.second, sizeof(cmd.second));

	if(waitSignal(cmd.first) != 1)
		exit(-1);
	
	pthread_mutex_lock(&mtx);
	
	sortedSlave.push(cmd.first);
	
	pthread_mutex_unlock(&mtx);
	pthread_cond_signal(&cond);
	return NULL;
}

void* phase_3_2(void* argv) {
	
	cmd_1_2* cmd = (cmd_1_2*)argv;
	write(slaveFd[cmd->sendSlave], &cmd->cmd2, sizeof(cmd->cmd2));
	write(slaveFd[cmd->recvSlave], &cmd->cmd1, sizeof(cmd->cmd1));

	if(waitSignal(cmd->sendSlave) != 1 ||
	   waitSignal(cmd->recvSlave) != 1)
		exit(-1);
	
	Cmd_3 c3;
	c3.order = 2;
	strcpy(c3.path, cmd->cmd1.path);
	sortCmd_type ret(cmd->recvSlave, c3);
	phase_sort(&ret);
	return NULL;
}

void readyData(void* argv) {
	char** cmd = static_cast<char**>(argv);
	int fileSize = getFileSize(cmd[2]);
	int perFileSize = (fileSize + slave_num - 1) / slave_num;

	Cmd_1 c1;
	inetAddr_type addr;
	
	setAddr(addr, ip, port);
	
	c1.order = 1;
	c1.addr = addr;
	for(int i = 0; i < slave_num; i++) {
		std::string filename(cmd[1]);
		filename += '0' + i;
		filePath[i] = (char*)malloc(30);
		strcpy(filePath[i], filename.c_str());
		
		strcpy(c1.path, filePath[i]);
		write(slaveFd[i], &c1, sizeof(c1));
	}

	off_t pos = 0;
	int fileFd = open(cmd[2], O_RDONLY);

	for(int i = 0; i < slave_num; i++) {
		int fd = accept(listenFd, NULL, NULL);
		sendFileByNet(fileFd, fd, min(fileSize, perFileSize), &pos);
		fileSize -= perFileSize;
		close(fd);
	}
	close(fileFd);
	pthread_t tt;
	for(int i = 0; i < slave_num; i++) {
		if(waitSignal(i) != 1)
			exit(-1);
		
		sortCmd_type* c3 = new sortCmd_type;
		c3->first = i;
		c3->second.order = 2;
		strcpy(c3->second.path, filePath[i]);
		pthread_create(&tt, NULL, phase_sort, c3);
	}
}

int waitSignal(int fd, int count) {
	fd = slaveFd[fd];
	char ok;
	return recv(fd, &ok, count, MSG_WAITALL);
}

int min(int x, int y) {
	return x < y ? x : y;
}
