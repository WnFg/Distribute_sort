#include "server.h"
#include "command.h"
#include <string.h>
#include "rio_file.h"
#include <fcntl.h>

int main()
{
	int fd = inetFd_and_bind("127.0.0.1", 1111);
	listen(fd, 5);
	
	int cl = accept(fd, NULL, NULL);
	
	Cmd_1 cmd;
	cmd.order = 1;

	inetAddr_type addr;
	setAddr(addr, "127.0.0.1", 1111);
	cmd.addr = addr;

	strcpy(cmd.path, "asd.txt");

	write(cl, &cmd, sizeof(cmd));

	int destFd = accept(fd, NULL, NULL);
	
	int file_fd = open("www.txt", O_RDONLY | O_CREAT);
	sendFileByNet(file_fd, destFd);
	close(file_fd);
	close(destFd);
	close(fd);
	//while(1) {}
	return 0;
}
