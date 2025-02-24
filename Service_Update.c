
#include "define.h"

/*---------------------------------------------------------------------------*/
/* <COMMON INCLUDE SECTION>                                                  */
/*---------------------------------------------------------------------------*/
#include "service_common.h"

/*---------------------------------------------------------------------------*/
/* <DEFINE SECTION >                                                         */
/*---------------------------------------------------------------------------*/
#define INCR_SIZE   64*1024                         /* realloc시 size        */
#define SVR_NAME    "s_jgaa530_u3"                  /* SERVICE NAME          */

#define Arr     1000


static FBFR32  *transf;                         /* Tuxedo용  FML buffer      */

typedef struct
{
    VARCHAR v_proc_step           [2  +1]; 
    VARCHAR v_data_type           [2  +1]; 
    VARCHAR v_rcv_dt              [8  +1]; 
    VARCHAR v_file_cd             [5  +1]; 
    VARCHAR v_seq                 [7  +1]; 
    VARCHAR v_army_type           [2  +1]; 
    VARCHAR v_pbsv_type           [1  +1]; 
    VARCHAR v_file_seq            [2  +1]; 
    VARCHAR v_req_file_nm         [200+1]; 
    VARCHAR v_proc_file_nm        [200+1]; 
    VARCHAR v_jumin_no            [13 +1]; 
    VARCHAR v_nm                  [40 +1]; 
    VARCHAR v_elsmt_dt            [8  +1]; 
    VARCHAR v_dcmil_dt            [8  +1]; 
    VARCHAR v_conv_pbsv_dt        [8  +1]; 
    VARCHAR v_cen_type            [2  +1]; 
    VARCHAR v_brch_cd             [4  +1]; 
    VARCHAR v_jung_no             [11 +1]; 
    VARCHAR v_pbsv_pttn_chg_cd    [3  +1]; 
    VARCHAR v_dcmil_type_cd       [4  +1]; 
    VARCHAR v_army_no             [11 +1]; 
    VARCHAR v_bf_elsmt_dt         [8  +1]; 
    VARCHAR v_bf_dcmil_dt         [8  +1]; 
    VARCHAR v_elsmt_dt_max        [8  +1]; 
    VARCHAR v_dcmil_dt_min        [8  +1]; 
    VARCHAR v_bf_pbsv_type        [1  +1]; 
    double  d_exi_data_cnt               ; 
    VARCHAR v_cfr_org             [2  +1]; 
    VARCHAR v_err_contn           [200+1]; 
    VARCHAR v_proc_yn             [1  +1]; 
    VARCHAR v_rmk                 [200+1]; 
    VARCHAR v_proc_dt             [8  +1]; 
    VARCHAR v_proc_emp_no         [6  +1]; 
    VARCHAR v_cre_brch_cd         [4  +1]; 
    VARCHAR v_cre_pstn_type       [1  +1]; 
    VARCHAR v_cre_emp_no          [6  +1]; 
    VARCHAR v_cre_dttm            [14 +1]; 
    VARCHAR v_chg_dttm            [14 +1]; 
    VARCHAR v_chg_brch_cd         [4  +1]; 
    VARCHAR v_chg_pstn_type       [1  +1]; 
    VARCHAR v_chg_emp_no          [6  +1]; 
    VARCHAR v_pbsv_pttn           [2  +1]; 
    VARCHAR v_conv_army_type      [2  +1]; 
    VARCHAR v_rsn                 [100+1]; 
    VARCHAR v_err_cd              [2  +1]; 
    VARCHAR v_err_msg             [200+1]; 
} S_SELECT;

/*******************************************************************************

*******************************************************************************/
typedef struct
{
    VARCHAR  v_jumin_no         [ 13 + 1]; 
    VARCHAR  v_army_nm          [ 40 + 1]; 
    VARCHAR  v_entrarmy_dt_org  [  8 + 1]; 
    VARCHAR  v_entrarmy_dt      [  8 + 1]; 
    VARCHAR  c_army_no          [ 11 + 1]; 
    VARCHAR  v_lv_army_dt       [  8 + 1]; 
    VARCHAR  v_army_type        [  2 + 1]; 
    char     c_army_class_type  [  2 + 1]; 
    char     c_army_conn_type   [  1 + 1]; 
    VARCHAR  v_army_lv_type     [  4 + 1]; 
    VARCHAR  v_rcv_dt           [  8 + 1]; 
    VARCHAR  v_file_cd          [  5 + 1]; 
    VARCHAR  v_seq              [  7 + 1]; 
} S_JGAA16;
S_JGAA16 s_jgaa16;

static S_SELECT s_select;

static long gl_row;
static long gl_cnt;

static VARCHAR gv_brch_cd     [4+1]; /* */
static VARCHAR gv_pstn_type   [1+1]; /* */
static VARCHAR gv_user_id     [6+1]; /* */

char     c_army_conn_type   [  1 + 1]; 
char     c_si_do_nm         [  4 + 1]; 
char     c_err_type         [  1 + 1];
char     c_pay_sus_proc_type[  1 + 1]; 
char     c_err_type         [  1 + 1];
char     c_si_do_cd         [  2 + 1];

char gc_today                 [  8 + 1];
char cSDateTime             [ 30 + 1];

int  iErCnt = 0;              /* Error  Count          */
int  iInCnt = 0;              /* Insert Count          */
int  iInCnt_G1 = 0;           /* Insert_G1 Count       */
int  iInCnt_G2 = 0;           /* Insert_G2 Count       */
int  iInCnt_G5 = 0;           /* Insert_G5 Count       */
int  iInCnt_G6 = 0;           /* Insert_G6 Count       */
int  iInCnt_G78= 0;           /* Insert_G78 Count      */
int  iUpCnt = 0;              /* Update Count          */
int  iUpCnt_G1 = 0;           /* Update_G1  Count      */
int  iUpCnt_G2 = 0;           /* Update_G2  Count      */
int  iUpCnt_G5 = 0;           /* Update_G5  Count      */
int  iUpCnt_G6 = 0;           /* Update_G6  Count      */
int  iUpCnt_G78= 0;           /* Update_G78 Count      */

int  iSkCnt = 0;              /* Skip   Count          */
int  iSkCntGuide = 0;         /* Skip   Count          */
int  iGuideCnt17 = 0;         
int  iGuideCnt18 = 0;         
int  iGuideCnt41 = 0;         
int  iGuideCnt42 = 0;         
int  iGuideCnt43 = 0;         

static long gl_insert_cnt  = 0;
static long gl_chg_cnt     = 0;
static long gl_reject_cnt  = 0;

    char  *cSiDoNm[] = { "서울 11"
                        ,"부산 26"
                        ,"대구 27"
                        ,"인천 28"
                        ,"광주 29"
                        ,"대전 30"
                        ,"울산 31"
                        ,"세종 36"
                        ,"경기 41"
                        ,"강원 42"
                        ,"충북 43"
                        ,"충남 44"
                        ,"전북 45"
                        ,"전남 46"
                        ,"경북 47"
                        ,"경남 48"
                        ,"제주 49"
                        ,"None ZZ" };
                        
  __S_JGBA40__ s_jgba40; 
  __S_ZZTB00__ s_zztb00; 

static int f_jgaa530_u3_fml_get(FBFR32 *transf); /* FML -> 변수 */
static int f_jgaa530_u3_fml_put(FBFR32 *transf); /* FML <- 변수 */
static int f_jgaa530_u3_insert               (); 
/*---------------------------------------------------------------------------*/
/*         Service Update          S T A R T                                 */
/*---------------------------------------------------------------------------*/
void s_jgaa530_u3(TPSVCINFO *transb)
{
    int li_ret   = 0;
    int li_cnt   = 0;
    int li_row   = 0;
    int li_check = 0;

    SERVICE_START(SVR_NAME);

    transf = (FBFR32 *)transb->data;
    transf = (FBFR32 *)tprealloc((char *)transf, ( INCR_SIZE));
    if(transf == NULL )
    {
        SERVICE_END(SVR_NAME);
        f_MsgSend(transf, "A", SQL_CODE, 501,"TP Reallocate Error");
        tpreturn( TPFAIL, 0, (char *)transf, 0L,0 );
    }

    FMLLOOK(transf);
    
    memset(c_err_type, 0x00, sizeof(c_err_type));
    memset(c_pay_sus_proc_type, 0x00, sizeof(c_pay_sus_proc_type));
    strcpy(c_err_type, "0");
    strcpy(c_pay_sus_proc_type, "0");
    
    INITCHAR (cSDateTime );
    f_GetDateTime((char*)cSDateTime);
    strncpy(gc_today , cSDateTime, 8);

    /* FML -> 변수 */
    li_ret = f_jgaa530_u3_fml_get(transf);
    if(li_ret == NOT_OK)
    {
        SERVICE_END(SVR_NAME);
    	f_MsgSend(transf, "B",SQL_CODE, 13, s_select.v_err_msg.arr);
    	tpreturn(TPFAIL, 0, (char *)transf, 0L,0);
    }

    /* 대상자료 등록 */
    if(V2C_CMP(s_select.v_proc_step, "00") == 0)        /* 자료 재반영 */
    {
        li_ret = f_jgaa530_u3_tbjgaa23_insert();
    }
    else if(V2C_CMP(s_select.v_proc_step, "01") == 0)   /* 자료 재반영 */
    {
        
        /* 연계작업 상태 Update */
        EXEC SQL
        UPDATE  TBJGAA31
           SET
                PBSV_WRK_STAT    = '2'  /* 연계자료 작업상태    2 : 진행중 */
         WHERE
                RCV_DT           = :s_select.v_rcv_dt
           AND  FILE_CD          = :s_select.v_file_cd
         ;
        
        if(SQL_CODE != SQL_OK)
        {
            sprintf(s_select.v_err_msg.arr, " 인수파일내역 Update(진행중) 오류[%d][%s]", SQL_CODE, SQL_ERRTEXT);
            f_jgzz_c_msg_log(__FILE__, "%s", s_select.v_err_msg.arr);
            EXEC SQL ROLLBACK;
        
            li_ret = -1;
        }
        
        EXEC SQL COMMIT;
        
        if(V2C_CMP(s_select.v_pbsv_type, "a") == 0)          
        {
            li_ret = f_jgaa530_u3_tbjgaa16_insert();
        }
        else if((V2C_CMP(s_select.v_pbsv_type, "c") == 0)|| (V2C_CMP(s_select.v_pbsv_type, "b") == 0))     
        {
            li_ret = f_jgaa530_u3_tbjgaa16_update();
        }
        li_ret = f_jgaa530_u3_proc_01_update();         
    }
    else if(V2C_CMP(s_select.v_proc_step, "02") == 0)   
    {
        li_ret = f_jgaa530_u3_proc_02();
    }
    
    if(li_ret == NOT_OK)
    {
    	f_MsgSend(transf, "B",SQL_CODE, 13, s_select.v_err_msg.arr);
    	tpreturn(TPFAIL, 0, (char *)transf, 0L,0);
    	
    }

    /* FML <- 변수 */
    li_ret = f_jgaa530_u3_fml_put(transf);
    if(li_ret == NOT_OK)
    {
        SERVICE_END(SVR_NAME);
    	  f_MsgSend(transf, "B",SQL_CODE, 13, s_select.v_err_msg.arr);
    	  tpreturn(TPFAIL, 0, (char *)transf, 0L,0);
    }

    FMLLOOK(transf);

    SERVICE_END(SVR_NAME);

    tpreturn(TPSUCCESS, 0, (char *)transf, 0L, 0);
}
/*@****************************************************************************/
/*@****************************************************************************/
static int f_jgaa530_u3_fml_get(FBFR32 *transf) /* FML -> 변수 */
{
    int li_ret   = 0;
    int li_cnt   = 0;
    int li_row   = 0;
    int li_check = 0;

    VARCHAR lv_date_check [6+1]; /* */

    gl_row    = 0;
    gl_cnt    = 0;

    memset(&gv_brch_cd  , 0x00, sizeof(gv_brch_cd  )); /* */
    memset(&gv_pstn_type, 0x00, sizeof(gv_pstn_type)); /* */
    memset(&gv_user_id  , 0x00, sizeof(gv_user_id  )); /* */

    memset(s_select.v_err_msg.arr , 0x00, sizeof(s_select.v_err_msg.arr )); 
    
    GETVAR(STR01, 0, gv_brch_cd  ); 
    GETVAR(STR02, 0, gv_pstn_type); 
    GETVAR(STR03, 0, gv_user_id  ); 
    
    GETVAR(STR10 , 0,  s_select.v_proc_step           ); 
    GETVAR(STR11 , 0,  s_select.v_data_type           ); 
    GETVAR(STR12 , 0,  s_select.v_rcv_dt              ); 
    GETVAR(STR13 , 0,  s_select.v_file_cd             ); 
    GETVAR(STR14 , 0,  s_select.v_seq                 ); 
    GETVAR(STR15 , 0,  s_select.v_err_cd              ); 
    GETVAR(STR16 , 0,  s_select.v_army_type           ); 
    GETVAR(STR17 , 0,  s_select.v_pbsv_type           ); 
    GETVAR(STR18 , 0,  s_select.v_file_seq            ); 
    GETVAR(STR19 , 0,  s_select.v_req_file_nm         ); 
    GETVAR(STR20 , 0,  s_select.v_proc_file_nm        ); 
    GETVAR(STR21 , 0,  s_select.v_jumin_no            ); 
    GETVAR(STR22 , 0,  s_select.v_nm                  ); 
    GETVAR(STR23 , 0,  s_select.v_elsmt_dt            ); 
    GETVAR(STR24 , 0,  s_select.v_dcmil_dt            ); 
    GETVAR(STR25 , 0,  s_select.v_conv_pbsv_dt        ); 
    GETVAR(STR26 , 0,  s_select.v_cen_type            ); 
    GETVAR(STR27 , 0,  s_select.v_brch_cd             ); 
    GETVAR(STR28 , 0,  s_select.v_jung_no             ); 
    GETVAR(STR29 , 0,  s_select.v_pbsv_pttn_chg_cd    ); 
    GETVAR(STR30 , 0,  s_select.v_dcmil_type_cd       ); 
    GETVAR(STR31 , 0,  s_select.v_army_no             ); 
    GETVAR(STR32 , 0,  s_select.v_bf_elsmt_dt         ); 
    GETVAR(STR33 , 0,  s_select.v_bf_dcmil_dt         ); 
    GETVAR(STR34 , 0,  s_select.v_elsmt_dt_max        ); 
    GETVAR(STR35 , 0,  s_select.v_dcmil_dt_min        ); 
    GETVAR(STR36 , 0,  s_select.v_bf_pbsv_type        ); 
    GET   (DOUB10, 0, &s_select.d_exi_data_cnt        ); 
    GETVAR(STR37 , 0,  s_select.v_cfr_org             ); 
    GETVAR(STR38 , 0,  s_select.v_err_contn           ); 
    GETVAR(STR39 , 0,  s_select.v_proc_yn             ); 
    GETVAR(STR40 , 0,  s_select.v_rmk                 ); 
    GETVAR(STR41 , 0,  s_select.v_proc_dt             ); 
    GETVAR(STR42 , 0,  s_select.v_proc_emp_no         ); 
    GETVAR(STR43 , 0,  s_select.v_cre_brch_cd         ); 
    GETVAR(STR44 , 0,  s_select.v_cre_pstn_type       ); 
    GETVAR(STR45 , 0,  s_select.v_cre_emp_no          ); 
    GETVAR(STR46 , 0,  s_select.v_cre_dttm            ); 
    GETVAR(STR47 , 0,  s_select.v_chg_dttm            ); 
    GETVAR(STR48 , 0,  s_select.v_chg_brch_cd         ); 
    GETVAR(STR49 , 0,  s_select.v_chg_pstn_type       ); 
    GETVAR(STR50 , 0,  s_select.v_chg_emp_no          ); 
    GETVAR(STR51 , 0,  s_select.v_pbsv_pttn           ); 
    GETVAR(STR53 , 0,  s_select.v_conv_army_type      ); 
    GETVAR(STR54 , 0,  s_select.v_rsn                 ); 
        
    
    if(gv_brch_cd.len == 0)
    {
        sprintf(s_select.v_err_msg.arr, "코드가 없습니다.");
        return(NOT_OK);
    }

    if(gv_brch_cd.len != 4)
    {
        sprintf(s_select.v_err_msg.arr, "코드가 4자리가 아닙니다.");
        return(NOT_OK);
    }

    
    if(gv_pstn_type.len == 0)
    {
        sprintf(s_select.v_err_msg.arr, "구분이 없습니다.");
        return(NOT_OK);
    }

    if(gv_pstn_type.len != 1)
    {
        sprintf(s_select.v_err_msg.arr, "구분이 1자리가 아닙니다.");
        return(NOT_OK);
    }

    /* 사용자ID */
    if(gv_user_id.len == 0)
    {
        sprintf(s_select.v_err_msg.arr, "사용자ID가 없습니다.");
        return(NOT_OK);
    }

    return(OK);

}
/*@****************************************************************************/
/*@****************************************************************************/
static int f_jgaa530_u3_fml_put(FBFR32 *transf) /* FML -> 변수 */
{
    int li_ret   = 0;
    int li_cnt   = 0;
    int li_row   = 0;
    int li_check = 0;

    return(OK);
}

