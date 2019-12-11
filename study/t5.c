#include<stdio.h>
#include<string.h>

int main(void)
{
	char S[10][10+1];
	int i, j;
	
	for(i=0; i<10; i++)
	{
		for(j=0; j<=i; j++)
		{
			S[i][j] = '*';
		}
	}

	for(i=0; i<10; i++)
	{
		for(j=0; j<=i; j++)
		{
			printf("%c", S[i][j]);
		}
		printf("\n");
	}

	/* 기존의 배열을 이용해서 역순 출력하기(문자)  */
	for(i=0; i<10; i++)
	{
		for(j=0; j<10; j++)
		{
			if(i+j>=9)
			{
				printf("%c", S[i][9-j]);
				//printf("A");		
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}

	/* memset으로 초기화하고 삽입하고 역순 출력하기(문자열)  */
	for(i=0; i<10; i++)
	{
		memset(S[i], 0x20, sizeof(S[i]));
		for(j=9; j>=9-i; j--)
		{
			S[i][j] = '*';
		}
		S[i][10] = 0x00;
	}

	for(i=0; i<10; i++)
	{
		printf("%s\n", S[i]);
	}

	return (0);
}
