/* 문자열 핸들링  */
#include "tr.h"

/* 문자열을 숫자로 변환 */
int str2int(char *s, int len)
{
	char temp[100+1];	//integer로 표현될 수 있는 크기(21억)
	
	memset(temp, 0x00, sizeof(temp));
	strncpy(temp, s, len);

	return (atoi(temp));
}

/* 숫자를 문자열로 변환  */
int int2str(char *s, int n, int len)
{
	char temp[100+1];

	memset(temp, 0x00, sizeof(temp));
	sprintf(temp,"%0*d", len, n);	//padding 작업
	strncpy(s, temp, len);

	return (0);
}

/*
 * 유효한 문자 추출 함수
 * 유효한 문자만 받을 버퍼 : d
 * 원본 문자열 소스 : s
 */
int trim(char *d, char *s, int len)
{
	int i;
	for(i=0; i<len; i++)
	{
		if(s[i] == 0x20)
		{
			break;	
		}
		
		d[i] = s[i];
	}
	d[i] = 0x00;

	return (i);
}

/* 
 * 컴파일할 때 무시해주는 매크로 함수 
 * 0 : 무시
 * 1 : 무시x
 */
#if(0)
int main(void)
{
	char *str = "000321        ";
	char buf[30];

	char *str2 = "123456789                ";	
	char d[40];	
	char d2[40];
	char d3[40];	

	memset(buf, 0x00, sizeof(buf));
	int2str(buf, str2int(str, 6), 6);

	printf("str : %s\n", str);
	printf("buf : %s\n", buf);

	trim(d, str2, 5);
	trim(d2, str2, 9);
	trim(d3, str2, 12);
	printf("d : (%s)\n", d);
	printf("d2 : (%s)\n", d2);
	printf("d3 : (%s)\n", d3);
}
#endif