/*@****************************************************************************/
/*@****************************************************************************/
static int f_jgaa530_u3_tbjgaa23_insert  () /* 자료 재반영   */
{
    int li_ret   = 0;
    int li_cnt   = 0;
    int li_row   = 0;
    int li_check = 0;
    char lc_msg [100+1]; /* */
    
    gl_insert_cnt  = 0;
    gl_chg_cnt  = 0;
    memset(lc_msg, 0x00, sizeof(lc_msg));

    /* 처리여부가 N(미처리) 인것만 등록 */
    if(strcmp((char*)s_select.v_proc_yn.arr, "N") != 0)
    {
        sprintf(lc_msg, "처리여부 오류[%d][%s]\n");   
        f_jgzz_c_msg_log(__FILE__, " %s\n",s_select.v_proc_yn.arr);
        
        return(NOT_OK);
    }

    EXEC SQL
    INSERT INTO TBJGAA23
           (
            RCV_DT
           ,FILE_CD
           ,SEQ
           ,ARMY_TYPE
           ,ARMY_NO
           ,PBSV_PTTN
           ,JUMIN_NO
           ,NM
           ,ELSMT_DT
           ,DCMIL_DT
           ,CONV_PBSV_DT
           ,PBSV_PTTN_CHG_CD
           ,DCMIL_TYPE           
           ,FILE_NM
           ,CONV_ARMY_TYPE    
           ,RSN               
           ,PROC_TYPE
           ,PROC_DT
           ,RE_PROC_YN
           ,RE_PROC_DT
           ,PROC_BRCH_CD
           ,PROC_PSTN_TYPE
           ,CRE_BRCH_CD
           ,CRE_PSTN_TYPE
           ,CRE_EMP_NO
           ,CRE_DTTM
           ,CHG_DTTM
           ,CHG_BRCH_CD
           ,CHG_PSTN_TYPE
           ,CHG_EMP_NO
           )
    VALUES
           (
            :s_select.v_rcv_dt                       
           ,:s_select.v_file_cd                      
           ,LPAD(:s_select.v_seq, 7, '0')            
           ,:s_select.v_army_type                    
           ,:s_select.v_army_no                      
           ,:s_select.v_pbsv_pttn                    
           ,:s_select.v_jumin_no                     
           ,:s_select.v_nm                           
           ,:s_select.v_elsmt_dt                     
           ,:s_select.v_dcmil_dt                     
           ,:s_select.v_conv_pbsv_dt                 
           ,:s_select.v_pbsv_pttn_chg_cd             
           ,:s_select.v_dcmil_type_cd                
           ,:s_select.v_req_file_nm                  
           ,:s_select.v_conv_army_type               
           ,:s_select.v_rsn                          
           ,'1'
           ,'99991231'
           ,'N'
           ,'99991231'
           ,:gv_brch_cd
           ,:gv_pstn_type
           ,:gv_brch_cd
           ,:gv_pstn_type
           ,:gv_user_id
           ,SYSDATE
           ,SYSDATE
           ,:gv_brch_cd
           ,:gv_pstn_type
           ,:gv_user_id
           );
    
    if(SQL_CODE != SQL_OK)
    {
        sprintf(lc_msg, "자료 INSERT 오류[%d][%s]\n", SQL_CODE,SQL_ERRTEXT);   
        f_jgzz_c_msg_log(__FILE__, " %s\n",s_select.v_pbsv_pttn_chg_cd.arr);
        return(NOT_OK);
    }
    else
    {
        /* 등록갯수 증가 */
        gl_insert_cnt++;

        C2V_CPY(s_select.v_err_cd , "" ); /* */
        C2V_CPY(s_select.v_err_msg, "" );
        C2V_CPY(s_select.v_proc_yn, "Y");
    }

    /* 등록갯수가 있으면 처리갯수를 UPDATE 를 한다. */
    if(gl_insert_cnt > 0)
    {
        EXEC SQL
        UPDATE  TBJGAA31
           SET
                ORIGIN_REG_CNT   = ORIGIN_REG_CNT + :gl_insert_cnt                
               ,ORIGIN_EXCEP_CNT = REQ_CNT - (ORIGIN_REG_CNT + :gl_insert_cnt)    
               ,ORIGIN_PROC_DT   = TO_CHAR(SYSDATE, 'YYYYMMDD')  
               ,ORIGIN_WRK_STAT  = '3'                           
               ,PROC_STAT        = '20'                          
         WHERE
                RCV_DT           = :s_select.v_rcv_dt  
           AND  FILE_CD          = :s_select.v_file_cd 
        ;
    
        if(SQL_CODE != SQL_OK && SQL_CODE != SQL_NF)
        {
            sprintf(s_select.v_err_msg.arr, "인수파일내역 Update 오류[%d][%s]", SQL_CODE, SQL_ERRTEXT);
            f_jgzz_c_msg_log(__FILE__, "%s %s", s_select.v_rcv_dt.arr, s_select.v_file_cd.arr);
            return(NOT_OK);
        }
    }

    return(OK);
}

