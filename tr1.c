
/* Result Sets Interface */
#ifndef SQL_CRSR
#  define SQL_CRSR
  struct sql_cursor
  {
    unsigned int curocn;
    void *ptr1;
    void *ptr2;
    unsigned int magic;
  };
  typedef struct sql_cursor sql_cursor;
  typedef struct sql_cursor SQL_CURSOR;
#endif /* SQL_CRSR */

/* Thread Safety */
typedef void * sql_context;
typedef void * SQL_CONTEXT;

/* Object support */
struct sqltvn
{
  unsigned char *tvnvsn; 
  unsigned short tvnvsnl; 
  unsigned char *tvnnm;
  unsigned short tvnnml; 
  unsigned char *tvnsnm;
  unsigned short tvnsnml;
};
typedef struct sqltvn sqltvn;

struct sqladts
{
  unsigned int adtvsn; 
  unsigned short adtmode; 
  unsigned short adtnum;  
  sqltvn adttvn[1];       
};
typedef struct sqladts sqladts;

static struct sqladts sqladt = {
  1,1,0,
};

/* Binding to PL/SQL Records */
struct sqltdss
{
  unsigned int tdsvsn; 
  unsigned short tdsnum; 
  unsigned char *tdsval[1]; 
};
typedef struct sqltdss sqltdss;
static struct sqltdss sqltds =
{
  1,
  0,
};

/* File name & Package Name */
struct sqlcxp
{
  unsigned short fillen;
           char  filnam[7];
};
static struct sqlcxp sqlfpn =
{
    6,
    "tr1.pc"
};


static unsigned int sqlctx = 4803;


static struct sqlexd {
   unsigned long  sqlvsn;
   unsigned int   arrsiz;
   unsigned int   iters;
   unsigned int   offset;
   unsigned short selerr;
   unsigned short sqlety;
   unsigned int   occurs;
            short *cud;
   unsigned char  *sqlest;
            char  *stmt;
   sqladts *sqladtp;
   sqltdss *sqltdsp;
   unsigned char  **sqphsv;
   unsigned long  *sqphsl;
            int   *sqphss;
            short **sqpind;
            int   *sqpins;
   unsigned long  *sqparm;
   unsigned long  **sqparc;
   unsigned short  *sqpadto;
   unsigned short  *sqptdso;
   unsigned int   sqlcmax;
   unsigned int   sqlcmin;
   unsigned int   sqlcincr;
   unsigned int   sqlctimeout;
   unsigned int   sqlcnowait;
            int   sqfoff;
   unsigned int   sqcmod;
   unsigned int   sqfmod;
   unsigned int   sqlpfmem;
   unsigned char  *sqhstv[8];
   unsigned long  sqhstl[8];
            int   sqhsts[8];
            short *sqindv[8];
            int   sqinds[8];
   unsigned long  sqharm[8];
   unsigned long  *sqharc[8];
   unsigned short  sqadto[8];
   unsigned short  sqtdso[8];
} sqlstm = {13,8};

/* SQLLIB Prototypes */
extern sqlcxt (/*_ void **, unsigned int *,
                   struct sqlexd *, struct sqlcxp * _*/);
extern sqlcx2t(/*_ void **, unsigned int *,
                   struct sqlexd *, struct sqlcxp * _*/);
extern sqlbuft(/*_ void **, char * _*/);
extern sqlgs2t(/*_ void **, char * _*/);
extern sqlorat(/*_ void **, unsigned int *, void * _*/);

/* Forms Interface */
static int IAPSUCC = 0;
static int IAPFAIL = 1403;
static int IAPFTL  = 535;
extern void sqliem(/*_ unsigned char *, signed int * _*/);

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* CUD (Compilation Unit Data) Array */
static short sqlcud0[] =
{13,4130,873,0,0,
5,0,0,0,0,0,27,13,0,0,4,4,0,1,0,1,5,0,0,1,5,0,0,1,10,0,0,1,10,0,0,
36,0,0,2,0,0,32,29,0,0,0,0,0,1,0,
51,0,0,3,156,0,4,47,0,0,8,1,0,1,0,2,3,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,
0,0,2,5,0,0,1,3,0,0,
};


