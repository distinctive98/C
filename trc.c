#include	"tr.h"

void Usage()
{
	printf("Usage: trc portno\n");
}

int main(int argc, char** argv)
{
	
	/* port 번호 */
	int portno;
	/* socket 변수  */
	int sfd;
	/* 구조체 변수  */	
	struct comm_hd_t hd;

	if(argc < 2)
	{
		Usage();
		exit(0);
	}

	portno = atoi(argv[1]);
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
		memset(&hd, 0x00, sizeof(hd));
		printf("1:고객정보조회, 2:주문내역조회, 9:종료\n");
		printf("번호를 선택해주세요: ");
		scanf("%d",&hd.tr);
		printf("고객번호를 입력하세요: ");
		scanf("%d",&hd.cust_id);
		printf("tr: %d, cust_id: %d\n", hd.tr, hd.cust_id);
		tcp_select_send(sfd, &hd, sizeof(hd));
		
		if(hd.tr == 9)
		{
			break;
		}				
	}
	
	close(sfd);
	return 0;
}
