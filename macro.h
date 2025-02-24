/*  Initialize memory  */
#define INITCHAR(x)	memset(x, 0x00, sizeof(x)); 
#define INITVAR(x) { \
        memset(x.arr,0x00,sizeof(x.arr));\
        x.len = 0; }
#define INIT2CHAR(x,n) { \
        int inx; \
        for(inx=0 ; inx < n ; inx++) { \
            memset(x[inx],0x00,sizeof(x[inx])); } \
}
#define INIT2VAR(x,n) { \
        int inx; \
        for(inx=0 ; inx < n ; inx++) { \
            memset(x[inx].arr,0x00,sizeof(x[inx].arr));\
        x[inx].len = 0; } \
}
   
#define INITSTRU(x)	memset(&x, 0x00, sizeof(x)); 
#define INIT2STRU(x,n) { \
        int idx = 0; \
        for(idx = 0; idx < n; idx++) { \
            memset(x[idx], 0x00, sizeof(x[idx])); } \
}

   
/* #define INITSTRU_D(x,y) memset((char *)&y, 0x00, sizeof(x)); */

/* structure pointer를  memset */
#define INITSTRU_P(x,y) memset(y, 0x00, sizeof(x));
      	
#define FOCCUR(x)    (long)Foccur32(transf,(x));
#define INITFML(x)         Finit32(x, Fsizeof32(x));

/*---------------------------------------------------------------------------------*/
/* raw Type의 Data 처리 -- start --*/
#define GETRAW(x,y,z)	{ \
   (z.len) = 0; \
   Fget32((FBFR32 *)(transf), (FLDID32)(x), (FLDOCC32)(y), (char *)(z.arr), \
		(FLDLEN32 *)(&z.len)); }

#define PUTRAW(x,y,z)	{ \
   Fchg32((FBFR32 *)(transf), (FLDID32)(x), (FLDOCC32)(y), (char *)(z.arr), \
		(FLDLEN32)(z.len)); }
		
/* macro for FML buffer fbfr */
#define FBFR_GETRAW(x,y,z)	{ \
   (z.len) = 0; \
   Fget32((FBFR32 *)(fbfr), (FLDID32)(x), (FLDOCC32)(y), (char *)(z.arr), \
		(FLDLEN32 *)(&z.len)); }

#define FBFR_PUTRAW(x,y,z)	{ \
   Fchg32((FBFR32 *)(fbfr), (FLDID32)(x), (FLDOCC32)(y), (char *)(z.arr), \
		(FLDLEN32)(z.len)); }
		
/* raw Type의 Data 처리 -- end --*/
/*---------------------------------------------------------------------------------*/

/* Debug Option 처리 -- start --*/
#ifdef DEBUG
#define GET(x,y,z)	{ \
   *z = 0x00; \
   if(Fget32((FBFR32 *)(transf), (x), (y), (char *)(z), 0) == -1) \
      printf("Fget32() error -> %s !\n", Fstrerror32(Ferror32)); }
#else
#define GET(x,y,z)	{ \
   	*z = 0x00; \
	Fget32((FBFR32 *)(transf), (x), (y), (char *)(z), 0); }
#endif 

#ifdef DEBUG
#define GETVAR(x,y,z)	{ \
   (z.arr)[0] = 0x00; \
   if(Fget32((FBFR32 *)(transf), (x), (y), (char *)(z.arr), 0) == -1) \
      printf("Fget32(%s) error -> %s !\n", (char *)z.arr, Fstrerror32(Ferror32)); \
   (z.len) = strlen((char *)(z.arr)); \
   (z.arr)[(z.len)]=0x00; }
#else
#define GETVAR(x,y,z)	{ \
   (z.arr)[0] = 0x00; \
   Fget32((FBFR32 *)(transf), (x), (y), (char *)(z.arr), 0); \
   (z.len) = strlen((char *)(z.arr)); \
   (z.arr)[(z.len)]=0x00; }
#endif 

#ifdef DEBUG
#define PUT(x,y,z)	{ \
   if(Fchg32((FBFR32 *)(transf), (x), (y), (char *)(z), 0) == -1) \
      printf("Fchg32() error -> %s !\n", Fstrerror32(Ferror32)); }
#else
#define PUT(x,y,z)	Fchg32((FBFR32 *)(transf), (x), (y), (char *)(z), 0)
#endif 