#include "tr.h"

/* EXEC SQL INCLUDE sqlca;
 */ 
/*
 * $Header: sqlca.h 24-apr-2003.12:50:58 mkandarp Exp $ sqlca.h 
 */

/* Copyright (c) 1985, 2003, Oracle Corporation.  All rights reserved.  */
 
/*
NAME
  SQLCA : SQL Communications Area.
FUNCTION
  Contains no code. Oracle fills in the SQLCA with status info
  during the execution of a SQL stmt.
NOTES
  **************************************************************
  ***                                                        ***
  *** This file is SOSD.  Porters must change the data types ***
  *** appropriately on their platform.  See notes/pcport.doc ***
  *** for more information.                                  ***
  ***                                                        ***
  **************************************************************

  If the symbol SQLCA_STORAGE_CLASS is defined, then the SQLCA
  will be defined to have this storage class. For example:
 
    #define SQLCA_STORAGE_CLASS extern
 
  will define the SQLCA as an extern.
 
  If the symbol SQLCA_INIT is defined, then the SQLCA will be
  statically initialized. Although this is not necessary in order
  to use the SQLCA, it is a good pgming practice not to have
  unitialized variables. However, some C compilers/OS's don't
  allow automatic variables to be init'd in this manner. Therefore,
  if you are INCLUDE'ing the SQLCA in a place where it would be
  an automatic AND your C compiler/OS doesn't allow this style
  of initialization, then SQLCA_INIT should be left undefined --
  all others can define SQLCA_INIT if they wish.

  If the symbol SQLCA_NONE is defined, then the SQLCA variable will
  not be defined at all.  The symbol SQLCA_NONE should not be defined
  in source modules that have embedded SQL.  However, source modules
  that have no embedded SQL, but need to manipulate a sqlca struct
  passed in as a parameter, can set the SQLCA_NONE symbol to avoid
  creation of an extraneous sqlca variable.
 
MODIFIED
    lvbcheng   07/31/98 -  long to int
    jbasu      12/12/94 -  Bug 217878: note this is an SOSD file
    losborne   08/11/92 -  No sqlca var if SQLCA_NONE macro set 
  Clare      12/06/84 - Ch SQLCA to not be an extern.
  Clare      10/21/85 - Add initialization.
  Bradbury   01/05/86 - Only initialize when SQLCA_INIT set
  Clare      06/12/86 - Add SQLCA_STORAGE_CLASS option.
*/
 
#ifndef SQLCA
#define SQLCA 1
 
struct   sqlca
         {
         /* ub1 */ char    sqlcaid[8];
         /* b4  */ int     sqlabc;
         /* b4  */ int     sqlcode;
         struct
           {
           /* ub2 */ unsigned short sqlerrml;
           /* ub1 */ char           sqlerrmc[70];
           } sqlerrm;
         /* ub1 */ char    sqlerrp[8];
         /* b4  */ int     sqlerrd[6];
         /* ub1 */ char    sqlwarn[8];
         /* ub1 */ char    sqlext[8];
         };

#ifndef SQLCA_NONE 
#ifdef   SQLCA_STORAGE_CLASS
SQLCA_STORAGE_CLASS struct sqlca sqlca
#else
         struct sqlca sqlca
#endif
 
#ifdef  SQLCA_INIT
         = {
         {'S', 'Q', 'L', 'C', 'A', ' ', ' ', ' '},
         sizeof(struct sqlca),
         0,
         { 0, {0}},
         {'N', 'O', 'T', ' ', 'S', 'E', 'T', ' '},
         {0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0}
         }
#endif
         ;
#endif
 
#endif
 
/* end SQLCA */
/* EXEC SQL INCLUDE oraca;
 */ 
/*
 * $Header: oraca.h 24-apr-2003.12:50:59 mkandarp Exp $ oraca.h 
 */

/* Copyright (c) 1985, 2003, Oracle Corporation.  All rights reserved.  */
 
