#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "w_sort.h"
#include "rio_file.h"
#include <unistd.h>

int main(int argc, char** argv)
{
	if(argc < 3) {
		printf("参数错误\n");
		exit(-1);
	}
	
	FILE* in = fopen(argv[2], "rb+");
	fseek(in, 0, SEEK_END);
	int size = ftell(in) / 4;
	fseek(in, 0, SEEK_SET);
	
	FILE* out, *recordIn;
	
	recordIn = in;

	std::string randomFile;
	
	if(!strcmp(argv[1], "0")) {
		if(argc == 3) {
			out = getRandomFile(randomFile, "wb+", argv[2]);
		}else {
			out = fopen(argv[3], "wb+");
		}
		if(size <= maxNum_Obj*2) {
			fread(C, 4, size, in);
			std::sort(C, C + size);
			fwrite(C, 4, size, out);
		}else {
			out_sort(in, out, size);
		}
	}else {
		out = getRandomFile(randomFile, "wb+", argv[2]);
		__sort(in, 0, getIntFromStr(argv[3]), getIntFromStr(argv[4]), out);
	}
	
	fclose(in);
	fclose(out);
	
	if(recordIn == in) {
		execlp("mv", "mv", randomFile.c_str(), argv[2], NULL);
	}else {
		execlp("rm", "rm", randomFile.c_str(), NULL);
	}

	return 0;
}