#ifdef DEBUG
#define PUTVAR(x,y,z)	{ \
   (z.arr)[(z.len)]=0x00; \
   if(Fchg32((FBFR32 *)(transf), (x), (y), (char *)(z.arr), (z.len)) == -1) \
      printf("Fchg32(%s) error -> %s !\n", (char *)(z.arr), Fstrerror32(Ferror32)); }
#else
#define PUTVAR(x,y,z)	{ \
   (z.arr)[(z.len)]=0x00; \
   Fchg32((FBFR32 *)(transf), (x), (y), (char *)(z.arr), (z.len)); }
#endif 

/* macro for FML buffer fbfr */
#ifdef DEBUG
#define FBFR_GET(x,y,z)	{ \
   *z = 0x00; \
   if(Fget32((FBFR32 *)(fbfr), (x), (y), (char *)(z), 0) == -1) \
      printf("Fget32() error -> %s !\n", Fstrerror32(Ferror32)); }
#else
#define FBFR_GET(x,y,z)	{ \
   	*z = 0x00; \
	Fget32((FBFR32 *)(fbfr), (x), (y), (char *)(z), 0); }
#endif 

#ifdef DEBUG
#define FBFR_GETVAR(x,y,z)	{ \
   (z.arr)[0] = 0x00; \
   if(Fget32((FBFR32 *)(fbfr), (x), (y), (char *)(z.arr), 0) == -1) \
      printf("Fget32(%s) error -> %s !\n", (char *)(z.arr), Fstrerror32(Ferror32)); \
   (z.len) = strlen((char *)(z.arr)); \
   (z.arr)[(z.len)]=0x00; }
#else
#define FBFR_GETVAR(x,y,z)	{ \
   (z.arr)[0] = 0x00; \
   Fget32((FBFR32 *)(fbfr), (x), (y), (char *)(z.arr), 0); \
   (z.len) = strlen((char *)(z.arr)); \
   (z.arr)[(z.len)]=0x00; }
#endif 

#ifdef DEBUG
#define FBFR_PUT(x,y,z)	{ \
   if(Fchg32((FBFR32 *)(fbfr), (x), (y), (char *)(z), 0) == -1) \
      printf("Fchg32() error -> %s !\n", Fstrerror32(Ferror32)); }
#else
#define FBFR_PUT(x,y,z)	Fchg32((FBFR32 *)(fbfr), (x), (y), (char *)(z), 0)
#endif 

#ifdef DEBUG
#define FBFR_PUTVAR(x,y,z)	{ \
   (z.arr)[(z.len)]=0x00; \
   if(Fchg32((FBFR32 *)(fbfr), (x), (y), (char *)(z.arr), (z.len)) == -1) \
      printf("Fchg32() error -> %s !\n", Fstrerror32(Ferror32)); }
#else
#define FBFR_PUTVAR(x,y,z)	{ \
   (z.arr)[(z.len)]=0x00; \
   Fchg32((FBFR32 *)(fbfr), (x), (y), (char *)(z.arr), (z.len)); }
#endif 

#ifdef DEBUG
#define DBLOG(x) { \
        userlog("[%s] -> [%d]"  , (char *)x, sqlca.sqlcode); \
        printf ("[%s] -> [%d]\n", (char *)x, sqlca.sqlcode); }
#else
#define DBLOG(x) { \
        userlog("[%s] -> [%d]"  , (char *)x, sqlca.sqlcode); }
#endif  

/* Debug Option 처리 -- End --*/
/*---------------------------------------------------------------------------------*/

#define C2C_CPY(x,y)	{ \
   strcpy((char *)(x), (char *)(y)); }   

#define V2C_CPY(x,y)	{ \
   strcpy((char *)(x), (char *)(y.arr)); }   

#define C2V_CPY(x,y)	{ \
   strcpy((char *)(x.arr), (char *)(y)); \
   (x.len) = strlen(y); \
   (x.arr)[(x.len)] = 0x00; }

#define V2V_CPY(x,y)	{ \
   (y.arr)[(y.len)] = 0x00; \
   strcpy((char *)(x.arr), (char *)(y.arr)); \
   (x.len) = (y.len); \
   (x.arr)[(x.len)] = 0x00; }