/*
NAME
  ORACA : Oracle Communications Area.
FUNCTION
  Contains no code. Provides supplementary communications to/from
  Oracle (in addition to standard SQLCA).
NOTES
  **************************************************************
  ***                                                        ***
  *** This file is SOSD.  Porters must change the data types ***
  *** appropriately on their platform.  See notes/pcport.doc ***
  *** for more information.                                  ***
  ***                                                        ***
  **************************************************************

  oracchf : Check cursor cache consistency flag. If set AND oradbgf
            is set, then directs SQLLIB to perform cursor cache
            consistency checks before every cursor operation
            (OPEN, FETCH, SELECT, INSERT, etc.).
  oradbgf : Master DEBUG flag. Used to turn all DEBUG options
            on or off.
  orahchf : Check Heap consistency flag. If set AND oradbgf is set,
            then directs SQLLIB to perform heap consistency checks
            everytime memory is dynamically allocated/free'd via
            sqlalc/sqlfre/sqlrlc. MUST BE SET BEFORE 1ST CONNECT
            and once set cannot be cleared (subsequent requests
            to change it are ignored).
  orastxtf: Save SQL stmt text flag. If set, then directs SQLLIB
            to save the text of the current SQL stmt in orastxt
            (in VARCHAR format).
  orastxt : Saved len and text of current SQL stmt (in VARCHAR
            format).
  orasfnm : Saved len and text of filename containing current SQL
            stmt (in VARCHAR format).
  oraslnr : Saved line nr within orasfnm of current SQL stmt.
 
  Cursor cache statistics. Set after COMMIT or ROLLBACK. Each
  CONNECT'd DATABASE has its own set of statistics.
 
  orahoc  : Highest Max Open OraCursors requested. Highest value
            for MAXOPENCURSORS by any CONNECT to this DATABASE.
  oramoc  : Max Open OraCursors required. Specifies the max nr
            of OraCursors required to run this pgm. Can be higher
            than orahoc if working set (MAXOPENCURSORS) was set
            too low, thus forcing the PCC to expand the cache.
  oracoc  : Current nr of OraCursors used.
  oranor  : Nr of OraCursor cache reassignments. Can show the
            degree of "thrashing" in the cache. Optimally, this
            nr should be kept as low as possible (time vs space
            optimization).
  oranpr  : Nr of SQL stmt "parses".
  oranex  : Nr of SQL stmt "executes". Optimally, the relation-
            ship of oranex to oranpr should be kept as high as
            possible.
 
 
  If the symbol ORACA_NONE is defined, then there will be no ORACA
  *variable*, although there will still be a struct defined.  This
  macro should not normally be defined in application code.

  If the symbol ORACA_INIT is defined, then the ORACA will be
  statically initialized. Although this is not necessary in order
  to use the ORACA, it is a good pgming practice not to have
  unitialized variables. However, some C compilers/OS's don't
  allow automatic variables to be init'd in this manner. Therefore,
  if you are INCLUDE'ing the ORACA in a place where it would be
  an automatic AND your C compiler/OS doesn't allow this style
  of initialization, then ORACA_INIT should be left undefined --
  all others can define ORACA_INIT if they wish.
 
OWNER
  Clare
DATE
  10/19/85
MODIFIED
    apopat     05/08/02  - [2362423] MVS PE to make lines shorter than 79
    apopat     07/31/99 -  [707588] TAB to blanks for OCCS
    lvbcheng   10/27/98 -  change long to int for oraca
    pccint     10/03/96 -  Add IS_OSD for linting
    jbasu      12/12/94 -  Bug 217878: note this is an SOSD file
    losborne   09/04/92 -  Make oraca variable optional 
    Osborne    05/24/90 - Add ORACA_STORAGE_CLASS construct
  Clare      02/20/86 - PCC [10101l] Feature: Heap consistency check.
  Clare      03/04/86 - PCC [10101r] Port: ORACA init ifdef.
  Clare      03/12/86 - PCC [10101ab] Feature: ORACA cuc statistics.
*/
/* IS_OSD */ 
#ifndef  ORACA
#define  ORACA     1
 
