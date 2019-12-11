#include<stdio.h>

/* call by value  */
int cbv(int x)
{
	x = x+1;
	return(0);
}

/* call by reference */
int cbr(int *x)
{
	*x = *x+1;
	return(0);
}

/* main */
int main(void)
{
	int x = 1;
	cbv(x);
	printf("cbv : %d\n", x);
	cbr(&x);
	printf("cbr : %d\n", x);

	return(0);
}
