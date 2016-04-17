#include <iostream>
#include <stdlib.h>
#include <stdio.h>

int t = 50000;

int main()
{
	FILE* p = fopen("www.txt", "wb+");
	int cnt = 0;
	int tt;
	for(int i = t; i < 2*t; i++) {
		tt = rand() % 1000000;
		fwrite(&tt, 4, 1, p);
		cnt++;
	}
	for(int i = 0; i < t; i++) {
		tt = rand() % 1000000;
		fwrite(&tt, 4, 1, p);
		cnt++;
	}

	printf("%d\n", cnt);
	return 0;
}