/*@****************************************************************************/
/*@****************************************************************************/
int f_jgaa530_u3_tbjgaa16_insert()
{
    char  c_lv_army_dt[ 8 +1 ];
    char  lc_gaibjatype [ 1 +1 ];
    char  cInpBuf  [200];
    
    gl_insert_cnt  = 0;
    gl_chg_cnt  = 0;
    
    int   iLoopCnt = 0;
    
    
    INITCHAR(c_lv_army_dt);
    INITCHAR(lc_gaibjatype);

printf("insert --- call [%s] [%d]\n", s_jgaa16.v_seq, gl_insert_cnt);



    if (V2C_CMP(s_select.v_army_type, "0A") == 0 ||
        V2C_CMP(s_select.v_army_type, "0a") == 0 )
    {
        C2C_CPY(c_army_conn_type, "1");
    }
    else if (V2C_CMP(s_select.v_army_type, "0B") == 0 ||
             V2C_CMP(s_select.v_army_type, "0b") == 0 )
    {
        C2C_CPY(c_army_conn_type, "2");
    }
    else if (V2C_CMP(s_select.v_army_type, "0C") == 0 ||
             V2C_CMP(s_select.v_army_type, "0c") == 0 )
    {
        C2C_CPY(c_army_conn_type, "3");
    }
    else if (V2C_CMP(s_select.v_army_type, "0D") == 0 ||
             V2C_CMP(s_select.v_army_type, "0d") == 0 )
    {
        C2C_CPY(c_army_conn_type, "9");
    }
    else if (V2C_CMP(s_select.v_army_type, "01" ) == 0 ||
             V2C_CMP(s_select.v_army_type, "02" ) == 0 ||
             V2C_CMP(s_select.v_army_type, "03" ) == 0 ||
             V2C_CMP(s_select.v_army_type, "04" ) == 0 ||
             V2C_CMP(s_select.v_army_type, "05" ) == 0 ||
             V2C_CMP(s_select.v_army_type, "06" ) == 0 ||
             V2C_CMP(s_select.v_army_type, "07" ) == 0 ||
             V2C_CMP(s_select.v_army_type, "08" ) == 0 ||
             V2C_CMP(s_select.v_army_type, "09" ) == 0 ||
             V2C_CMP(s_select.v_army_type, "00" ) == 0 )
    {
        strncpy(c_army_conn_type, s_select.v_army_type.arr+1, 1);
    }
    
    /***********************************************************************
     **********************************************************************/
    if(strncmp(c_army_conn_type , "07" , 2 ) == 0 )
    {
        strncpy(c_si_do_nm       ,  cInpBuf+71  ,  4);
        for (iLoopCnt = 0; iLoopCnt < 18; iLoopCnt++)
        {
            if (strncmp(c_si_do_nm , cSiDoNm[iLoopCnt], 4 ) == 0 )
            {
                strncpy(c_si_do_cd , cSiDoNm[iLoopCnt]+5, 2);
                break;
            }

            if(strncmp(cSiDoNm[iLoopCnt], "None", 4 ) == 0 )
            {
                break;
            }
        }
    }

    EXEC SQL
    INSERT    
    INTO   TBJGAA16
           (
              JUMIN_NO
            , ARMY_NM
            , ENTRARMY_DT
            , ARMY_NO
            , LV_ARMY_DT
            , RECV_DT
            , PROC_DT
            , ARMY_CLASS_TYPE
            , ARMY_TYPE
            , ARMY_CONN_TYPE
            , ERR_TYPE
            , PAY_SUS_PROC_TYPE
            , SI_DO_CD
            , CRE_DTTM
            , CRE_EMP_NO
            , CHG_DTTM
            , CHG_EMP_NO
            , ARMY_SPC_TYPE  /*20080303*/
            , ARMY_LV_TYPE   /*20080303*/
           )
     VALUES
           (
              :s_select.v_jumin_no
            , :s_select.v_nm
            , :s_select.v_elsmt_dt
            , TRIM(:s_select.v_army_no   )
            , NVL(TRIM(:s_select.v_dcmil_dt),'99991231')
            , :s_select.v_rcv_dt
            , '99991231'
            , NVL(TRIM(:s_select.v_pbsv_pttn), '00')
            , :s_select.v_army_type
            , :c_army_conn_type
            , :c_err_type
            , :c_pay_sus_proc_type
            , DECODE(:c_army_conn_type, '7' , :c_si_do_cd , '' )
            , SYSDATE
            , 'nhicjg'
            , SYSDATE
            , 'nhicjg'
            , TRIM(:s_select.v_pbsv_pttn_chg_cd )
            , TRIM(:s_select.v_dcmil_type_cd  )

           );
     
    if (SQL_CODE == 0)
    {
        /* 등록건수 증가 */
        gl_insert_cnt++;
    }
    else
    {
        if(SQL_CODE == SQL_DUP)
        {
            /* INSERT 오류가 나면 UPDATE 한다. */
            
                EXEC SQL
                UPDATE  TBJGAA16
                   SET
                        LV_ARMY_DT        = NVL(TRIM(:s_select.v_dcmil_dt),'99991231')                        
                      , RECV_DT           = :s_select.v_rcv_dt
                      , PROC_DT           = '99991231'                                                          
                      , ERR_TYPE          = '0'                                              
                      , PAY_SUS_PROC_TYPE = :c_pay_sus_proc_type     
                      , CHG_DTTM          = SYSDATE                                  
                      , CHG_EMP_NO        = 'nhicjg'                                                            
                      , ARMY_CLASS_TYPE   = NVL(TRIM(:s_select.v_pbsv_pttn), '00')   
                 WHERE 
                        JUMIN_NO    = :s_select.v_jumin_no
                   AND  ENTRARMY_DT = :s_select.v_elsmt_dt;

                if(SQL_CODE != SQL_OK)
                {
                    f_jgzz_c_msg_log(__FILE__, "Table 중복건 UPDATE :[%s] [%s] [%d]\n", s_select.v_jumin_no.arr, s_select.v_dcmil_dt.arr,SQL_CODE);
                    return(SQL_CODE);
                }
                else
                {
                    gl_chg_cnt++;
                     /* 제외내역을 삭제한다. */
    
                     EXEC SQL
                     DELETE TBJGAA33
                     WHERE JUMIN_NO    = :s_select.v_jumin_no
                     AND   ELSMT_DT    = :s_select.v_elsmt_dt
                     AND   RCV_DT      = :s_select.v_rcv_dt;
                     ;
    
                     if (sqlca.sqlcode != 0) 
                     {
                         f_jgzz_c_msg_log(__FILE__, "Table DELETE ERROR [%s] [%s] [%d]\n", s_select.v_jumin_no.arr, s_select.v_dcmil_dt.arr,SQL_CODE);
                     }
                    EXEC SQL COMMIT;
                     
                }
                 
        }
    }
     
    /* 원시내역 변경 */
    EXEC SQL
    UPDATE  TBJGAA23
       SET
            PROC_TYPE           = '2'
           ,PROC_DT             = TO_CHAR(SYSDATE, 'YYYYMMDD')
           ,CHG_DTTM            = SYSDATE
           ,CHG_EMP_NO          = 'nhicjg'

     WHERE
     
            RCV_DT           = :s_select.v_rcv_dt  
       AND  FILE_CD          = :s_select.v_file_cd 
       AND  JUMIN_NO         = :s_select.v_jumin_no
     ;

    if(SQL_CODE != SQL_OK)
    {
        sprintf(s_select.v_err_msg.arr, "원시자료 변경 등록 오류[%s][%s][%s]-[%d][%s]"
                         ,s_select.v_rcv_dt
                         ,s_select.v_file_cd
                         ,s_select.v_jumin_no
                         ,SQL_CODE
                         ,SQL_ERRTEXT);
        f_jgzz_c_msg_log(__FILE__, "TBJGAA23 :[%s] [%s] [%s]\n", s_select.v_jumin_no.arr, s_select.v_rcv_dt.arr,s_select.v_file_cd.arr);                  
        return(NOT_OK);
    }
    f_jgzz_c_msg_log(__FILE__, "TBJGAA23 :[%s] [%s] [%s]\n", s_select.v_jumin_no.arr, s_select.v_rcv_dt.arr,s_select.v_file_cd.arr);
    EXEC SQL COMMIT;
    
   
    
    
    return(SQL_CODE);

}

/*@****************************************************************************/
/*@****************************************************************************/
int f_jgaa530_u3_tbjgaa16_update()
{
    char cChk [1+1];
    int   row_cnt        = 0;
    int   li_sabung_cnt  = 0;
    int   li_jangkyo_cnt = 0;
    char  lc_max_entrarmy_dt  [ 8+1 ];
    char  lc_GaibjaType         [ 1+1 ];
    gl_insert_cnt  = 0;
    gl_chg_cnt  = 0;
    
    INITCHAR(lc_max_entrarmy_dt);
    INITCHAR(lc_GaibjaType);

/*
printf("f_jgaa400_u5_tbjgaa16_update -------- call [%s]  [%d]\n", s_jgaa16.c_seq, gl_chg_cnt);
*/
   
         f_jgaa530_u3_tbjgaa26_insert("" , s_select.v_dcmil_dt);

        EXEC SQL
        UPDATE TBJGAA16
           SET  RECV_DT         = :s_select.v_rcv_dt
               ,PROC_DT         = '99991231'
               ,LV_ARMY_DT      = nvl(ltrim(rtrim(:s_select.v_dcmil_dt)), '99991231')
               ,ARMY_TYPE       = '56'          
               ,pay_sus_proc_type = '0' 
               ,chg_dttm        =  sysdate
               ,chg_emp_no      =  'nhicjg'
         WHERE  JUMIN_NO        = :s_select.v_jumin_no
           AND  entrarmy_dt     = :s_select.v_bf_elsmt_dt
                ;
        if (SQL_CODE == 0)
        {
            /* 변경건수 증가 */
            gl_chg_cnt++;
        }
    
    return(SQL_CODE);
}

/*@****************************************************************************/
/*@****************************************************************************/
int f_jgaa530_u3_tbjgaa26_insert(char *pcGetCafEntrarmyDt , char *pcGetCafLvArmyDt)
{
    long  l_seq_no = 0;
    EXEC SQL
    SELECT /*+ index_desc (TBJGAA26 PK_TBJGAA26) */
           SEQ_NO
    INTO   :l_seq_no
    FROM   TBJGAA26
   WHERE  JUMIN_NO          = :s_select.v_jumin_no
     AND  ENTRARMY_DT       = :s_select.v_bf_elsmt_dt 
     AND  ROWNUM            = 1;

    l_seq_no++;

    EXEC SQL
    INSERT INTO TBJGAA26
    (
          JUMIN_NO
        , ARMY_NM
        , ENTRARMY_DT
        , SEQ_NO
        , ARMY_NO
        , LV_ARMY_DT
        , RECV_DT
        , PROC_DT
        , ARMY_CLASS_TYPE
        , ARMY_TYPE
        , ARMY_CONN_TYPE
        , ERR_TYPE
        , PAY_SUS_PROC_TYPE
        , CAF_ENTRARMY_DT
        , CAF_LV_ARMY_DT
        , SI_DO_CD
        , CHG_DTTM
        , CHG_EMP_NO
        , army_spc_type
        , army_lv_type
    )
    SELECT
          JUMIN_NO
        , ltrim(rtrim(:s_select.v_nm))
        , ENTRARMY_DT
        , :l_seq_no
        , ARMY_NO
        , LV_ARMY_DT
        , RECV_DT
        , PROC_DT
        , ARMY_CLASS_TYPE
        , ARMY_TYPE
        , ARMY_CONN_TYPE
        , ERR_TYPE
        , PAY_SUS_PROC_TYPE
        , :pcGetCafEntrarmyDt
        , :pcGetCafLvArmyDt
        , SI_DO_CD
        , SYSDATE
        , CHG_EMP_NO
        , army_spc_type   /*20080303*/
        , army_lv_type    /*20080303*/
    FROM  TBJGAA16
   WHERE  jumin_no          = :s_select.v_jumin_no
     AND  entrarmy_dt       = :s_select.v_bf_elsmt_dt
     ;
    if (SQL_CODE != 0)
    {
        f_jgzz_c_msg_log(__FILE__, " 변경이력(tbjgaa26) %s %s [%s] [$ld] : %s\n"
                                , s_select.v_jumin_no.arr
                                , s_select.v_bf_elsmt_dt.arr
                                , s_select.v_nm.arr
                                , l_seq_no
                                , SQL_ERRTEXT);
    }

    return(SQL_CODE);
}