struct   oraca
         {
    /* text */ char oracaid[8];      /* Reserved                            */
    /* ub4  */ int oracabc;          /* Reserved                            */
 
    /*       Flags which are setable by User. */
 
   /* ub4 */ int  oracchf;           /* <> 0 if "check cur cache consistncy"*/
   /* ub4 */ int  oradbgf;           /* <> 0 if "do DEBUG mode checking"    */
   /* ub4 */ int  orahchf;           /* <> 0 if "do Heap consistency check" */
   /* ub4 */ int  orastxtf;          /* SQL stmt text flag                  */
#define  ORASTFNON 0                 /* = don't save text of SQL stmt       */
#define  ORASTFERR 1                 /* = only save on SQLERROR             */
#define  ORASTFWRN 2                 /* = only save on SQLWARNING/SQLERROR  */
#define  ORASTFANY 3                 /* = always save                       */
         struct
           {
  /* ub2  */ unsigned short orastxtl;
  /* text */ char  orastxtc[70];
           } orastxt;                /* text of last SQL stmt               */
         struct
           {
  /* ub2  */   unsigned short orasfnml;
  /* text */   char       orasfnmc[70];
           } orasfnm;                /* name of file containing SQL stmt    */
  /* ub4 */ int   oraslnr;           /* line nr-within-file of SQL stmt     */

  /* ub4 */ int   orahoc;            /* highest max open OraCurs requested  */
  /* ub4 */ int   oramoc;            /* max open OraCursors required        */
  /* ub4 */ int   oracoc;            /* current OraCursors open             */
  /* ub4 */ int   oranor;            /* nr of OraCursor re-assignments      */
  /* ub4 */ int   oranpr;            /* nr of parses                        */
  /* ub4 */ int   oranex;            /* nr of executes                      */
         };

#ifndef ORACA_NONE

#ifdef ORACA_STORAGE_CLASS
ORACA_STORAGE_CLASS struct oraca oraca
#else
struct oraca oraca
#endif
#ifdef ORACA_INIT
         =
         {
         {'O','R','A','C','A',' ',' ',' '},
         sizeof(struct oraca),
         0,0,0,0,
         {0,{0}},
         {0,{0}},
         0,
         0,0,0,0,0,0
         }
#endif
         ;

#endif

#endif
/* end oraca.h */

/* EXEC ORACLE OPTION (ORACA=YES); */ 


char	*ora_userid = "bo";
char	*ora_passwd = "bo";

int
db_connect()
{
	/* EXEC SQL CONNECT :ora_userid IDENTIFIED BY :ora_passwd; */ 

{
 struct sqlexd sqlstm;
 sqlorat((void **)0, &sqlctx, &oraca);
 sqlstm.sqlvsn = 13;
 sqlstm.arrsiz = 4;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )10;
 sqlstm.offset = (unsigned int  )5;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)ora_userid;
 sqlstm.sqhstl[0] = (unsigned long )0;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)ora_passwd;
 sqlstm.sqhstl[1] = (unsigned long )0;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlstm.sqlcmax = (unsigned int )100;
 sqlstm.sqlcmin = (unsigned int )2;
 sqlstm.sqlcincr = (unsigned int )1;
 sqlstm.sqlctimeout = (unsigned int )0;
 sqlstm.sqlcnowait = (unsigned int )0;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	if (sqlca.sqlcode != 0) {
		fprintf(stdout, "CONNECT ERROR : %.*s\n",
			sqlca.sqlerrm.sqlerrml, sqlca.sqlerrm.sqlerrmc);

		exit(0);
	}
	
	fprintf(stdout, "DATABASE와 연결이 되었습니다.\n");

	return (0);
}

int
db_disconnect()
{
	/* EXEC SQL ROLLBACK WORK RELEASE; */ 

{
 struct sqlexd sqlstm;
 sqlorat((void **)0, &sqlctx, &oraca);
 sqlstm.sqlvsn = 13;
 sqlstm.arrsiz = 4;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )36;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	fprintf(stdout,"DATABASE와의 연결을 해제했습니다. \n");
	exit (0);
}

