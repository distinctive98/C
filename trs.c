#include<stdio.h>
#include<stdlib.h>

struct comm_hd_t {
	unsigned short len;
	int tr;
	int rslt;
	int cust_id;
	char data[1];
};

#define SZ_COMM_HD (sizeof(struct comm_hd_t)-1)

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
	/* socket 변수  */
	int sfd;
	int listen_sfd;
	
	struct comm_hd_t hd;
	int rval;

	if(argc < 2)
	{
		Usage();
		return(0);
	}

	db_connect();	

	portno = atoi(argv[1]);	
	listen_sfd = tcp_server_init(portno);
	if(listen_sfd < 0)
	{
		printf("err: listen_std가 0보다 작습니다\n");
		return(0);
	}

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

	while(1)
	{
		printf("대기중입니다.\n");
		rval = wait_event3(sfd, 3);
	
		if(rval == 9999)
		{
			continue;
		}
		else if(rval < 0)
		{
			break;
		}
		
		rval = tcp_recv_size(sfd, &hd, sizeof(hd));
		if(rval < 0)
		{
			break;
		}

		printf("tr: %d, cust_id: %d\n", hd.tr, hd.cust_id);
		if(hd.tr == 1)
		{
			tr1(hd.cust_id);
		}
		else if(hd.tr == 2)
		{
			tr2(hd.cust_id);
		}
		else if(hd.tr == 9)
		{
			break;
		}
	}

	close(listen_sfd);
	close(sfd);
	db_disconnect();

	return(0);
}
