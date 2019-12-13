#ifndef	__TR_H__ /* 헤더 중복 선언 방지 */
#define	__TR_H__ /* 프로그램 끝날 때까지 유지 */

#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>

struct comm_hd_t {
    unsigned short len;
    int tr;
    int rslt;
    int cust_id;
    char data[1];
};

#define SZ_COMM_HD (sizeof(struct comm_hd_t)-1)

struct order_t {
        int     order_id;
        int     o_cust_id;
        char    order_dt[8+1];
        char    order_channel_type[1+1];
        char    order_status[1+1];
        int     order_rct_emp_id;
};

/* 트랜잭션에 대한 I/O 작성  */



#endif
