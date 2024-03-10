#ifndef dotserver_h
#define dotserver_h

#ifdef GLOBAL_VAL_DEF
#define GLOBAL
#else
#define GLOBAL extern
#endif


void startWiFi(void);
void startWebserver(void);
void setWebhandle(void);


#undef GLOBAL
#endif
