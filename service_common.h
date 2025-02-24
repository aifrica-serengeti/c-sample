
/*******************************************************************************
 일반 공통
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/timeb.h>

/*******************************************************************************
 Oracle
 ******************************************************************************/
#include  "sqlca.h"
#include <macro.h>
#include "common.h"                      

#include <service.fld.h>


#ifdef ONBATCH
EXEC SQL include tblstr.h;                          
EXEC SQL INCLUDE batchprt.h;
#endif


#define   TMP_DIR getenv("BATCH_FILE_PATH_JG_TEMP")

#include "service_path.h"
#include "service_ipc.h"                                 
EXEC SQL include jg_az001.h;                        
EXEC SQL include jg_tz001.h;                        
EXEC SQL include jg_ea001.h;                        
EXEC SQL include jg_bz001.h;                        
EXEC SQL include b_jgec300_u1.h;                    

EXEC SQL include jg_if001.h;                        


#define  U_BLANK     "                                                        " 

static char *gcCenNm[] = {"01 서울지역본부",
                          "02 부산지역본부",
                          "03 대구지역본부",
                          "04 광주지역본부",
                          "05 대전지역본부",
                          "06 경기지역본부"
                         }; 



/* --------------------------------------------------------------------------
   Function Prototype
** -------------------------------------------------------------------------- */
extern int  f_GetBrchCd( 
                  VARCHAR *pvPut_BrchCd,           
                  VARCHAR *pvPut_Pstn_type,        
                  VARCHAR *pvGet_Si_do_cd,         
                  VARCHAR *pvGet_Si_gun_gu_cd,     
                  VARCHAR *pvGet_H_dong_cd );      
#if 0
extern char *f_GetDateTime(char *buf);
#endif

extern void  f_C2V_CPY_Trim(VARCHAR *vBuf, char *cBuf, int n);
extern int   f_isNumeric(char *Str);
extern int   f_isDate(char *chkDate);
extern int   f_aToi_n(char* Str, short Leng);
extern int   f_chkJuminNo(char *chkJuminNo);
extern int   f_chkName(char *cNm);
extern void  f_Cursor  (short line, short col);
extern void  f_Display (short line, short col, char *buf);
extern void  f_DisplayN(short line, short col, char *buf, short length);
extern void  f_Accept  (short line, short col, char *buf);
extern int   f_GetPostNo(
            char *cPostNo,
            char *cSiDoCd,
            char *cSiGunGuCd,
            char *cHdongCd,
            char *cBdongCd,
            char *cRiCd,
            char *cTong,
            char *cSanType,    
            char *cBunji,
            char *cHo,
            char *cSpcYn,
            char *cSpcDong );
extern void  f_jgzz_c_msg_log(char *filename, const char *format, ...);
extern int   f_jgab_d_tbjgab00_i();

extern int  f_DateCal_M(char *cSyyyymm, char *cEyyyymm);

extern int  f_jgaa_d_ChkSodukPibu ();

extern int  f_jgtz_get_addr(__S_JGTZ_ADDR__ *s_jgtz_addr);

extern int  f_jgtz_get_firm_addr(__S_JGTZ_FIRM_ADDR__ *s_jgtz_firm_addr);

extern int  f_cwjgba_bugwa_yn(
                           char *c_jumin_no      , 
                           char *c_gaibja_type   , 
                                                   
                                                   
                                                   
                                                   
                                                   
                                                   
                           char *c_chwiduk_dt    ,
                           char *c_chwiduk_rsn_cd,
                           char *c_bugwa_yn      , 
                           char *c_err_msg         
                             );


extern int f_jgba_last_chk(__S_LAST_CHK__ *s_last_chk);

extern int f_jgba_cre_sedaeju(
                              __S_JGBA_COM__  *s_jgba_com,
                              __S_JGBA_GAIB__ *s_jgba_gaib,
                              char *c_bf_jung_no,
                              long  i_bf_jung_seq_no
                             );
extern int f_jgba_sedaeju_check (
                                 __S_JGBA_COM__  *s_jgba_com,
                                 char *ac_sedaeju_jumin_no  ,
                                 char *ac_sedaeju_jumin_seq ,
                                 char *ac_bf_jung_no        ,
                                 long  al_bf_jung_seq_no    ,
                                 char *ac_jgba24_yn          
                                );

