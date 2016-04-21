#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

int a[10000000];

int main()
{
	FILE *file = fopen("qwe2", "r+");
	if(file == NULL) {
		exit(-1);
	}
//	fseek(file, 0, SEEK_END);
//	int t = ftell(file);
//	printf("%d\n", t / 4);
	int t;
//	do{
	t = fread(a, 4, 10000000, file);
//	std::sort(a, a + 100000000);
	int i;
	for(i = 0; i < t-1; i++) {
		
	//	printf("%d\n", a[i]);
		if(a[i] > a[i + 1]){
			break;
		}
	}

	printf("%d gfgg \n", i);
//	}while(t);
	fclose(file);
	return 0;
}
