#include <stdio.h>

struct ff
{
	int a;
	char c;
};

ff w;



int main()
{
	w.a = 1;

	void* p = &w;
	printf("%d\n", *(int*)p);
	return 0;
}
