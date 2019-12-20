#include	"tr.h"	/* 유저가 정의한 헤더임을 의미 */

int listen_sfd;

void sig_act(int signo)
{
	printf("signo: %d\n", signo);
	close(listen_sfd);
	db_disconnect();
	exit(0);
}

int main(argc ,argv)
int argc;
char *argv[];
{
	
	/* port 번호 */
	int portno = PORT;
	int sfd;
	
	comm_hd_t hd;
	comm_hd_t *ohd;
	char ibuf[1000];	//input
	char obuf[100000];	//output
	char emsg[1000];
	char svc_id[64+1];	//tr 받을 변수

	int len;
	int rval;
	
	signal(SIGINT, sig_act); //Ctrl+C(sig interrupt)
	signal(SIGTERM, sig_act); //kill signal

	db_connect();	

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

		printf("연결 요청이 완료 되었습니다.\n");
		
		if(fork() == 0)
		{
			close(listen_sfd);
			while(1)
			{
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
				
				/* 헤더를 받음  */	
				rval = tcp_recv_size(sfd, &hd, sizeof(hd));
				if(rval < 0)
				{
					/* 종료(9)에 의해  클라이언트의 연결이 종료되는 시점 -> 들어오는 헤더가 없기 때문에  */
					break;
				}
				
				memset(ibuf, 0x00, sizeof(ibuf));
				memset(obuf, 0x00, sizeof(obuf));

				/* 헤더를 제외한 입력 데이터 길이 */
				len = str2int(hd.len, sizeof(hd.len))-sizeof(hd);
				/* 입력 데이터를 ibuf에 저장  */
				rval = tcp_recv_size(sfd, ibuf, len);				
				if(rval < 0)
				{
					/* 데이터가 정상적으로 들어오지 않은 경우 */
					printf("잘못된 길이입니다.\n");
					break;
				}
				
				//printf("(%d %d) (%s %s)\n", len+SZ_COMM_HD, len, (char*)&hd, ibuf);
				
				/* 트림을 통해 유효 데이터(트랜잭션 네임) 추출 */
				trim(svc_id, hd.svc_id, sizeof(hd.svc_id));
				printf("tr: %s\n", svc_id);				

				/* tcp통신을 하려면 길이를 알아야 하므로 return을 길이로 받는다  */
				/* strcmp(s1, s2) : s1과 s2가 같으면 0을 리턴 */
				/* 헤더 뒤에 obuf를 붙여서 헤더+output을 클라이언트로 바로 보낼 수 있게 함 */
				if(strcmp(svc_id, "tr1") == 0)
				{
					len = tr1(ibuf, obuf+SZ_COMM_HD, emsg);
				}
				else if(strcmp(svc_id, "tr2") == 0)
				{
					len = tr2(ibuf, obuf+SZ_COMM_HD, emsg);
				}
				else if(strcmp(svc_id, "tr4") == 0)
				{
					len = tr4(ibuf, obuf+SZ_COMM_HD, emsg);
				}
				else if(strcmp(svc_id, "tr5") == 0)
				{
					len = tr5(ibuf, obuf+SZ_COMM_HD, emsg);
				}
				else if(strcmp(svc_id, "tr6") == 0)
				{
					len = tr6(ibuf, obuf+SZ_COMM_HD, emsg);
				}
				else if(strcmp(svc_id, "exit") == 0)
				{
					//sprintf(obuf, "연결 종료\n");
					//len = strlen(obuf);
				}

				/* send  */
				if(len > 0)
				{
					/* comm_hd_t 타입으로 캐스팅하여 헤더 공간을 컨트롤 */
					ohd = (comm_hd_t*)obuf;
					/* 바이너리 타입이므로 메모리 복사를 해야 함 */
					memcpy(ohd, &hd, SZ_COMM_HD);
					/* errno에 0을 넣음으로써 정상임을 알려줌 */
					int2str(ohd->errno, 0, sizeof(ohd->errno));
					/* 헤더 + output 길이를 저장 */
					int2str(ohd->len, len+SZ_COMM_HD, sizeof(ohd->len));
					printf("%s\n", obuf);
					tcp_send_size(sfd, obuf, len+SZ_COMM_HD);
				}
			}
			printf("연결이 종료되었습니다.\n");
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