extern int f_jgtz_4insu_send_jumin(__S_4INSU_JUMIN__ *s_4insu_jumin );

extern int f_jgtz_common_jgea10_insert(__S_JGEA10__ *s_jgea10 );

extern int f_jgtz_get_insurance_stop(char *,char *,char *,char *,char *) ;

extern int f_jgtz_common_jgbz10_insert(S_JGBZ10 *s_jgbz10, long *pSeqNo) ;


extern int  f_jgzz_c_FileToMemory(__S_JG_TMPFILE__  *s_file );
extern FILE  *f_jgzz_c_FileCreate(
                         FILE *fp                     /* 화일 Pointer ; in-out*/
                       , __S_JG_TMPFILE__  *s_file );
extern int f_jgba_entry_reject(
                                char *c_pibu_jumin_no   ,  
                                char *c_pibu_jumin_seq  ,  
                                char *c_gaibja_jumin_no ,  
                                char *c_gaibja_jumin_seq,  
                                char *c_reject_yn       ,  
                                char *c_err_msg            
                              );

extern int f_jgba_get_qlf_type(
                                char *c_jumin_no    ,  
                                char *c_gaibja_type ,  
                                                       
                                                       
                                                       
                                                       
                                                       
                                                       
                                char *c_jung_no     ,  
                                int   i_jung_seq_no ,  
                                char *c_qlf_type    ,  
                                                       
                                                       
                                                       
                                                       
                                char *c_err_msg        
                              );

extern int f_jgba_firm_chwiduk(__S_JGBA_COM__    *s_jgba_com   ,
                               __S_JGBA_GAIB__   *s_jgba_gaib  ,
                               __S_JGBA_PERSON__ *s_jgba_person);
extern int f_jgba_firm_chwiduk_siis_20180901(__S_JGBA_COM__    *s_jgba_com   ,
                               __S_JGBA_GAIB__   *s_jgba_gaib  ,
                               __S_JGBA_PERSON__ *s_jgba_person);
extern int f_jgba_firm_chwiduk_chg(__S_JGBA_COM__    *s_jgba_com   ,
                                   __S_JGBA_GAIB__   *s_jgba_gaib  ,
                                   __S_JGBA_PERSON__ *s_jgba_person);
extern int f_jgba_firm_chwiduk_cont(__S_JGBA_COM__    *s_jgba_com   ,
                                    __S_JGBA_GAIB__   *s_jgba_gaib  ,
                                   __S_JGBA_PERSON__ *s_jgba_person);                                   

extern int f_jgba_chwiduk(__S_JGBA_COM__    *s_jgba_com   ,
                          __S_JGBA_GAIB__   *s_jgba_gaib  ,
                          __S_JGBA_PERSON__ *s_jgba_person,
                          char *c_new_jung_yn);

extern int f_jgba_chwiduk_20181129(__S_JGBA_COM__    *s_jgba_com   ,
                                           __S_JGBA_GAIB__   *s_jgba_gaib  ,
                                           __S_JGBA_PERSON__ *s_jgba_person,
                                           char *c_new_jung_yn);
extern int f_jgba_firm_sangsil(__S_JGBA_COM__    *s_jgba_com   ,
                               __S_JGBA_GAIB__   *s_jgba_gaib  ,
                               __S_JGBA_PERSON__ *s_jgba_person);
extern int f_jgba_firm_sangsil_20180901(__S_JGBA_COM__    *s_jgba_com   ,
                                                  __S_JGBA_GAIB__   *s_jgba_gaib  ,
                                                  __S_JGBA_PERSON__ *s_jgba_person);

extern int f_jgba_sangsil(__S_JGBA_COM__    *s_jgba_com   ,
                          __S_JGBA_GAIB__   *s_jgba_gaib  ,
                          __S_JGBA_PERSON__ *s_jgba_person);

extern int f_jgba_sangsil_20181129(__S_JGBA_COM__    *s_jgba_com   ,
                                             __S_JGBA_GAIB__   *s_jgba_gaib  ,
                                             __S_JGBA_PERSON__ *s_jgba_person);

