

#define ONBATCH

#define  DEVELOP_ENV
#define  FILEIO    1
#define  TEMPIO    2

#define BATCH_PATH         getenv("BATCH_APP_PATH_SVC")
#define ONBATCH_PATH       getenv("ONBATCH_FILE_PATH_SVC")
#define TMP_PATH           getenv("BATCH_FILE_PATH_SVC_TEMP")
#define FILEIO_PATH        getenv("BATCH_FILE_PATH_SVC")

#define FIRMRECV_PATH      "./fileio/firmrecv"
#define FIRMBACK_PATH      "./fileio/firmback"
#define FIRMTRANS_PATH     "./fileio/firmtrans"  
#define FIRMJOJUNG_PATH    "./DB/DB12/work"  
#define FIRMTRANS_JLOAD    "./fileio/firmtrans/jload"  
#define FIRMTRANS_JFETCH   "./fileio/firmtrans/jfetch"  
#define FIRMTRANS_JBACKUP  "./fileio/firmtrans/jbackup"  

/*******************************************************************************
 화일 Dir 위치 정의 (데이터 화일, 로그, 레포트 등)
 ******************************************************************************/
#define  LOG_DIR     getenv("BATCH_LOG_PATH_SVC")
#define  FILEIO_DIR  getenv("BATCH_FILE_PATH_HMG_DAT_SVC")
#define  RPT_DIR     getenv("BATCH_FILE_PATH_HMG_RPR_SVC")
