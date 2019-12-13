#include<stdio.h>
#include<string.h>

int fil(int l, int m, char* S, int f)
{
	int i;

	if(f==1)
	{
		for(i=0; i<=l; i++)
		{
			S[i] = '*';
		}
		S[i] = 0x00;
	}
	else
	{
		memset(S, 0x20, m);
		for(i=m-1; i>=m-l-1; i--)
		{
			S[i] = '*';
		}
		S[m] = 0x00;
	}

	return(0);			
}

void Usage()
{
	printf("Usage: t6 size\n");
}

int main(int argc, char** argv)
{	
	if(argc < 2)
	{
		Usage();
		return (0);
	}

	int l;	//line No
	int m;	//배열 크기
	char S[100][100+1];
	int f;	//1=정방향. 0=역방향
	int i;
	
	/* 배열 크기 초기화 */
	m = atoi(argv[1]);
	if(m > 100)
	{
		printf("err: 크기는 100을 넘을 수 없습니다.");
		return (0);
	}
	
	/* 정방향 */
	for(i=0; i<m; i++)
	{
		l = i;
		fil(i, m, S[i], 1);
		printf("%s\n", S[i]);
	}

	/* 역방향 */
	for(i=0; i<m; i++)
	{
		l = i;
		fil(i, m, S[i], 0);
		printf("%s\n", S[i]);
	}
	
	return (0);
}