extern int f_jgba_get_birth_yyyymm(char *lc_jumin_no ,
                                   char *lc_birth_yyyymm, 
                                   char *lc_err_msg);

extern int f_jgba_check_holiday(char *lc_gaibja_type      ,
                                char *lc_jumin_no         ,
                                char *lc_gaibja_chwiduk_dt,
                                char *lc_pibu_chwiduk_dt  ,
                                char *lc_chwiduk_rsn_cd   ,
                                char *lc_err_msg);

extern int f_jgba_get_fst_chwiduk(char *lc_jumin_no       ,
                                  char *lc_jumin_seq      ,
                                  char *lc_chwiduk_rsn_cd ,
                                  char *lc_chwiduk_dt     ,
                                  char *lc_fst_chwiduk_dt ,
                                  char *lc_err_msg);

extern int f_jgba_get_jumin_seq(char *lc_jumin_no , 
                                char *lc_jumin_seq,
                                char *lc_nm       ,
                                char *lc_err_msg);

extern int f_jgba_get_jung(
                             int i_busi_type,
                             int i_new_type , 
                            char *c_firm_sym ,
                            char *c_jumin_no , 
                            char *c_jumin_seq, 
                            char *c_jung_no  , 
                            long *l_jung_seq ,
                            char *c_old_jung_no,
                            char *c_jung_yn
                           )  ;

extern int f_jgaz_d_GetBrch_PostNo(
            char *cPostNo,                              
            char *cBrchCd,                              
            char *cPstnType);                           

extern int f_jgaa_d_tbjgda10_i(__S_JGDA10__ *s_jgda10) ;

extern int f_jgaa_d_tbjgda13_i(__S_JGDA13__ *s_jgda13);

extern int f_jgzz_d_GetEmpInfo(__S_ZZTB00__ *s_zztd00 );

extern int f_jgba_pibu_conn(__S_JGBA_COM__    *s_jgba_com   ,
                            __S_JGBA_GAIB__   *s_jgba_gaib  ,
                            __S_JGBA_PERSON__ *s_jgba_person,
                            __S_PIBU_CHWIDUK__  *s_pibu_chwiduk);

extern int  f_jgaa_d_pibu_aprroval_yn (__S_PIBU_REL_CHK__    *s_PibuRelChk);

extern int  f_jgba_pibu_check_main (S_PIBU_CHECK *as_pibu_check_in, S_PIBU_RESULT *as_pibu_result_in);

extern int f_remove_char(char *pcInBuf, char *pcOutBuf, int n);

extern int f_jgaa_d_tbjgba40_i(__S_JGBA40__ *s_jgba40);

extern int f_jgaa_d_tbjgba60_i(__S_JGBA60__ *s_jgba60);

extern int f_jgaa_GetRegSoduk_TBBHAA10_s(__S_SODUK_CHK__    *s_soduk_chk);

extern int f_jgaa_GetTotSoduk_TBBHAA10_s(__S_SODUK_CHK__    *s_soduk_chk);

extern char  *f_DateAdd(
                 char *cDate,   
                 char *cOutBuf, 
                 char cType,    
                 int ymd);       

extern int f_CreateSharedMemory(int *iShmId, int iShmKey, int iMemSize);

extern int f_RemoveSharedMemory(int iShmId);

extern void sigchld_handler(int sig);

extern void *f_AttachSMemory(int shmid);

extern int f_GetDayOfWeek( char * pcharYyyymmdd );

extern int f_jgba_last_chg_jung(char *ac_jung_no     , 
                                char *ac_proc_emp_no ,
                                char *ac_err_msg     );

extern int f_jgba_last_chg_jumin(char *ac_jumin_no   ,
                                 char *ac_jumin_seq  ,
                                 char *ac_proc_emp_no,
                                 char *ac_err_msg    );


extern int f_jgba_chwiduk_temp(__S_JGBA_COM__    *s_jgba_com   ,
                               __S_JGBA_GAIB__   *s_jgba_gaib  ,
                               __S_JGBA_PERSON__ *s_jgba_person);

