#include "rio_file.h"
#include "server.h"
#include "IO.h"
#include "command.h"

#include "rio_file.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/socket.h>

#define ip "127.0.0.1"
#define svPort 1111

#define maxBufSize 550

char buf[maxBufSize];

/*void createFile(char* path)
{
	FILE* p = fopen(path, "a");
	close(p);
}*/

fileStat_type fileStat;

int main(int argc, char** argv)
{
	if(argc < 2)
		exit(-1);
	
	fileStat.stat = -1;
	fileStat.size = 0;
	int port = getIntFromStr(argv[1]);
	int listenFd = inetFd_and_bind(ip, port);
	listen(listenFd, 5);

	int masterFd = getFd_and_connect(ip, svPort);

	void* cmd = malloc(200);
	//int order;
	while(true) {
		while(recv(masterFd, cmd, 4, MSG_WAITALL) != 4);
		
		if(*(int*)cmd == 0) {				
			int len = sizeof(Cmd_2) - 4;
			
			while(recv(masterFd, (char*)cmd + 4, len, MSG_WAITALL) != len);
			
			Cmd_2* p = static_cast<Cmd_2*>(cmd);
			int destFd = accept(listenFd, NULL, NULL);
			if(destFd == -1) {
				printf("accept error!\n");
				exit(-1);
			}
			int file_fd = open(p->path, O_RDONLY | O_CREAT);
			sendFileByNet(file_fd, destFd);
			close(file_fd);
			close(destFd);
			write(masterFd, "1", 1);
		}else if(*(int*)cmd == 1) {
			printf("zxcweff\n");
			int len = sizeof(Cmd_1) - 4;
			
			while(recv(masterFd, (char*)cmd + 4, len, MSG_WAITALL) != len);

			Cmd_1* p = static_cast<Cmd_1*>(cmd);
			printf("%d	recv\n", p->addr.sin_addr.s_addr);
			printf("%d  recv\n", p->addr.sin_port);
			int sourceFd = getFd_and_connect(p->addr);
			
			int count;
			FILE* file = fopen(p->path, "a+");
			if(file == NULL) {
				printf("zxc\n");
				exit(-1);
			}
			int sum = 0;
			do{
				count = __rio_read(sourceFd, buf, maxBufSize);
				printf("asdqw: %d\n", count);
				fwrite(buf, 1, count, file);
				sum += count;
			}while(count >= maxBufSize);
			if(!fileStat.size)
				fileStat.stat = -1;
			else
				fileStat.stat = 1;
			fileStat.mid = fileStat.size;
			fileStat.size += sum;

			close(sourceFd);
			fclose(file);
			write(masterFd, "1", 1);
		}else {
			// 排序
			printf("qweqwe\n");
			int len = sizeof(Cmd_3) - 4;
			
			while(recv(masterFd, (char*)cmd + 4, len, MSG_WAITALL) != len);
			
			Cmd_3* p = static_cast<Cmd_3*>(cmd);
			
			int pid;
			printf("fileStat: %d\n", fileStat.stat);
			printf("%s\n", p->path);
			printf("%d %d\n", fileStat.mid, fileStat.size);
			if(fileStat.stat != 0)
				if((pid = fork()) != 0) {
					waitpid(pid, NULL, 0);
					fileStat.stat = 0;
					write(masterFd, "1", 1);
					continue;
				}
			
			if(fileStat.stat == -1)
				execl("sort", "sort", "0", p->path, NULL);
			else if(fileStat.stat == 1) 
				execl("sort", "sort", "1", p->path, 
					getStrFromInt(fileStat.mid / 4).c_str(),
					getStrFromInt(fileStat.size / 4).c_str(), NULL);
		}
	}
	return 0;
}
