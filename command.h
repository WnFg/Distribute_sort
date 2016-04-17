#ifndef __COMMAND_
#define __COMMAND_

#include "server.h"

struct Cmd_1
{
	int order;
	inetAddr_type addr;
	char path[100];
};

struct Cmd_2
{
	int order;
	char path[100];
};

struct Cmd_3
{
	int order;
	char path[100];
};

struct fileStat_type
{
	int stat;
	int mid;
	int size;
};
#endif
