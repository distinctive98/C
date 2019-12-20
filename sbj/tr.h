#ifndef	__TR_H__ /* 헤더 중복 선언 방지 */
#define	__TR_H__ /* 프로그램 끝날 때까지 유지 */

#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<signal.h>
#include	<unistd.h>

#include	"jansson.h"

#define PORT 9901

/* 헤더에 널문자를 명시할 필요는 없음 */
struct _comm_hd_t {
    char	len[6];
    char	port[5];
    char	session_id[64];
    char	svc_id[64];
    char	errno[4];
};
typedef struct _comm_hd_t comm_hd_t;

#define SZ_COMM_HD (sizeof(comm_hd_t))

struct _order_t {
    int     order_id;
    int     cust_id;
    char    order_dt[8+1];
    char    order_channel_type[1+1];
    char    order_status[1+1];
    int     order_rct_emp_id;
};
typedef struct _order_t order_t;

struct _cust_t {
	int		cust_id;
	char	cust_name[40+1];
	char	cust_gender_type[1+1];
	char	login_id[10+1];
	char	login_pswd[40+1];
	char	login_name[40+1];
	char	cust_grade[1+1];
};
typedef struct _cust_t cust_t;

/* tr2에 사용 */
/* tr6에 사용 */
struct _cust2_t {
	int		cust_id;
	char	cust_name[40+1];
	char	login_id[10+1];
	char	cust_grade[1+1];
};
typedef struct _cust2_t cust2_t;

/* tr5에 사용 */
struct _cust5_t {
	int		cust_id;
	char	cust_name[40+1];
	char 	login_id[10+1];
	int		order_cnt;
};
typedef struct _cust5_t cust5_t;

/* tr6에 사용 */
struct _cust6_t {
	int		order_id;
	char	order_dt[40+1];
	char	order_channel_type[10+1];
	char	order_status[10+1];
};
typedef struct _cust6_t cust6_t;

/* 트랜잭션에 대한 I/O 작성  */



#endif