#define V2V_CPY_N(x,y,n)	{ \
   (y.arr)[(y.len)] = 0x00; \
   strncpy((char *)(x.arr), (char *)(y.arr), n); \
   (x.len) = (n); \
   (x.arr)[(x.len)] = 0x00; }

#define C2V_CPY_N(x,y,n)	{ \
   strncpy((char *)(x.arr), (char *)(y), n); \
   (x.len) = (n); \
   (x.arr)[(x.len)] = 0x00; }
   
#define V2C_CPY_N(x,y,n)	{ \
   strncpy((char *)(x), (char *)(y.arr), n); }   
   
#define C2C_CPY_N(x,y,n)	{ \
   strncpy((char *)(x), (char *)(y), n); }      
   

#define V2V_CMP(x,y)	strcmp((char *)(x.arr), (char *)(y.arr)) 

#define V2C_CMP(x,y)   strcmp((char *)(x.arr), (char *)(y)) 

#define C2C_CMP(x,y)   strcmp((char *)(x), (char *)(y)) 

#define V2C_CMP_N(x,y,n,rval)	{ \
   (x.arr)[(x.len)] = 0x00; \
   rval = strncmp((char *)(x.arr), (char *)(y), (int)n); }

#define V2V_CMP_N(x,y,n,rval)	{ \
   (x.arr)[(x.len)] = 0x00; (y.arr)[(y.len)] = 0x00; \
   rval = strncmp((char *)(x.arr), (char *)(y.arr), (int)n); }

#define C2C_CMP_N(x,y,n,rval)	{ \
   rval = strncmp((char *)(x), (char *)(y), (int)n); }

#define V2C_CAT(x,y)	{ \
   (x.arr)[(x.len)] = 0x00; \
   strcat((char *)(x.arr), (char *)(y)); \
   (x.len) += strlen((y)); \
   (x.arr)[(x.len)] = 0x00; }

#define V2V_CAT(x,y)	{ \
   (x.arr)[(x.len)] = 0x00; (y.arr)[(y.len)] = 0x00; \
   strncat((char *)(x).arr, (char *)(y).arr, (y.len)); \
   (x.len) += (y.len); \
   (x.arr)[(x.len)] = 0x00; }
   
#define V2V_CAT_N(x,y,pos,length)	{ \
   (x.arr)[(x.len)] = 0x00; (y.arr)[(y.len)] = 0x00; \
   memcpy((char *)(&((x).arr[x.len])),(char *)(&((y).arr[pos])),length); \
   (x.len) += length; \
   (x.arr)[(x.len)] = 0x00; }   

#define C2C_CAT(x,y)    { \
   strcat((char *)(x), (char *)(y)); }

#define C2V_CAT_TAB(x,y)    { \
   strncat((char *)(x), (char *)(y).arr, (y.len)); \
   strcat((char *)(x), TAB); \
   (x)[strlen(x)] = 0x00; }

#define V2L(x, rval) { \
   (x.arr)[(x.len)] = 0x00; \
   rval = atoi((char *)(x.arr)); }

#define L2V(rval, x) { \
   sprintf((char *)(x.arr), "%d", rval); \
   (x.len) = strlen((char *)(x.arr)); \
   (x.arr)[(x.len)] = 0x00; }
   
static struct tm  *t;
static struct tm  *t1;
static struct timeb s_tm,e_tm;
static struct timeb s1_tm,e1_tm;
static char hc_svc_name[20];


#define SERVICE_START(a) {  \
        ftime(&s_tm);   \
        t = localtime(&s_tm.time);  \
        printf("[%s] 시작시각 => [%02d:%02d:%02d:%04d]\n",a,    \
                t->tm_hour,t->tm_min,t->tm_sec,s_tm.millitm);   \
        f_DBMS_APPLICATION_INFO_SET_MODULE(a); \
        f_SetSvcName(a); }
#define SERVICE_END(a) {    \
        ftime(&e_tm);   \
        t = localtime(&e_tm.time);  \
        printf("[%s] 종료시각 => [%02d:%02d:%02d:%04d]\n",a,   \
            t->tm_hour,t->tm_min,t->tm_sec,e_tm.millitm);   \
        printf("[%s] 수행시간 => [%04d:%04d]\n",a, \
                (int)((e_tm.time*1000+e_tm.millitm) -    \
                      (s_tm.time*1000+s_tm.millitm))/1000,   \
                (int)((e_tm.time*1000+e_tm.millitm) -    \
                      (s_tm.time*1000+s_tm.millitm))%1000);  }