extern int f_jgba_firm_sangsil_temp(__S_JGBA_COM__    *s_jgba_com   ,
                                    __S_JGBA_GAIB__   *s_jgba_gaib  ,
                                    __S_JGBA_PERSON__ *s_jgba_person);

extern int f_jgba_firm_sangsil_conn(__S_JGBA_COM__    *s_jgba_com   ,
                                    __S_JGBA_GAIB__   *s_jgba_gaib  ,
                                    __S_JGBA_PERSON__ *s_jgba_person);

int f_jgtz_common_jgba50_update(char *p_mode        ,
                                char *p_jumin_no    ,
                                char *p_jumin_seq   ,
                                char *p_jung_no     ,
                                char *p_rdnl_dt     ,
                                char *p_rdnl_rpt_dt ,
                                char *p_brch_cd     ,
                                char *p_emp_no      );                                    

extern int f_jgba_firm_sangsil_only(__S_JGBA_COM__    *s_jgba_com   ,
                                    __S_JGBA_GAIB__   *s_jgba_gaib  ,
                                    __S_JGBA_PERSON__ *s_jgba_person);

extern int f_Jgaa_GetTotGwapyo_Std_Amt(__S_PIBU_REL_CHK__    *s_PibuRelChk);

typedef struct
{
    VARCHAR v_nps    [5000][  11+1]; 
    VARCHAR v_yyyymm [5000][   6+1]; 
    VARCHAR v_data   [5000][4000+1]; 
} S_NPS_CHG;


typedef struct
{
    int  i_col_no      [500]       ; 
    char c_col_val     [500][500+1]; 
    int  i_col_row     [500]       ; 
} S_NPS_CHG_DATA;


typedef struct
{
    VARCHAR v_value[500][1000+1]; 
} S_SEP_TEXT;

typedef struct
{
    VARCHAR v_bugwa_base_data_type  [  1+1];    
    VARCHAR v_bugwa_proc_std_dt     [  8+1];    
    VARCHAR v_adpt_yyyymm           [  6+1];    
    VARCHAR v_errmsg                [100+1];    
} S_F_BHAA92_ADPT_YYYYMM;

typedef struct
{
    VARCHAR v_working_yyyymmdd      [  8+1];    
    VARCHAR v_adapt_step            [  2+1];    
    VARCHAR v_ErrMsg                [100+1];    
} S_F_ZZTC11_WORK_STEP;


typedef struct
{
    VARCHAR v_category                      [ 2+1]; 
    VARCHAR v_adpt_yyyymm                   [ 6+1]; 
    VARCHAR v_base_chwiduk_dt               [ 8+1]; 
    VARCHAR v_pibu_soduk_pttn_type_cd       [ 2+1]; 
    VARCHAR v_pibu_soduk_rcgt_pttn_type_cd  [ 2+1]; 
    VARCHAR v_adpt_fr_yyyymm                [ 6+1]; 
    VARCHAR v_adpt_to_yyyymm                [ 6+1]; 
    double  lf_fr_gwase_soduk_amt;                  
    double  lf_to_gwase_soduk_amt;                  
    VARCHAR v_ErrMsg                        [100+1];
    VARCHAR v_base_cd                       [ 2+1]; 
} S_F_JGAA60_JUDGE;

extern int f_jgaa_d_obst_yugongja_check( char *cJuminNo, char *cAdpt_dt  );

extern int f_jgaa_d_dongge_check(char *c_gaibja_jumin_no, char *c_pibu_jumin_no );

int f_common_bh_conn_adpt_yymm(S_F_BHAA92_ADPT_YYYYMM* s_adpt_yyyymm);

int f_common_jg_get_step(S_F_ZZTC11_WORK_STEP* s_work_step);

int f_common_jg_yy_soduk(__S_SODUK_CHK__* s_soduk_chk, char* lc_jumin_no, double* ld_yy_amt, char* lcStep);

int f_common_jg_anty_soduk(__S_SODUK_CHK__* s_soduk_chk, char* lc_jumin_no, double* ld_anty_amt, char* lcStep);

