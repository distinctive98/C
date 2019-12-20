#include	"tr.h"

int main(int argc, char** argv)
{
	/* port 번호 */
	int portno = PORT;

	/* socket 변수  */
	int sfd;

	/* 구조체 변수  */	
	comm_hd_t *hd;
	
	char ibuf[100000];
	char obuf[100000];
	
	int rval;
	int len;

	int tr;
	int cust_id;
	char svc_id[64+1];	

	sfd = tcp_connect_server("localhost", portno); //hostname이 서버에 대한 ip일까?
	if(sfd < 0)	
	{
		printf("err: std가 0보다 작습니다\n");
		exit(0);
	}	

//	printf("sfd: %d", sfd);
	printf("연결 요청이 완료 되었습니다.\n");
	
	/* 통신헤더 채우기 */
	while(1)
	{
		/* memset은 포인터를 받는 것을 주의한다 */
		//memset(&hd, 0x00, sizeof(hd));

		printf("1:고객정보조회, 2:주문내역조회, 9:종료\n");
		printf("번호를 선택해주세요: ");
		scanf("%d",&tr);

		/* tr 체크 */
		switch(tr)
		{
			case 1:
				strcpy(svc_id, "tr1");
				break;
			case 2:
				strcpy(svc_id, "tr2");
				break;
			case 9:
				strcpy(svc_id, "exit");	
				close(sfd);
				exit(0);
				break;
			defualt:
				continue;
		}

		printf("고객번호를 입력하세요: ");
		scanf("%d",&cust_id);

		printf("svc_id: %s, cust_id: %d\n", svc_id, cust_id);

		/* 버퍼 초기화 */
		memset(ibuf, 0x00, sizeof(ibuf));
		memset(obuf, 0x00, sizeof(obuf));
		
		/* 헤더 초기화 */
		hd = (comm_hd_t*)&ibuf[0];
		memset(hd, 0x20, SZ_COMM_HD);
		/* port, session_id는 임시 값 대입(클라이언트를 지칭하는 유저 값/비동기 통신을 위한) */
		int2str(hd->port, 00001, sizeof(hd->port));
		int2str(hd->session_id, 950209, sizeof(hd->session_id));
		strncpy(hd->svc_id, svc_id, strlen(svc_id));

		/* input 데이터 헤더 뒤에 넣음 */
		//int2str(ibuf+SZ_COMM_HD, cust_id, 6);
		
		/* sprintf는 문자열 길이를 리턴함 */
		len = sprintf(ibuf+SZ_COMM_HD, "{\"cust_id\":%d}", cust_id);
		/* 헤더 사이즈 채우기 */
		int2str(hd->len, SZ_COMM_HD+len, sizeof(hd->len));
			
		/* 헤더 + input 데이터 서버 전송 */
		/* 사이즈만큼 보내야 하기 때문에 tcp_send_size */
		tcp_send_size(sfd, ibuf, SZ_COMM_HD+len);
	
		/* 서버로부터 헤더 수신 */
		rval = tcp_recv_size(sfd, obuf, SZ_COMM_HD);
		if(rval < 0)
		{
			break;
		}

		hd = (comm_hd_t*)obuf;
		len = str2int(hd->len, sizeof(hd->len)) - SZ_COMM_HD;

		/* 서버로 부터 데이터 수신 */
		rval = tcp_recv_size(sfd, obuf+SZ_COMM_HD, len);
		if(rval < 0)
		{
			break;
		}
	
		/* 헤더까지 포함해서 출력 */	
		printf("%s\n", obuf);

		/* 파일 시스템에 데이터 부분만 출력 */
		fprintf(stderr, "%s\n", obuf+SZ_COMM_HD);
		
	}
	
	close(sfd);
	return 0;
}