#define TIMER_START(a) {  \
        ftime(&s1_tm);   \
        t1 = localtime(&s1_tm.time);  \
        printf("[%s] 시작시각 => [%02d:%02d:%02d:%04d]\n",a,    \
                t1->tm_hour,t1->tm_min,t1->tm_sec,s1_tm.millitm);   }
#define TIMER_END(a) {    \
        ftime(&e1_tm);   \
        t1 = localtime(&e1_tm.time);  \
        printf("[%s] 종료시각 => [%02d:%02d:%02d:%04d]\n",a,   \
            t1->tm_hour,t1->tm_min,t1->tm_sec,e1_tm.millitm);   \
        printf("[%s] 수행시간 => [%04d:%04d]\n",a, \
                ((e1_tm.time*1000+e1_tm.millitm) -    \
                 (s1_tm.time*1000+s1_tm.millitm))/1000,   \
                ((e1_tm.time*1000+e1_tm.millitm) -    \
                 (s1_tm.time*1000+s1_tm.millitm))%1000);  }

#define FSERVICE_START(fpl,a) {  \
        ftime(&s_tm);   \
        t = localtime(&s_tm.time);  \
        fprintf(fpl,"[%s] 시작시각 => [%02d:%02d:%02d:%04d]\n",a,    \
                t->tm_hour,t->tm_min,t->tm_sec,s_tm.millitm);   }
#define FSERVICE_END(fpl,a) {    \
        ftime(&e_tm);   \
        t = localtime(&e_tm.time);  \
        fprintf(fpl,"[%s] 종료시각 => [%02d:%02d:%02d:%04d]\n",a,   \
            t->tm_hour,t->tm_min,t->tm_sec,e_tm.millitm);   \
        fprintf(fpl,"[%s] 수행시간 => [%04d:%04d]\n",a, \
                ((e_tm.time*1000+e_tm.millitm) -    \
                 (s_tm.time*1000+s_tm.millitm))/1000,   \
                ((e_tm.time*1000+e_tm.millitm) -    \
                 (s_tm.time*1000+s_tm.millitm))%1000);  }
#define FTIMER_START(fpl,a) {  \
        ftime(&s1_tm);   \
        t1 = localtime(&s1_tm.time);  \
        fprintf(fpl,"[%s] 시작시각 => [%02d:%02d:%02d:%04d]\n",a,    \
                t1->tm_hour,t1->tm_min,t1->tm_sec,s1_tm.millitm);   }
#define FTIMER_END(fpl,a) {    \
        ftime(&e1_tm);   \
        t1 = localtime(&e1_tm.time);  \
        fprintf(fpl,"[%s] 종료시각 => [%02d:%02d:%02d:%04d]\n",a,   \
            t1->tm_hour,t1->tm_min,t1->tm_sec,e1_tm.millitm);   \
        fprintf(fpl,"[%s] 수행시간 => [%04d:%04d]\n",a, \
                ((e1_tm.time*1000+e1_tm.millitm) -    \
                 (s1_tm.time*1000+s1_tm.millitm))/1000,   \
                ((e1_tm.time*1000+e1_tm.millitm) -    \
                 (s1_tm.time*1000+s1_tm.millitm))%1000);  }
#define FPRINTF_VAR(fpl,a) {\
        fprintf(fpl,"[%s] [%ld]\n", a.arr, a.len); }
#define FPRINTF_LONG(fpl,a) {\
        fprintf(fpl,"[%ld]\n",      a); }
#define FPRINTF_DOUB(fpl,a) {\
        fprintf(fpl,"[%14.2f]\n",   a); }

#define TPRETURN(x,y) { \
    Fchg32((FBFR32 *)(transf), STATLIN, 0, (char *)(x),0); \
    tpreturn(TPSUCCESS, y, (char *)(transf), 0, 0); }

#define TPRETURN_ERROR(x,y) { \
    Fchg32((FBFR32 *)(transf), STATLIN, 0, (char *)(x),0); \
    tpreturn(TPFAIL, y, (char *)(transf), 0, 0); }