/*@****************************************************************************/
/*@****************************************************************************/
static int f_jgaa530_u3_proc_01_update           ()
{
    int li_ret   = 0;
    int li_cnt   = 0;
    int li_row   = 0;
    int li_check = 0;

    typedef struct
    {
        VARCHAR v_proc_step         [100][2+1]; 
        VARCHAR v_data_type         [100][2+1]; 
        VARCHAR v_rcv_dt            [100][8+1]; 
        VARCHAR v_file_cd           [100][5+1]; 
        VARCHAR v_excep_proc_rsn_cd [100][2+1]; 
        double  d_excep_cnt         [100]     ; 
    } S_JGAA32;

    S_JGAA32 s_jgaa32;

    memset(&s_jgaa32, 0x00, sizeof(S_JGAA32));

    EXEC SQL
    UPDATE  TBJGAA31
       SET
            PBSV_REG_CNT        = PBSV_REG_CNT   + :gl_insert_cnt    
           ,PBSV_CHG_CNT        = PBSV_CHG_CNT   + :gl_chg_cnt       
           ,PBSV_EXCEP_CNT = PBSV_EXCEP_CNT - (:gl_insert_cnt + :gl_chg_cnt )           
           ,PBSV_PROC_DT        = TO_CHAR(SYSDATE, 'YYYYMMDD')      
           ,PBSV_RE_PROC_CNT    = 0                                 
           ,PBSV_WRK_STAT       = '3'                               
           ,PROC_STAT           = '30'                              
           ,CHG_DTTM            = SYSDATE
           ,CHG_BRCH_CD         = :gv_brch_cd
           ,CHG_PSTN_TYPE       = :gv_pstn_type
           ,CHG_EMP_NO          = 'nhicjg'
     WHERE
            RCV_DT           = :s_select.v_rcv_dt
       AND  FILE_CD          = :s_select.v_file_cd
     ;

    if(SQL_CODE != SQL_OK)
    {
        sprintf(s_select.v_err_msg.arr, "인수파일내역 Update 오류[%d][%s]", SQL_CODE, SQL_ERRTEXT);
        EXEC SQL ROLLBACK;

        return(NOT_OK);
    }

    EXEC SQL COMMIT;

    if(gl_reject_cnt > 0)
    {
        /* 통계생성 */
        EXEC SQL
        SELECT
                PROC_STEP        
               ,DATA_TYPE        
               ,RCV_DT           
               ,FILE_CD          
               ,ERR_CD           
               ,COUNT(*)
          INTO
                :s_jgaa32
          FROM
                TBJGAA33
         WHERE
                PROC_STEP = '01' 
           AND  DATA_TYPE = :s_select.v_data_type
           AND  RCV_DT    = :s_select.v_rcv_dt
           AND  FILE_CD   = :s_select.v_file_cd
         GROUP  BY
                PROC_STEP 
               ,DATA_TYPE 
               ,RCV_DT    
               ,FILE_CD   
               ,ERR_CD    
               ;

        if(SQL_CODE != SQL_OK && SQL_CODE != SQL_NF)
        {
            sprintf(s_select.v_err_msg.arr, "Table 통계 조회 오류[%d][%s]", SQL_CODE, SQL_ERRTEXT);
            EXEC SQL ROLLBACK;

            return(NOT_OK);
        }

        li_row = SQL_ROWNUM;

        for(li_cnt = 0; li_cnt < li_row; li_cnt++)
        {
            EXEC SQL
            INSERT INTO TBJGAA32
                   (
                    PROC_STEP
                   ,DATA_TYPE
                   ,RCV_DT
                   ,FILE_CD
                   ,EXCEP_PROC_STEP
                   ,EXCEP_PROC_RSN_CD
                   ,EXCEP_CNT
                   ,CRE_BRCH_CD
                   ,CRE_PSTN_TYPE
                   ,CRE_EMP_NO
                   ,CRE_DTTM
                   ,CHG_DTTM
                   ,CHG_BRCH_CD
                   ,CHG_PSTN_TYPE
                   ,CHG_EMP_NO
                   )
            VALUES
                   (
                    :s_jgaa32.v_proc_step           [li_cnt]
                   ,:s_jgaa32.v_data_type           [li_cnt]
                   ,:s_jgaa32.v_rcv_dt              [li_cnt]
                   ,:s_jgaa32.v_file_cd             [li_cnt]
                   ,'02' 
                   ,:s_jgaa32.v_excep_proc_rsn_cd   [li_cnt]
                   ,:s_jgaa32.d_excep_cnt           [li_cnt]
                   ,:gv_brch_cd
                   ,:gv_pstn_type
                   ,'nhicjg'
                   ,SYSDATE
                   ,SYSDATE
                   ,:gv_brch_cd
                   ,:gv_pstn_type
                   ,'nhicjg'
                   );

            if(SQL_CODE != SQL_OK && SQL_CODE != SQL_DUP)
            {
                sprintf(s_select.v_err_msg.arr, "통계 등록 오류[%d][%s]", SQL_CODE, SQL_ERRTEXT);
                EXEC SQL ROLLBACK;

                return(NOT_OK);
            }

            if(SQL_CODE == SQL_DUP)
            {
                EXEC SQL
                UPDATE  TBJGAA32
                   SET
                        EXCEP_CNT          = :s_jgaa32.d_excep_cnt           [li_cnt]
                       ,CHG_DTTM           = SYSDATE
                       ,CHG_BRCH_CD        = :gv_brch_cd
                       ,CHG_PSTN_TYPE      = :gv_pstn_type
                       ,CHG_EMP_NO         = 'nhicjg'
                 WHERE
                        PROC_STEP          = :s_jgaa32.v_proc_step           [li_cnt]
                   AND  DATA_TYPE          = :s_jgaa32.v_data_type           [li_cnt]
                   AND  RCV_DT             = :s_jgaa32.v_rcv_dt              [li_cnt]
                   AND  FILE_CD            = :s_jgaa32.v_file_cd             [li_cnt]
                   AND  EXCEP_PROC_STEP    = '02' 
                   AND  EXCEP_PROC_RSN_CD  = :s_jgaa32.v_excep_proc_rsn_cd   [li_cnt]
                ;

                if(SQL_CODE != SQL_OK )
                {
                    sprintf(s_select.v_err_msg.arr, "통계 변경 오류[%d][%s]", SQL_CODE, SQL_ERRTEXT);
                    EXEC SQL ROLLBACK;

                    return(NOT_OK);
                }
            }
        } /* for end */

        EXEC SQL COMMIT;
    }

    return(OK);
}


