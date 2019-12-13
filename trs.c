#include	"tr.h"	/* 유저가 정의한 헤더임을 의미 */

int listen_sfd;

void sig_act(int signo)
{
	printf("signo: %d\n", signo);
	close(listen_sfd);
	db_disconnect();
	exit(0);
}

void Usage()
{
	printf("Usage: trs portno\n");
}

int main(argc ,argv)
int argc;
char *argv[];
{
	
	/* port 번호 */
	int portno;
	int sfd;
	
	struct comm_hd_t hd;
	char buf[100000];
	int len;

	int rval;

	if(argc < 2)
	{
		Usage();
		return(0);
	}
	
	signal(SIGINT, sig_act); //Ctrl+C
	signal(SIGTERM, sig_act); //kill signal

	db_connect();	

	portno = atoi(argv[1]);	
	listen_sfd = tcp_server_init(portno);
	if(listen_sfd < 0)
	{
		printf("err: listen_std가 0보다 작습니다\n");
		return(0);
	}
	while(1)
	{
		sfd = tcp_accept_client(listen_sfd);
		if(sfd < 0)
		{
			printf("err: std가 0보다 작습니다\n");
			close(listen_sfd);
			return(0);
		}	
	//	printf("sfd: %d", sfd);
	//	printf("listen_sfd: %d", listen_sfd);

		printf("연결 요청이 완료 되었습니다.\n");
		
		if(fork() == 0)
		{
			close(listen_sfd);
			while(1)
			{
				printf("대기중입니다.\n");
				/* 소켓이 정상적으로 살아 있는지, 데이터가 정상적으로 오는 중인지 확인 */
				rval = wait_event3(sfd, 3);
			
				if(rval == 9999)
				{
					continue;
				}
				else if(rval < 0)
				{
					/* err */
					break;
				}
				
				rval = tcp_recv_size(sfd, &hd, sizeof(hd));
				if(rval < 0)
				{
					/* 종료(9)에 의해  클라이언트의 연결이 종료되는 시점  */
					break;
				}

				printf("tr: %d, cust_id: %d\n", hd.tr, hd.cust_id);
				
				memset(buf, 0x00, sizeof(buf));
				/* tcp통신을 하려면 길이를 알아야 하므로 return을 길이로 받는다  */
				if(hd.tr == 1)
				{
					len = tr1(&hd, buf);
				}
				else if(hd.tr == 2)
				{
					len = tr2(&hd, buf);
				}
				else if(hd.tr == 9)
				{
					sprintf(buf, "연결 종료\n");
					len = strlen(buf);
				}

				/* send  */
				if(len > 0)
				{
					tcp_select_send(sfd, buf, len);
				}
			}
			close(sfd);
			exit(0);
		}
		else
		{
			close(sfd);
		}
	}

/* signal을 작성했기 때문에 필요 없음  */
//	close(listen_sfd);
//	db_disconnect();

	return(0);
}
