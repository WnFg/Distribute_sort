#include <stdio.h>
#include <stdlib.h>

int a[1000000];

int main()
{
	FILE *file = fopen("qwe3", "r+");
	if(file == NULL) {
		exit(-1);
	}
//	fseek(file, 0, SEEK_END);
//	int t = ftell(file);
//	printf("%d\n", t / 4);
	int t = fread(a, 4, 1000000, file);
	
	int i;
	for(i = 0; i < t-1; i++) {
		
	//	printf("%d\n", a[i]);
		if(a[i] > a[i + 1]){
			break;
		}
	}
	printf("%d gfgg \n", i);
	fclose(file);
	return 0;
}