/*@****************************************************************************/
/*@****************************************************************************/
int f_jgaa530_u3_proc_02()
{
    int   i          = 0;
    int   iFetchCnt  = 0; 
    int   iLoopCnt   = 0; 
    int   li_ret     = 0;
    int   iDataCnt   = 0;
    int   li_mm_cnt     = 0;

    int   iCompTotMon = 0;
    int   iCurrTotMon = 0;
    char  lc_get_yyyymm [ 6 + 1];

    char  lc_proc_type[1+1];
    char  lc_err_type [1+1];
    char  lc_reg_type [1+1];
    char  lc_gaibja_type   [1+1];
    char  lc_diff_type [1+1];

    INITCHAR(lc_gaibja_type);

    /***************************************************************************
    **************************************************************************/
    iCurrTotMon = f_aToi_n(gc_today, 4) * 12 + f_aToi_n(gc_today+4, 2);

    INITCHAR(lc_proc_type);
    INITCHAR(lc_err_type );
    INITCHAR(lc_reg_type );

    li_ret = 0;

    EXEC SQL
    UPDATE  TBJGAA31
       SET
            PAY_SUS_WRK_STAT    = '2'                                  
     WHERE
            RCV_DT             = :s_select.v_rcv_dt
       AND  FILE_CD            = :s_select.v_file_cd
     ;

    if(SQL_CODE != SQL_OK)
    {
        sprintf(s_select.v_err_msg.arr, "인수파일내역 Update(진행) 오류[%d][%s]", SQL_CODE, SQL_ERRTEXT);
        EXEC SQL ROLLBACK;

        return(NOT_OK);
    }
    
    /***********************************************************************
     *********************************************************************/     
    memset(&s_zztb00, 0x00, sizeof(__S_ZZTB00__));
    strcpy(s_zztb00.c_user_id, gv_user_id.arr);
    if (f_jgzz_d_GetEmpInfo((__S_ZZTB00__ *) &s_zztb00 ) != 0)
    {

        f_jgzz_c_msg_log(__FILE__, "[%s] 사용자(TBZZTB00) Select Error !!! : %s\n"
                 , gv_user_id.arr
                 , SQL_ERRTEXT);
        strcpy(s_zztb00.c_brch_cd            , "9999" );
        strcpy(s_zztb00.c_pstn_type          , "0"    );
        strcpy(s_zztb00.c_dept_cd            , "000"  );
    }

    EXEC SQL COMMIT;
    
    EXEC SQL
    SELECT ARMY_CONN_TYPE
    INTO   :c_army_conn_type
    FROM   TBJGAA16
    WHERE  JUMIN_NO    = :s_select.v_jumin_no
    AND    RECV_DT = :s_select.v_rcv_dt
    ;
    
    if(SQL_CODE != SQL_OK)
    {
        sprintf(s_select.v_err_msg.arr, "인수파일내역 Update(진행) 오류[%d][%s]", SQL_CODE, SQL_ERRTEXT);
        f_jgzz_c_msg_log(__FILE__, "%s %s", s_select.v_jumin_no.arr, s_select.v_rcv_dt.arr);
        EXEC SQL ROLLBACK;

        return(NOT_OK);
    }

    /*memset(gc_reject_msg, 0x00, sizeof(gc_reject_msg));*/
    memset(s_select.v_err_msg.arr    , 0x00, sizeof(s_select.v_err_msg    ));

    lc_proc_type[0] = '0';
    lc_err_type [0] = '0';
    lc_reg_type [0] = ' ';
f_jgzz_c_msg_log(__FILE__, "kkk lc_reg_type [%s] --- 1\n", lc_reg_type);

    li_ret = f_jgaa530_u3_chk_tbjgba40();
    if (li_ret == 0)
    {
        if (strncmp((char*)s_select.v_dcmil_dt.arr, "99991231", 8) != 0)
        {     
            lc_proc_type[0] = '2';
            lc_reg_type [0] = 'U';
f_jgzz_c_msg_log(__FILE__, "kkk lc_reg_type [%s] --- 3\n", lc_reg_type);
            if (V2C_CMP(s_select.v_pbsv_pttn, "39") == 0 )
            {
                C2V_CPY  (s_jgba40.v_chwiduk_rsn_cd, "39" ); 
            }
            else if (strncmp(s_select.v_pbsv_pttn.arr, "40", 2) == 0 ||
            	       strncmp(s_select.v_pbsv_pttn.arr, "41", 2) == 0    )
            {
                C2V_CPY  (s_jgba40.v_chwiduk_rsn_cd, "40" ); 
            }
        }
        else if (strncmp((char*)s_select.v_dcmil_dt.arr, "99991231", 8) != 0 &&
                 strncmp((char*)s_select.v_dcmil_dt.arr, (char*)s_jgba40.v_pay_sus_to_dt.arr,  8) == 0 )
        {
            /*
            lc_err_type [0] = '1';  
            lc_proc_type[0] = '9';
            lc_reg_type [0] = 'U';*/
f_jgzz_c_msg_log(__FILE__, "kkk lc_reg_type [%s] --- 4\n", lc_reg_type);

            sprintf(s_select.v_err_cd.arr    , "02"); 
            
            f_jgzz_c_msg_log(__FILE__, "%13.13s %-20.20s %8.8s %8.8s %2.2s "
                                      "기처리 : 일자가 [%s]이 동일합니다.\n"
                                     , (char*) s_select.v_jumin_no   .arr
                                     , (char*) s_select.v_nm    .arr
                                     , (char*) s_select.v_bf_elsmt_dt.arr
                                     , (char*) s_select.v_dcmil_dt .arr
                                     , (char*) s_select.v_army_type  .arr
                                     , (char*) s_jgba40.v_pay_sus_to_dt.arr );
        }
        

        else if (li_ret == 1)
        {
           
            EXEC SQL
            SELECT  
                    PAY_SUS_DT
                  , PAY_SUS_TO_DT
              INTO
                   :s_jgba40.v_pay_sus_dt
                 , :s_jgba40.v_pay_sus_to_dt
              FROM
            TBJGBA40
               WHERE
            JUMIN_NO       = :s_select.v_jumin_no
                 AND  CHWIDUK_RSN_CD in ('15','22','31','32','33','34','35','36','38','39','40','55')   
                 AND  PAY_SUS_DT    != PAY_SUS_TO_DT
            ;
            
            /** 정지 자료 *************************************************/
            if(strncmp((char*)s_select.v_dcmil_dt.arr, "99991231", 8) == 0)
            {
                
                INITCHAR(lc_get_yyyymm);
                f_jgba_get_birth_yyyymm(
                                        (char*)s_select.v_jumin_no .arr
                                       , lc_get_yyyymm
                                       , "");
            
                iCompTotMon = f_aToi_n(lc_get_yyyymm, 4) * 12 + f_aToi_n(lc_get_yyyymm+4, 2);    

            }
            

            if (strncmp(s_select.v_pbsv_pttn.arr, "56", 2) == 0 ||
            	  strncmp(s_select.v_pbsv_pttn.arr, "66", 2) == 0 ||
                strncmp(s_select.v_pbsv_pttn.arr, "81", 2) == 0 ||
                strncmp((char*)s_select.v_army_type  .arr, "10", 2) == 0 )
            {
				f_jgzz_c_msg_log(__FILE__, "kkk lc_reg_type [%s] --- 11\n", lc_reg_type);
            }
            else if( strncmp(s_select.v_pbsv_pttn.arr, "39", 2) == 0 ||
                     strncmp(s_select.v_pbsv_pttn.arr, "40", 2) == 0 ||
                     strncmp(s_select.v_pbsv_pttn.arr, "41", 2) == 0  )
            {
                
                INITCHAR(lc_diff_type);
                EXEC SQL
                SELECT     '1'
                INTO      :lc_diff_type
                FROM
                            TBJGBA40
                WHERE
                            jumin_no       = :s_select.v_jumin_no
                AND         chwiduk_rsn_cd in ('15','22',
                                                '31','32','33','34','35',
                                                '36','38','39','40','55')   
                AND         pay_sus_dt    != pay_sus_to_dt
                AND         (
                                  (:s_select.v_bf_elsmt_dt >= pay_sus_dt AND :s_select.v_bf_elsmt_dt <= pay_sus_to_dt    )
                              OR (:s_select.v_dcmil_dt  >= pay_sus_dt AND :s_select.v_dcmil_dt  <= pay_sus_to_dt    )
                              OR (:s_select.v_bf_elsmt_dt <= pay_sus_dt AND :s_select.v_bf_elsmt_dt <= pay_sus_to_dt AND
                                  :s_select.v_dcmil_dt  >= pay_sus_dt AND :s_select.v_dcmil_dt  >= pay_sus_to_dt    )
                            )
                AND         rownum       = 1
                ;

                if(strncmp(s_select.v_pbsv_pttn.arr, "39", 2) == 0)
                {
                    INITCHAR(lc_diff_type);
                    EXEC SQL
                    SELECT      
                             '1'
                      INTO :lc_diff_type
                      FROM TBJGBA40
                     WHERE
                           JUMIN_NO       = :s_select.v_jumin_no
                       AND CHWIDUK_RSN_CD in('15','22','31','32','33','34','35','36','38','40','55')  
                       AND PAY_SUS_DT    != PAY_SUS_TO_DT
                       AND PAY_SUS_DT  BETWEEN TO_CHAR(TO_DATE(:s_select.v_bf_elsmt_dt ,'YYYYMMDD')-1,'YYYYMMDD')
                                           AND TO_CHAR(TO_DATE(:s_select.v_bf_elsmt_dt ,'YYYYMMDD')+1,'YYYYMMDD')
                       AND ROWNUM         = 1
                         ;

                    if (strncmp(lc_diff_type, "1", 1) == 0 )  
                    {
                        /*
                        lc_err_type [0] = '7';   
                        lc_proc_type[0] = '9';
                        lc_reg_type [0] = 'I'; */
f_jgzz_c_msg_log(__FILE__, "kkk lc_reg_type [%s] --- 13\n", lc_reg_type);
                    }
                    else
                    {
                        /*
                        lc_err_type [0] = '0';
                        lc_proc_type[0] = '2';
                        lc_reg_type [0] = 'I';
                        */
f_jgzz_c_msg_log(__FILE__, "kkk lc_reg_type [%s] --- 14\n", lc_reg_type);
                    }
                }
                else if((strncmp(s_select.v_pbsv_pttn.arr, "40", 2) == 0) ||
                	      (strncmp(s_select.v_pbsv_pttn.arr, "41", 2) == 0))
                {
                    INITCHAR(lc_diff_type);

                    EXEC SQL
                    SELECT 
                           '1'
                      INTO :lc_diff_type
                      FROM TBJGBA40
                     WHERE JUMIN_NO       = :s_select.v_jumin_no
                       AND  CHWIDUK_RSN_CD in('15','22','31','32','33','34','35','36','38','39','55')  
                       AND  PAY_SUS_DT    != PAY_SUS_TO_DT
                       AND  PAY_SUS_DT  BETWEEN TO_CHAR(TO_DATE(:s_select.v_bf_elsmt_dt ,'YYYYMMDD')-1,'YYYYMMDD')
                                            AND TO_CHAR(TO_DATE(:s_select.v_bf_elsmt_dt ,'YYYYMMDD')+1,'YYYYMMDD')
                       AND  ROWNUM         = 1
                       ;
                  
                    if (strncmp(lc_diff_type, "1", 1) == 0 )
                    {                       
						f_jgzz_c_msg_log(__FILE__, "kkk lc_reg_type [%s] --- 15\n", lc_reg_type);
                    }
                    else
                    {
						f_jgzz_c_msg_log(__FILE__, "kkk lc_reg_type [%s] --- 16\n", lc_reg_type);
                    }
                }
            }
        }
    } /* li_ret == 1 등록 */

    V2V_CPY  (s_jgba40.v_chg_emp_no , gv_user_id);
    V2V_CPY  (s_jgba40.v_jumin_no   , s_select.v_jumin_no          );
    C2V_CPY  (s_jgba40.v_in_type    , "2"); 

    li_ret = 0;
    
    if(V2C_CMP(s_select.v_pbsv_type, "a") == 0)   
    {
        if (strncmp(s_select.v_pbsv_pttn.arr, "61", 2) == 0 )
        {
            li_mm_cnt = 48;
        }
        
        else if (strncmp(s_select.v_pbsv_pttn.arr, "62", 2) == 0 )
        {
            li_mm_cnt = 26;
        }
        else if (strncmp(s_select.v_pbsv_pttn.arr, "1", 1) ||
                 strncmp(s_select.v_pbsv_pttn.arr, "4", 1) ||
                 strncmp(s_select.v_pbsv_pttn.arr, "6", 1) ||
                 strncmp(s_select.v_pbsv_pttn.arr, "9", 1) )
        {        
            if (strncmp((char*)s_select.v_bf_elsmt_dt.arr, "20110227", 8) >= 0)
            {
               li_mm_cnt = 21;
            }
            else
            {
               li_mm_cnt = 24;
            }
        }
        else if (strncmp(s_select.v_pbsv_pttn.arr, "2", 1) ||
                 strncmp(s_select.v_pbsv_pttn.arr, "5", 1) ||
                 strncmp(s_select.v_pbsv_pttn.arr, "7", 1) )
        {
            if (strncmp((char*)s_select.v_bf_elsmt_dt.arr, "20110103", 8) >= 0)
            {
               li_mm_cnt = 23;
            }
            else
            {
               li_mm_cnt = 26;
            }
        }
        else if (c_army_conn_type[0] == '3')
        {
            if (strncmp((char*)s_select.v_bf_elsmt_dt.arr, "20110101", 8) >= 0)
            {
               li_mm_cnt = 24;
            }
            else
            {
               li_mm_cnt = 27;
            }
        }

        if (strncmp((char*)s_select.v_dcmil_dt.arr, "99991231", 8) == 0)
        {
            if (strncmp((char*)s_select.v_army_type.arr, "10", 2) != 0) 
            {
                EXEC SQL
                SELECT  TO_CHAR(ADD_MONTHS(:s_select.v_bf_elsmt_dt, :li_mm_cnt), 'YYYYMMDD')
                  INTO  :s_jgba40.v_pay_sus_to_dt
                  FROM  DUAL;
            }
            else
            {
                EXEC SQL
                SELECT  /*+ index (TBJGAA16 IX_TBJGAA16_03 ) */
                        TO_CHAR(ADD_MONTHS(TO_CHAR(TO_DATE(entrarmy_dt, 'yyyymmdd')+1,'yyyymmdd'), :li_mm_cnt), 'YYYYMMDD')
                  INTO  :s_jgba40.v_pay_sus_to_dt
                  FROM  TBJGAA16
                 WHERE  RECV_DT       = :s_select.v_rcv_dt
                   AND  JUMIN_NO      = :s_select.v_jumin_no
                   AND  ARMY_TYPE    != '10'
                   AND  LV_ARMY_DT    = TO_CHAR(TO_DATE(:s_select.v_bf_elsmt_dt, 'YYYYMMDD')-1,'YYYYMMDD')
               ;
            }  

        }
        else
        {
            V2V_CPY(s_jgba40.v_pay_sus_to_dt ,s_select.v_dcmil_dt);
        }

        if (strncmp(s_select.v_pbsv_pttn.arr, "39", 2) == 0 )
        {
            C2V_CPY  (s_jgba40.v_chwiduk_rsn_cd, "39" ); 
        }
        else if ((strncmp(s_select.v_pbsv_pttn.arr, "40", 2) == 0 ) ||
        	       (strncmp(s_select.v_pbsv_pttn.arr, "41", 2) == 0 ))
        {
            C2V_CPY  (s_jgba40.v_chwiduk_rsn_cd, "40" ); 
        }
        else
        {
            C2V_CPY  (s_jgba40.v_chwiduk_rsn_cd, "22" ); 
        }

        V2V_CPY  (s_jgba40.v_pay_sus_dt, s_select.v_bf_elsmt_dt);
        C2V_CPY  (s_jgba40.v_pay_sus_rpt_dt        , gc_today);
        V2V_CPY  (s_jgba40.v_pay_sus_proc_emp_no   , gv_user_id);

        
        V2V_CPY(s_jgba40.v_sus_proc_brch_cd  , gv_brch_cd  );
        V2V_CPY(s_jgba40.v_sus_proc_pstn_type, gv_pstn_type);
        C2V_CPY(s_jgba40.v_sus_proc_dept_cd  , s_zztb00.c_dept_cd  );

        C2V_CPY(s_jgba40.v_pay_sus_in_dt     , gc_today );

        if (strncmp((char*)s_select.v_dcmil_dt.arr, "99991231", 8) != 0)
        {
            V2V_CPY(s_jgba40.v_to_proc_brch_cd  , gv_brch_cd  );
            V2V_CPY(s_jgba40.v_to_proc_pstn_type, gv_pstn_type);
            C2V_CPY(s_jgba40.v_to_proc_dept_cd  , s_zztb00.c_dept_cd  );

            V2V_CPY(s_jgba40.v_pay_sus_to_dt ,s_select.v_dcmil_dt);
            C2V_CPY(s_jgba40.v_pay_sus_to_rpt_dt, gc_today              );

            C2V_CPY(s_jgba40.v_pay_sus_cnclctr_in_dt, gc_today          );
        }
        else
        {
            C2V_CPY(s_jgba40.v_pay_sus_to_rpt_dt, "99991231"          );
            C2V_CPY(s_jgba40.v_pay_sus_cnclctr_in_dt, gc_today          );
        }
        V2V_CPY(s_jgba40.v_pay_sus_to_emp_no, gv_user_id           );

        li_ret = f_jgaa_d_tbjgba40_i((__S_JGBA40__ *)&s_jgba40);
        if (li_ret == 0)
        {
            gl_insert_cnt++;

            iInCnt++;

            if ((strncmp(s_select.v_pbsv_pttn.arr, "39", 2) == 0 ) || 
                (strncmp(s_select.v_pbsv_pttn.arr, "40", 2) == 0 ) || 
                (strncmp(s_select.v_pbsv_pttn.arr, "41", 2) == 0 ) )
            {
                 li_ret = f_jgaa530_u3_guide2_insert("I");

                 INITCHAR(lc_gaibja_type);

                 EXEC SQL
                 SELECT      
                         GAIBJA_TYPE
                   INTO
                         :lc_gaibja_type
                   FROM
                         TBJGBA20
                  WHERE
                         JUMIN_NO       = :s_jgba40.v_jumin_no
                    AND  :s_jgba40.v_pay_sus_to_dt BETWEEN CHWIDUK_DT AND SANGSIL_DT
                    AND  CHWIDUK_DT <> SANGSIL_DT
                    AND  CHWIDUK_DT <>:s_jgba40.v_pay_sus_to_dt
                    AND  ROWNUM = 1
                 ;
                if (strncmp(lc_gaibja_type, "1", 1) == 0 )
                {
                    iInCnt_G1++;
                }
                else if(strncmp(lc_gaibja_type, "2", 1) == 0 )
                {
                    iInCnt_G2++;
                }
                else if(strncmp(lc_gaibja_type, "5", 1) == 0 )
                {
                    iInCnt_G5++;
                }
                else if(strncmp(lc_gaibja_type, "6", 1) == 0 )
                {
                    iInCnt_G6++;
                }
                else
                {
                    iInCnt_G78++;
                }
            }
            else if (strncmp(s_select.v_pbsv_pttn.arr, "56", 2) != 0 )
                li_ret = f_jgaa530_u3_guide_insert("I");
            		if (li_ret >= 0) li_ret = f_jungsan_call();
        }
        else
        {
            iErCnt++;
            gl_reject_cnt++;
            f_jgzz_c_msg_log(__FILE__, "[%s|%s|%s] 급여정지(TBJGBA40) Insert Error !!! : [%s]\n"
                                     , (char*)s_select.v_jumin_no   .arr
                                     , (char*)s_select.v_nm    .arr
                                     , (char*)s_select.v_bf_elsmt_dt.arr
                                     , SQL_ERRTEXT);
        } /* {} Dream 25.01.17 Check 해볼것 */
        else if (lc_reg_type[0] == 'U')
        {
f_jgzz_c_msg_log(__FILE__, "kkk lc_reg_type [%s] --- 22\n", lc_reg_type);

             V2V_CPY  (s_jgba40.v_pay_sus_dt, s_select.v_bf_elsmt_dt);

 
            V2V_CPY(s_jgba40.v_to_proc_brch_cd  , gv_brch_cd  );
            V2V_CPY(s_jgba40.v_to_proc_pstn_type, gv_pstn_type);
            C2V_CPY(s_jgba40.v_to_proc_dept_cd  , s_zztb00.c_dept_cd  );

            V2V_CPY(s_jgba40.v_pay_sus_to_dt ,s_select.v_dcmil_dt);
            C2V_CPY(s_jgba40.v_pay_sus_to_rpt_dt, gc_today              );
            V2V_CPY(s_jgba40.v_pay_sus_to_emp_no, gv_user_id           );

            C2V_CPY(s_jgba40.v_pay_sus_cnclctr_in_dt, gc_today          );

            li_ret = f_jgaa530_u3_tbjgba40_u();
            if (li_ret == 0)
            {
                if ((strncmp(s_select.v_pbsv_pttn.arr, "39", 2) == 0 )||
                    (strncmp(s_select.v_pbsv_pttn.arr, "40", 2) == 0 )||
                	  (strncmp(s_select.v_pbsv_pttn.arr, "41", 2) == 0 ))
                {
                    INITCHAR(lc_gaibja_type);
                    EXEC SQL
                    SELECT  
                            GAIBJA_TYPE
                      INTO
                            :lc_gaibja_type
                      FROM  TBJGBA20
                     WHERE
                            JUMIN_NO       = :s_jgba40.v_jumin_no
                       AND  :s_jgba40.v_pay_sus_to_dt BETWEEN CHWIDUK_DT AND SANGSIL_DT
                       AND  CHWIDUK_DT <> SANGSIL_DT
                       AND  CHWIDUK_DT <>:s_jgba40.v_pay_sus_to_dt
                       AND  ROWNUM = 1
                     ;
                    if (strncmp(lc_gaibja_type, "1", 1) == 0 )
                    {
                        iUpCnt_G1++;
                    }
                    else if(strncmp(lc_gaibja_type, "2", 1) == 0 )
                    {
                        iUpCnt_G2++;
                    }
                    else if(strncmp(lc_gaibja_type, "5", 1) == 0 )
                    {
                        iUpCnt_G5++;
                    }
                    else if(strncmp(lc_gaibja_type, "6", 1) == 0 )
                    {
                        iUpCnt_G6++;
                    }
                    else
                    {
                        iUpCnt_G78++;
                    }
                }

                iUpCnt++;
                li_ret = f_jungsan_call();

            }
            else
            {
                iErCnt++;
                gl_reject_cnt++;
                f_jgzz_c_msg_log(__FILE__, "[%s|%s|%s] 정지(Table) Update Error !!! : %s\n"
                             , (char*)s_select.v_jumin_no   .arr
                             , (char*)s_select.v_nm    .arr
                             , (char*)s_select.v_bf_elsmt_dt.arr
                             , SQL_ERRTEXT);              
            }
        }

        if (li_ret == 0)
        {
            li_ret = f_jgaa530_u3_tbjgaa16_u(lc_proc_type, lc_err_type);
        }

        if (li_ret == 0)
        {
            if (lc_reg_type[0] == 'U')
            {
f_jgzz_c_msg_log(__FILE__, "kkk lc_reg_type [%s] --- 23\n", lc_reg_type);
                f_jgzz_c_msg_log(__FILE__, "%13.13s %-20.20s %8.8s ** %8.8s %8.8s %2.2s\n"
                                 , (char*) s_select.v_jumin_no   .arr
                                 , (char*) s_select.v_nm    .arr
                                 , (char*) s_select.v_dcmil_dt.arr
                                 , (char*) s_select.v_bf_elsmt_dt.arr
                                 , (char*) s_select.v_dcmil_dt .arr
                                 , (char*) s_select.v_army_type  .arr );
            }


            EXEC SQL COMMIT;
        }
        else
        {
            EXEC SQL ROLLBACK;

        }
        
    }

    return(li_ret);

}

