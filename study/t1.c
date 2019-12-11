#include<stdio.h>

int G = 1;

void main(int argc, char **argv)
{
	int i, j;
	for(i=0; i<10; i++)
	{
		for(j=0; j<=i; j++)
		{
			printf("*");
		}
		printf("\n");
	}

	printf("%d\n", G);
	t2();
}