int f_common_jg_work_etc_soduk(__S_SODUK_CHK__* s_soduk_chk, char* lc_jumin_no, double* ld_work_etc_amt, char* lcStep);

int f_common_jg_fin_soduk(__S_SODUK_CHK__* s_soduk_chk, char* lc_jumin_no, double* ld_fin_amt, char* lcStep);

int f_common_jg_get_judge(S_F_JGAA60_JUDGE* s_judge);

int f_common_jg_get_reopen_cnt(char* lc_jumin_no, int* li_reopen_cnt, char* cErrMsg, char* cProcDt);

int f_common_jg_get_biz_reg_yn(char* lc_jumin_no, int* li_reg_yn, char* lc_reg_no, char* cErrMsg);

int f_common_jg_ex_soduk(__S_SODUK_CHK__* s_soduk_chk, char* lc_jumin_no, double* ld_ex_soduk_amt, char* lcYyyyMm);

int f_common_jg_bz_soduk(__S_SODUK_CHK__* s_soduk_chk, char* lc_jumin_no, double* ld_soduk_amt, char* lcYyyyMm);


int f_common_jg_get_db_datetime(__S_DB_TIME__ *s_dbtime );

int f_common_bh_yy_soduk(char* lcLastBugwaYyyyMm, char* lc_jumin_no, double* ld_yy_amt, char* c_ErrMsg, char* cProcDt);

int f_common_bh_bz_soduk(char* lcLastBugwaYyyyMm, char* lc_jumin_no, double* ld_soduk_amt, char* c_ErrMsg);

int f_common_bh_ex_soduk(char* lcLastBugwaYyyyMm, char* lc_jumin_no, double* ld_ex_soduk_amt, char* c_ErrMsg);

int f_common_bh_anty_soduk(char* lcLastBugwaYyyyMm, char* lc_jumin_no, double* ld_anty_amt, char* c_ErrMsg);

int f_common_bh_work_soduk(char* lcLastBugwaYyyyMm, char* lc_jumin_no, double* ld_work_amt, char* c_ErrMsg);

int f_common_bh_etc_soduk(char* lcLastBugwaYyyyMm, char* lc_jumin_no, double* ld_etc_amt, char* c_ErrMsg);

int f_common_bh_intr_soduk(char* lcLastBugwaYyyyMm, char* lc_jumin_no, double* ld_intr_amt, char* c_ErrMsg);

int f_common_bh_dvd_soduk(char* lcLastBugwaYyyyMm, char* lc_jumin_no, double* ld_dvd_amt, char* c_ErrMsg);

int f_common_bh_fin_soduk(char* lcLastBugwaYyyyMm, char* lc_jumin_no, double* ld_fin_amt, char* c_ErrMsg);

int f_common_bh_work_etc_soduk(char* lcLastBugwaYyyyMm, char* lc_jumin_no, double* ld_work_etc_amt, char* c_ErrMsg);

int f_common_jg_sumtax(__S_PIBU_REL_CHK__* s_PibuRelChk, char* lc_jumin_no, char* lcStep);

double f_common_bh_pptax(char* lcLastBugwaYyyyMm, char* lc_jumin_no);

double f_common_bh_altax(char* lcLastBugwaYyyyMm, char* lc_jumin_no);

double f_common_bh_hstax(char* lcLastBugwaYyyyMm, char* lc_jumin_no);

extern int f_jgaa_pibu_family_extrt(char    *s_jumin_no  ,
                                    char    *s_jumin_seq ,
                                    char    *s_RecvDt,
                          _S_PIBU_FAMILY    *s_pibu_fam
                                        );

extern int f_jgfb_forn_chg(char* ac_jumin_no
                          ,char* ac_chwiduk_scd_dt
                          ,char* ac_proc_tgt_yn
                          ,char* ac_brch_cd
                          ,char* ac_pstn_type
                          ,char* ac_dept_cd
                          ,char* ac_emp_no
                          ,char* ac_errmsg
                          );

extern int f_jgtz_common_get_mng_brch_cd(S_MNG_BRCH_CD * s_mng_brch_cd
                                        ,char* ac_emp_no
                                        ,char* ac_pgm_id
                                        ,char* ac_errmsg
                                        );