/*@****************************************************************************/
/*@****************************************************************************/
int  f_jgaa530_u3_chk_tbjgba40()
{
    if (strncmp(s_select.v_pbsv_pttn.arr, "39", 2) == 0 )
    {
        EXEC SQL
        SELECT   
                  JUMIN_SEQ
                , CHWIDUK_RSN_CD
                , PAY_SUS_TO_DT
                , PAY_SUS_TO_RPT_DT
                , NVL(IN_TYPE, '2') 
                , PAY_SUS_SEQ_NO
                , NVL(PAY_SUS_CNCLCTR_IN_DT,'99991231')
                , CHG_EMP_NO
          INTO
                  :s_jgba40.v_jumin_seq
                , :s_jgba40.v_chwiduk_rsn_cd
                , :s_jgba40.v_pay_sus_to_dt
                , :s_jgba40.v_pay_sus_to_rpt_dt
                , :s_jgba40.v_in_type
                , :s_jgba40.l_pay_sus_seq_no
                , :s_jgba40.v_pay_sus_cnclctr_in_dt
                , :s_jgba40.v_chg_emp_no
          FROM
                 TBJGBA40
         WHERE
                 JUMIN_NO       = :s_select.v_jumin_no
           AND   CHWIDUK_RSN_CD ='39'
           AND   PAY_SUS_DT    != PAY_SUS_TO_DT
           AND   PAY_SUS_DT  BETWEEN TO_CHAR(TO_DATE(:s_select.v_bf_elsmt_dt ,'YYYYMMDD')-1,'YYYYMMDD')
                                 AND TO_CHAR(TO_DATE(:s_select.v_bf_elsmt_dt ,'YYYYMMDD')+1,'YYYYMMDD')
            ;
    }
    else if (strncmp(s_select.v_pbsv_pttn.arr, "40", 2) == 0  ||
    	       strncmp(s_select.v_pbsv_pttn.arr, "41", 2) == 0    	)
    {
        EXEC SQL
        SELECT  
                 JUMIN_SEQ
               , CHWIDUK_RSN_CD
               , PAY_SUS_TO_DT
               , PAY_SUS_TO_RPT_DT
               , NVL(IN_TYPE, '2') 
               , PAY_SUS_SEQ_NO
               , NVL(PAY_SUS_CNCLCTR_IN_DT,'99991231')
               , CHG_EMP_NO
         INTO
                 :s_jgba40.v_jumin_seq
               , :s_jgba40.v_chwiduk_rsn_cd
               , :s_jgba40.v_pay_sus_to_dt
               , :s_jgba40.v_pay_sus_to_rpt_dt
               , :s_jgba40.v_in_type
               , :s_jgba40.l_pay_sus_seq_no
               , :s_jgba40.v_pay_sus_cnclctr_in_dt
               , :s_jgba40.v_chg_emp_no
         FROM
                TBJGBA40
        WHERE
                JUMIN_NO       = :s_select.v_jumin_no
          AND   CHWIDUK_RSN_CD ='40'
          AND   PAY_SUS_DT    != PAY_SUS_TO_DT
          AND   PAY_SUS_DT  BETWEEN TO_CHAR(TO_DATE(:s_select.v_bf_elsmt_dt ,'YYYYMMDD')-1,'YYYYMMDD')
                                AND TO_CHAR(TO_DATE(:s_select.v_bf_elsmt_dt ,'YYYYMMDD')+1,'YYYYMMDD')
        ;
    }
    else
    {
        EXEC SQL
        SELECT  
                 JUMIN_SEQ
               , CHWIDUK_RSN_CD
               , PAY_SUS_TO_DT
               , PAY_SUS_TO_RPT_DT
               , NVL(IN_TYPE, '2') 
               , PAY_SUS_SEQ_NO
               , NVL(PAY_SUS_CNCLCTR_IN_DT,'99991231')  
               , CHG_EMP_NO                             
         INTO
                 :s_jgba40.v_jumin_seq
               , :s_jgba40.v_chwiduk_rsn_cd
               , :s_jgba40.v_pay_sus_to_dt
               , :s_jgba40.v_pay_sus_to_rpt_dt
               , :s_jgba40.v_in_type
               , :s_jgba40.l_pay_sus_seq_no
               , :s_jgba40.v_pay_sus_cnclctr_in_dt   
               , :s_jgba40.v_chg_emp_no              
         FROM
                TBJGBA40
        WHERE
               JUMIN_NO       = :s_select.v_jumin_no
          AND  CHWIDUK_RSN_CD in ('15','22','31','32','33','34','35','36','38','55') 
          AND  PAY_SUS_DT    != PAY_SUS_TO_DT
          AND  PAY_SUS_DT  BETWEEN TO_CHAR(TO_DATE(:s_select.v_bf_elsmt_dt ,'YYYYMMDD')-1,'YYYYMMDD')
                               AND TO_CHAR(TO_DATE(:s_select.v_bf_elsmt_dt ,'YYYYMMDD')+1,'YYYYMMDD')
        ;
    }
	
    if (SQL_CODE == 0)
    {
        return (0);
    }
  
    EXEC SQL
    SELECT      
            JUMIN_SEQ
      INTO
           :s_jgba40.v_jumin_seq
      FROM  TBJGBA20
     WHERE
            JUMIN_NO       = :s_select.v_jumin_no
       AND  (QLF_TYPE IS NOT NULL AND  QLF_TYPE  != '2') 
       AND  CHWIDUK_DT != SANGSIL_DT
       AND  ROWNUM = 1
     ;
	
    return (SQL_CODE == 0 ? 1 : -1);
}