int tr1(_cust_id)
int _cust_id;
{
	/* customers 테이블 변수 */
	int	c_cust_id;
	char	cust_name[40+1];
	char	cust_gender_type[1+1];
	char	login_id[10+1];
	char	login_pswd[40+1];
	char	login_name[40+1];
	char	cust_grade[1+1];

	/* cust_id에 해당하는 하나의row 추출 */
	/* EXEC SQL
	SELECT	CUST_ID,
		CUST_NAME,
		CUST_GENDER_TYPE,
		LOGIN_ID,
		LOGIN_PSWD,
		LOGIN_NAME,
		CUST_GRADE
	INTO	:c_cust_id,
		:cust_name,
		:cust_gender_type,
		:login_id,
		:login_pswd,
		:login_name,
		:cust_grade
	FROM	CUSTOMERS
	WHERE	CUST_ID = :_cust_id; */ 

{
 struct sqlexd sqlstm;
 sqlorat((void **)0, &sqlctx, &oraca);
 sqlstm.sqlvsn = 13;
 sqlstm.arrsiz = 8;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select CUST_ID ,CUST_NAME ,CUST_GENDER_TYPE ,LOGIN_ID ,LOGI\
N_PSWD ,LOGIN_NAME ,CUST_GRADE into :b0,:b1,:b2,:b3,:b4,:b5,:b6  from CUSTOME\
RS where CUST_ID=:b7";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )51;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.sqlpfmem = (unsigned int  )0;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)4352;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (unsigned char  *)&c_cust_id;
 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         short *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned long )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (unsigned char  *)cust_name;
 sqlstm.sqhstl[1] = (unsigned long )41;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         short *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned long )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (unsigned char  *)cust_gender_type;
 sqlstm.sqhstl[2] = (unsigned long )2;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         short *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned long )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (unsigned char  *)login_id;
 sqlstm.sqhstl[3] = (unsigned long )11;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         short *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned long )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (unsigned char  *)login_pswd;
 sqlstm.sqhstl[4] = (unsigned long )41;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         short *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned long )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (unsigned char  *)login_name;
 sqlstm.sqhstl[5] = (unsigned long )41;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         short *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned long )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (unsigned char  *)cust_grade;
 sqlstm.sqhstl[6] = (unsigned long )2;
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         short *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned long )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqhstv[7] = (unsigned char  *)&_cust_id;
 sqlstm.sqhstl[7] = (unsigned long )sizeof(int);
 sqlstm.sqhsts[7] = (         int  )0;
 sqlstm.sqindv[7] = (         short *)0;
 sqlstm.sqinds[7] = (         int  )0;
 sqlstm.sqharm[7] = (unsigned long )0;
 sqlstm.sqadto[7] = (unsigned short )0;
 sqlstm.sqtdso[7] = (unsigned short )0;
 sqlstm.sqphsv = sqlstm.sqhstv;
 sqlstm.sqphsl = sqlstm.sqhstl;
 sqlstm.sqphss = sqlstm.sqhsts;
 sqlstm.sqpind = sqlstm.sqindv;
 sqlstm.sqpins = sqlstm.sqinds;
 sqlstm.sqparm = sqlstm.sqharm;
 sqlstm.sqparc = sqlstm.sqharc;
 sqlstm.sqpadto = sqlstm.sqadto;
 sqlstm.sqptdso = sqlstm.sqtdso;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



	if(sqlca.sqlcode != 0)
	{
		if(sqlca.sqlcode == 1403)
		{
			printf("%d 고객 데이터가 존재하지 않습니다.\n", _cust_id);
		}
		else
			printf("OPEN ERR : %d\n", sqlca.sqlcode);
		
		return (-1);
	}
	else
	{	
		printf("%d %s %s %s %s %s %s\n",
			c_cust_id, cust_name, cust_gender_type, login_id, login_pswd, login_name, cust_grade);	
	}

	return (0);
}