/*@****************************************************************************/
/*@****************************************************************************/
int f_jgaa530_u3_guide2_insert(char *pcType)
{
    char  cBrchCd     [ 4+1];
    char  cPstnType   [ 1+1];
    char  cGaibjaType [ 1+1];
    char  cGuideType  [ 2+1];
    char  cNm         [40+1];
    char  cJungNo     [11+1];
    long  lJungSeqNo  = 0;
    long  l_seq_no = 0;
    long  lMonth   = 0;

    struct
    {
        char  cPostNo    [  4+1];
        char  cBasicAddr [100+1];
        char  cBlockAddr [100+1];
        char  cSpcAddr   [100+1];
    } s_addr;

    INITCHAR(cBrchCd    );
    INITCHAR(cPstnType  );
    INITCHAR(cGaibjaType);
    INITCHAR(cGuideType );
    INITCHAR(cNm        );
    INITSTRU(s_addr     );

    EXEC SQL
    SELECT  
            BRCH_CD
           ,PSTN_TYPE
           ,GAIBJA_TYPE
           ,NM
           ,JUNG_NO
           ,JUNG_SEQ_NO
      INTO
            :cBrchCd
           ,:cPstnType
           ,:cGaibjaType
           ,:cNm
           ,:cJungNo
           ,:lJungSeqNo
      FROM
            TBJGBA20
     WHERE
            JUMIN_NO       = :s_jgba40.v_jumin_no
       AND  SANGSIL_DT     = '99991231'
       AND  ROWNUM = 1
     ;

    if (SQL_CODE == 1403)
    {
        f_jgzz_c_msg_log(__FILE__, "[%s] 현 자격이 존재하지 않습니다. \n"
                         , (char*)s_jgba40.v_jumin_no .arr);
        return(SQL_CODE);
    }
    else if (SQL_CODE != SQL_OK)
    {
        f_jgzz_c_msg_log(__FILE__, "[%s] 현 자격 Select Error!!! : %s\n"
                         , (char*)s_jgba40.v_jumin_no .arr
                         , SQL_ERRTEXT);
        return(SQL_CODE);
    }

    if (cGaibjaType[0] > '6')
    {
        iSkCntGuide++;
        return(1);
    }

    if ((pcType[0] == 'I') || (pcType[0] == 'U'))
    {

        if (cGaibjaType[0] < '3')
        {
            sprintf(cGuideType, "41");
        }
        else if (cGaibjaType[0] == '5')
        {
            sprintf(cGuideType, "42");
        }
        else if (cGaibjaType[0] == '6')
        {
            sprintf(cGuideType, "43");
        }
        else
        {
            iSkCntGuide++;
            return(1);
        }
    }

    
    EXEC SQL
    SELECT 
            NVL(MAX(GUIDE_SEQ_NO), 0) + 1
      INTO
           :l_seq_no
      FROM
            TBJGEC10
     WHERE
           BRCH_CD    = :cBrchCd
           AND GUIDE_TYPE = :cGuideType
       AND GUIDE_DT   = TO_CHAR(SYSDATE,'YYYYMMDD')
    ;

    if (SQL_CODE != SQL_OK)
    {

        return(SQL_CODE);
    }

    
    EXEC SQL
    INSERT
      INTO  TBJGEC10
           (
              BRCH_CD
            , PSTN_TYPE
            , GUIDE_TYPE
            , GUIDE_DT
            , GUIDE_SEQ_NO
            , GUIDE_PTTN_TYPE           
            , GUIDE_JUNG_NO
            , GUIDE_JUNG_SEQ_NO
            , GUIDE_JUMIN_NO
            , GUIDE_NM
            , GUIDE_ISSUE_DT
            , GUIDE_ISSUE_TYPE
            , GUIDE_ISSUE_NO
            , MAGAM_JUNG_ISSUE_YN
            , GUIDE_EMP_NO
            , cre_dttm
            , cre_emp_no
            , chg_dttm
            , chg_emp_no
           )
    VALUES
          (
              :cBrchCd
            , :cPstnType
            , :cGuideType
            , TO_CHAR(SYSDATE,'YYYYMMDD')
            , :l_seq_no
            , '1'
            , :cJungNo
            , :lJungSeqNo
            , :s_jgba40.v_jumin_no
            , :cNm
            , '99991231'
            , '1'
            , ''
            , 'N'
            , :gv_user_id
            , sysdate
            , :gv_user_id
            , sysdate
            , :gv_user_id
          ) ;

    if (SQL_CODE != SQL_OK)
    {
         return(SQL_CODE);
    }

    if (strncmp(cGuideType, "41", 2) == 0)
    {
        iGuideCnt41++;      
    }
    else if (strncmp(cGuideType, "42", 2) == 0)
    {
        iGuideCnt42++;      
    }
    else if (strncmp(cGuideType, "43", 2) == 0)
    {
        iGuideCnt43++;      
    }

    return(SQL_CODE);
}

/*@****************************************************************************/
/*@****************************************************************************/
int f_jgaa530_u3_guide_insert(char *pcType)
{
    char  cBrchCd     [ 4+1];
    char  cPstnType   [ 1+1];
    char  cGaibjaType [ 1+1];
    char  cGuideType  [ 2+1];
    char  cNm         [40+1];
    char  cJungNo     [11+1];
    long  lJungSeqNo  = 0;
    long  l_seq_no = 0;
    long  lMonth   = 0;

    struct
    {
        char  cPostNo    [  4+1];
        char  cBasicAddr [100+1];
        char  cBlockAddr [100+1];
        char  cSpcAddr   [100+1];
    } s_addr;

    INITCHAR(cBrchCd    );
    INITCHAR(cPstnType  );
    INITCHAR(cGaibjaType);
    INITCHAR(cGuideType );
    INITCHAR(cNm        );
    INITSTRU(s_addr     );
    
    EXEC SQL
    SELECT  MONTHS_BETWEEN(TO_DATE(:s_jgba40.v_pay_sus_to_dt , 'YYYYMMDD')
                         , TO_DATE(:s_jgba40.v_pay_sus_dt    , 'YYYYMMDD'))
      INTO :lMonth
      FROM  DUAL
    ;

    if (( lMonth > 0 ) && (lMonth < 2 ))
    {
        iSkCntGuide++;
        return(1);
    }

    if (strncmp((char*)s_jgba40.v_pay_sus_to_rpt_dt.arr, "99991231", 8) != 0)
    {
        iSkCntGuide++;
        return(1);
    }


    EXEC SQL
    SELECT  
             BRCH_CD
           , PSTN_TYPE
           , GAIBJA_TYPE
           , NM
           , JUNG_NO
           , JUNG_SEQ_NO
      INTO
             :cBrchCd
           , :cPstnType
           , :cGaibjaType
           , :cNm
           , :cJungNo
           , :lJungSeqNo
      FROM    TBJGBA20
     WHERE
            JUMIN_NO       = :s_jgba40.v_jumin_no
       AND  SANGSIL_DT     = '99991231'
       AND  ROWNUM = 1
     ;

    if (SQL_CODE == 1403)
    {
        return(SQL_CODE);
    }
    else if (SQL_CODE != SQL_OK)
    {
        return(SQL_CODE);
    }

    if (cGaibjaType[0] > '3')
    {
        iSkCntGuide++;
        return(1);
    }

    if (pcType[0] == 'I')
    {
            sprintf(cGuideType, "17");
    }
    else
    {
            sprintf(cGuideType, "18");
    }

    
    EXEC SQL
    SELECT 
            NVL(MAX(GUIDE_SEQ_NO), 0) + 1
      INTO
           :l_seq_no
      FROM
            TBJGEC10
     WHERE
           BRCH_CD    = :cBrchCd
       AND PSTN_TYPE  = :cPstnType
       AND GUIDE_TYPE = :cGuideType
       AND GUIDE_DT   = TO_CHAR(SYSDATE,'YYYYMMDD')
    ;

    if (SQL_CODE != SQL_OK)
    {
           f_jgzz_c_msg_log(__FILE__, "**:%s 안내구분[%s] 안내서 일련번호 조회 오류!!! : %s\n"
                         , cBrchCd
                         , cGuideType
                         , SQL_ERRTEXT);
        return(SQL_CODE);
    }

    EXEC SQL
    INSERT
      INTO  TBJGEC10
           (
              BRCH_CD
            , PSTN_TYPE
            , GUIDE_TYPE
            , GUIDE_DT
            , GUIDE_SEQ_NO
            , GUIDE_PTTN_TYPE            
            , GUIDE_JUNG_NO
            , GUIDE_JUNG_SEQ_NO
            , GUIDE_JUMIN_NO
            , GUIDE_NM
            , GUIDE_ISSUE_DT
            , GUIDE_ISSUE_TYPE
            , GUIDE_ISSUE_NO
            , MAGAM_JUNG_ISSUE_YN
            , GUIDE_EMP_NO
            , cre_dttm
            , cre_emp_no
            , chg_dttm
            , chg_emp_no
           )
    VALUES
          (
              :cBrchCd
            , :cPstnType
            , :cGuideType
            , TO_CHAR(SYSDATE,'YYYYMMDD')
            , :l_seq_no
            , '1'
            , :cJungNo
            , :lJungSeqNo
            , :s_jgba40.v_jumin_no
            , :cNm
            , '99991231'
            , '1'
            , ''
            , 'N'
            , :gv_user_id
            , sysdate
            , :gv_user_id
            , sysdate
            , :gv_user_id
          ) ;

    if (SQL_CODE != SQL_OK)
    {
        return(SQL_CODE);
    }

    if (strncmp(cGuideType, "17", 2) == 0)
    {
        iGuideCnt17++;        
    }
    else
    {
        iGuideCnt18++;        
    }

    return(SQL_CODE);
}

/*@****************************************************************************/
/*@****************************************************************************/
int f_jungsan_call()
{
    int   i          = 0;
    int   iFetchCnt  = 0; 
    int   iNoMoreSw  = 1; 
    int   iLoopCnt   = 0; 
    int   intRet     = 0;
    int   iJikCnt    = 0;
    char  cGetErrMsg[200];

    struct
    {
        VARCHAR v_brch_cd          [Arr][ 4+1]; 
        VARCHAR v_gaibja_type      [Arr][ 1+1]; 
        VARCHAR v_chwiduk_seq_no   [Arr][ 3+1]; 
        VARCHAR v_jung_no          [Arr][11+1]; 
        long    l_jung_seq_no      [Arr]      ; 
    } s_jgba20;

    
    struct
    {
        varchar v_jumin_no                  [13+1];
        varchar v_jumin_seq                 [ 2+1];
        long    l_pay_sus_seq_no                  ;
        varchar v_chwiduk_rsn_cd            [ 2+1];
        varchar v_forn_hjob_type            [ 1+1];
        varchar v_pay_sus_dt                [ 8+1];
        varchar v_pay_sus_to_dt             [ 8+1];
        varchar v_work_gubn                 [ 1+1];
        varchar v_ex_pay_sus_dt             [ 8+1];
        varchar v_ex_pay_sus_to_dt          [ 8+1];
        varchar v_proc_brch_cd              [ 4+1];
        varchar v_proc_pstn_type            [ 1+1];
        varchar v_proc_dept_cd              [ 3+1];
        varchar v_proc_emp_no               [ 6+1];
    } s_bmac100;


    __S_BHCA01__  s_bhca01;
    memset(&s_bhca01   , 0x00, sizeof(__S_BHCA01__));
    memset(&s_bmac100  , 0x00, sizeof(s_bmac100));

        EXEC SQL DECLARE cur_f_jungsan_call CURSOR FOR
         SELECT  
                     brch_cd
                   , gaibja_type
                   , chwiduk_seq_no
                   , jung_no
                   , jung_seq_no
         FROM        TBJGBA20
         WHERE
                     jumin_no       = :s_jgba40.v_jumin_no

         AND      :s_jgba40.v_pay_sus_dt <= sangsil_dt
         AND       chwiduk_dt    != sangsil_dt

         ;
    /***************************************************************************
     Cursor Open
     **************************************************************************/
    EXEC SQL OPEN cur_f_jungsan_call;
    if (sqlca.sqlcode != 0)
    {
    	  f_jgzz_c_msg_log(__FILE__, "Cursor Open Error : %s\n"
		                 , sqlca.sqlerrm.sqlerrmc);
        return(sqlca.sqlcode);
    }

    while(iNoMoreSw)
    {
        INITSTRU(s_jgba20);

        EXEC SQL FETCH cur_f_jungsan_call
        		 INTO
                             :s_jgba20.v_brch_cd         
                           , :s_jgba20.v_gaibja_type
                           , :s_jgba20.v_chwiduk_seq_no  
                           , :s_jgba20.v_jung_no         
                           , :s_jgba20.l_jung_seq_no     
                 ;
        if (sqlca.sqlcode == 1403)
        {
        	  intRet = 0;
            iNoMoreSw = 0;
            if (sqlca.sqlerrd[2] == iFetchCnt) break;
        }
        else if (sqlca.sqlcode != 0)
        {
        	  intRet = sqlca.sqlcode;
        	  f_jgzz_c_msg_log(__FILE__, "cur_f_jungsan_call Cursor Fetch Error : %s\n"
			                 , sqlca.sqlerrm.sqlerrmc);
			      break;
        }

        iLoopCnt  = sqlca.sqlerrd[2] - iFetchCnt;
        iFetchCnt = sqlca.sqlerrd[2];

        for (i = 0; i < iLoopCnt; i++)
        {     
            if (s_jgba20.v_gaibja_type[i].arr[0] < '3')
            {
                memset(&s_bhca01   , 0x00, sizeof(__S_BHCA01__));

                V2V_CPY  (s_bhca01.v_jung_no            , s_jgba20.v_jung_no[i]       ); 
                C2V_CPY  (s_bhca01.v_ocur_dt            , gc_today                      ); 
                C2V_CPY  (s_bhca01.v_corr_ocur_rsn_cd   , "14"                        ); 
                C2V_CPY  (s_bhca01.v_qlf_bugwa_proc_dt  , gc_today                      ); 
                V2V_CPY  (s_bhca01.v_qlf_bugwa_rpt_dt   , s_select.v_rcv_dt  ); 

                V2V_CPY  (s_bhca01.v_jumin_no           , s_jgba40.v_jumin_no         ); 
                V2V_CPY  (s_bhca01.v_jumin_seq          , s_jgba40.v_jumin_seq        ); 
                V2V_CPY  (s_bhca01.v_chwiduk_seq_no     , s_jgba20.v_chwiduk_seq_no[i]); 

                V2V_CPY_N(s_bhca01.v_goji_tgt_fr_yyyymm , s_jgba40.v_pay_sus_dt    , 6); 
                C2V_CPY_N(s_bhca01.v_goji_tgt_to_yyyymm , gc_today                   , 6); 

                V2V_CPY  (s_bhca01.v_p_chrg_emp_no      , gv_user_id                  ); 
                C2V_CPY  (s_bhca01.v_cre_brch_cd        , s_zztb00.c_brch_cd          ); 
                V2V_CPY  (s_bhca01.v_brch_cd            , s_jgba20.v_brch_cd       [i]); 
                C2V_CPY  (s_bhca01.v_man_yn             , "N"                         ); 

                intRet = f_bhca_d_GetSeqInsTBBHCA01(&s_bhca01);
                if (intRet == FALSE)
                {
                    iErCnt++;
                    gl_reject_cnt++;
            		return(-1);
                }
            }
            else
            if (s_jgba20.v_gaibja_type[i].arr[0] < '7')
            {
                if (iJikCnt != 0) continue;
                     
                if ((strncmp((char*)s_jgba20.v_jung_no [i].arr, "77", 2) == 0) ||
            	    (strncmp((char*)s_jgba20.v_jung_no [i].arr, "78", 2) == 0) ||
            	    (strncmp((char*)s_jgba20.v_jung_no [i].arr, "79", 2) == 0))
            	  {
                      V2V_CPY  (s_bhca01.v_jung_no            , s_jgba20.v_jung_no[i]       ); 
                      C2V_CPY  (s_bhca01.v_ocur_dt            , gc_today                      ); 
                      C2V_CPY  (s_bhca01.v_corr_ocur_rsn_cd   , "14"                        ); 
                      C2V_CPY  (s_bhca01.v_qlf_bugwa_proc_dt  , gc_today                      ); 
                      V2V_CPY  (s_bhca01.v_qlf_bugwa_rpt_dt   , s_select.v_rcv_dt  ); 

                      V2V_CPY  (s_bhca01.v_jumin_no           , s_jgba40.v_jumin_no         ); 
                      V2V_CPY  (s_bhca01.v_jumin_seq          , s_jgba40.v_jumin_seq        ); 
                      V2V_CPY  (s_bhca01.v_chwiduk_seq_no     , s_jgba20.v_chwiduk_seq_no[i]); 

                      V2V_CPY_N(s_bhca01.v_goji_tgt_fr_yyyymm , s_jgba40.v_pay_sus_dt    , 6); 
                      C2V_CPY_N(s_bhca01.v_goji_tgt_to_yyyymm , gc_today                   , 6); 
                      V2V_CPY  (s_bhca01.v_p_chrg_emp_no      , gv_user_id                  ); 
                      C2V_CPY  (s_bhca01.v_cre_brch_cd        , s_zztb00.c_brch_cd          ); 
                      V2V_CPY  (s_bhca01.v_brch_cd            , s_jgba20.v_brch_cd       [i]); 
                      C2V_CPY  (s_bhca01.v_man_yn             , "N"                         ); 

                      intRet = f_bmbk_d_GetSeqInsTBBHCA01(&s_bhca01);    
                      if (intRet == FALSE)
                      {
                          iErCnt++;
                          gl_reject_cnt++;
             	      	  return(-1);
                      }
                }
                else      
                {         
                memset(&s_bmac100  , 0x00, sizeof(s_bmac100));

                V2V_CPY(s_bmac100.v_jumin_no        , s_jgba40.v_jumin_no       );
                V2V_CPY(s_bmac100.v_jumin_seq       , s_jgba40.v_jumin_seq      );
                        s_bmac100.l_pay_sus_seq_no  = s_jgba40.l_pay_sus_seq_no  ;
                V2V_CPY(s_bmac100.v_chwiduk_rsn_cd  , s_jgba40.v_chwiduk_rsn_cd );
                C2V_CPY(s_bmac100.v_forn_hjob_type  , "0"                       );
                V2V_CPY(s_bmac100.v_pay_sus_dt      , s_jgba40.v_pay_sus_dt     );
                V2V_CPY(s_bmac100.v_pay_sus_to_dt   , s_jgba40.v_pay_sus_to_dt  );

                if (strncmp((char*)s_jgba40.v_pay_sus_to_dt.arr, "99991231", 8) == 0)
                {
                    C2V_CPY(s_bmac100.v_work_gubn       , "1"   ); 
                }
                else
                if (strncmp((char*)s_jgba40.v_pay_sus_dt.arr,
                            (char*)s_jgba40.v_pay_sus_to_dt.arr, 8) == 0)
                {
                    
                    C2V_CPY(s_bmac100.v_work_gubn       , "4"   ); 
                }
                else
                {       
                    C2V_CPY(s_bmac100.v_work_gubn       , "2"   ); 
                }

                V2V_CPY(s_bmac100.v_ex_pay_sus_dt   , s_jgba40.v_pay_sus_dt    );
                V2V_CPY(s_bmac100.v_ex_pay_sus_to_dt, s_jgba40.v_pay_sus_to_dt );
                C2V_CPY(s_bmac100.v_proc_brch_cd    , s_zztb00.c_brch_cd       );
                C2V_CPY(s_bmac100.v_proc_pstn_type  , s_zztb00.c_pstn_type     );
                C2V_CPY(s_bmac100.v_proc_dept_cd    , s_zztb00.c_dept_cd       );
                V2V_CPY(s_bmac100.v_proc_emp_no     , gv_user_id               );


               INITCHAR(cGetErrMsg);
               intRet = f_bmac100_u1(&s_bmac100, cGetErrMsg);
               if (intRet == NOT_OK)
               {
                   iErCnt++;
                   gl_reject_cnt++;
                   return(-1);
               }
            }
          }   
        }
    }

    EXEC SQL CLOSE cur_f_jungsan_call;

    return(0);
}

/*@****************************************************************************/
/*@****************************************************************************/
int f_jgaa530_u3_tbjgaa16_u(char *plc_proc_type
                          , char *plc_err_type )
{
    EXEC SQL
    UPDATE     
            TBJGAA16                          
       SET
            PROC_DT           = :gc_today
          , PAY_SUS_PROC_TYPE = :plc_proc_type
          , ERR_TYPE          = :plc_err_type
          , CHG_DTTM          =  sysdate
          , CHG_EMP_NO        =  :gv_user_id
     WHERE
            JUMIN_NO       = :s_select.v_jumin_no
       AND  ARMY_NM        = :s_select.v_nm      
       AND  ENTRARMY_DT    = :s_select.v_bf_elsmt_dt  
    ;

    if (SQL_CODE != SQL_OK)
    {
        iErCnt++;
        gl_reject_cnt++;
        f_jgzz_c_msg_log(__FILE__, "[%s|%s] 근무자료 Update Error !!! : %s\n"
                         , (char*)s_select.v_jumin_no   .arr
                         , (char*)s_select.v_nm    .arr
                         , SQL_ERRTEXT);
    }

    return SQL_CODE;
}

/*@****************************************************************************/
int f_jgaa530_u3_tbjgba40_u()
{
    long ll_seq = 0;

    EXEC SQL
    SELECT 
            NVL(MAX(TO_NUMBER(CHG_SEQ_NO)), 0)
      INTO
            :ll_seq
      FROM 
            TBJGBA41
     WHERE 
            JUMIN_NO       = :s_jgba40.v_jumin_no             
       AND  JUMIN_SEQ      = :s_jgba40.v_jumin_seq            
       AND  PAY_SUS_SEQ_NO = :s_jgba40.l_pay_sus_seq_no       
    ;

    if(SQL_CODE != SQL_OK && SQL_CODE != SQL_NF)
    {
    	  iErCnt++;
        gl_reject_cnt++;
   		  f_jgzz_c_msg_log(__FILE__, "[%s|%s][%d] 변경일련번호 조회 오류 SELECT Error !!! : [%s]\n"
                         , s_jgba40.v_jumin_no       .arr
                         , s_jgba40.v_jumin_seq      .arr
                         , s_jgba40.l_pay_sus_seq_no 
		                 , SQL_ERRTEXT);
        return(SQL_CODE);

    }

    /* 자료가 없는 경우 0 으로 Set */
    if(SQL_CODE == SQL_NF || ll_seq == 0)
    {
        ll_seq = 0;
    }

    /* 일련번호 증가 */
    ll_seq = ll_seq + 1;

    EXEC SQL
    INSERT INTO TBJGBA41
           (
           JUMIN_NO            
          ,JUMIN_SEQ           
          ,PAY_SUS_SEQ_NO      
          ,CHG_SEQ_NO          
          ,PROC_DT             
          ,CHWIDUK_RSN_CD      
          ,CBF_FORN_HJOB_TYPE  
          ,CBF_PAY_SUS_DT      
          ,CBF_PAY_SUS_TO_DT   
          ,PAY_SUS_CHG_EMP_NO  
          ,PROC_BRCH_CD        
          ,PROC_PSTN_TYPE      
          ,PROC_DEPT_CD        
          ,CRE_DTTM            
          ,CRE_EMP_NO          
          ,CBF_RMK             
           )
    SELECT
            JUMIN_NO                            
           ,JUMIN_SEQ                           
           ,PAY_SUS_SEQ_NO                      
           ,LPAD(TRIM(TO_CHAR(:ll_seq)), 3, '0')
           ,TO_CHAR(SYSDATE, 'YYYYMMDD')        
           ,CHWIDUK_RSN_CD                      
           ,FORN_HJOB_TYPE                  
           ,PAY_SUS_DT                      
           ,PAY_SUS_TO_DT                   
           ,:gv_user_id                          
           ,:gv_brch_cd
           ,:gv_pstn_type
           ,'000'
           ,SYSDATE                             
           ,:gv_user_id
           ,RMK                                 
      FROM 
            TBJGBA40
     WHERE 
            JUMIN_NO       = :s_jgba40.v_jumin_no             
       AND  JUMIN_SEQ      = :s_jgba40.v_jumin_seq            
       AND  PAY_SUS_SEQ_NO = :s_jgba40.l_pay_sus_seq_no       
    ;

    if(SQL_CODE != SQL_OK)
    {
    	  iErCnt++;
        gl_reject_cnt++;
   		  f_jgzz_c_msg_log(__FILE__, "[%s|%s][%d][%d] 변경이력등록 오류 SELECT Error !!! : [%s]\n"
                         , s_jgba40.v_jumin_no       .arr
                         , s_jgba40.v_jumin_seq      .arr
                         , s_jgba40.l_pay_sus_seq_no 
                         , ll_seq
		                 , SQL_ERRTEXT);
        return(SQL_CODE);

    }
    
    EXEC SQL
    UPDATE 
             TBJGBA40
       SET
             CHWIDUK_RSN_CD        = :s_jgba40.v_chwiduk_rsn_cd                  
           , TO_PROC_BRCH_CD       = :s_jgba40.v_to_proc_brch_cd                 
           , TO_PROC_PSTN_TYPE     = :s_jgba40.v_to_proc_pstn_type               
           , TO_PROC_DEPT_CD       = :s_jgba40.v_to_proc_dept_cd                 
           , PAY_SUS_TO_DT         = :s_jgba40.v_pay_sus_to_dt                   
           , PAY_SUS_TO_RPT_DT     = :s_jgba40.v_pay_sus_to_rpt_dt               
           , PAY_SUS_TO_EMP_NO     = :s_jgba40.v_pay_sus_to_emp_no               
           , PAY_SUS_CNCLCTR_IN_DT = :s_jgba40.v_pay_sus_cnclctr_in_dt           
           , IN_TYPE               = '2'
           , CHG_DTTM              = SYSDATE             
           , CHG_EMP_NO            = :gv_user_id          
     WHERE
             JUMIN_NO              = :s_jgba40.v_jumin_no             
       AND   JUMIN_SEQ             = :s_jgba40.v_jumin_seq            
       AND   PAY_SUS_SEQ_NO        = :s_jgba40.l_pay_sus_seq_no       
         ;

    if(SQL_CODE == SQL_OK)
    {
        gl_chg_cnt ++;
    }

    return(SQL_CODE);
}

/********************************************************************************/
/*                              END OF SOURCE                                   */
/********************************************************************************/
