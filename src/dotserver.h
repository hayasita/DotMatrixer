#ifndef dotserver_h
#define dotserver_h

#ifdef GLOBAL_VAL_DEF
#define GLOBAL
#else
#define GLOBAL extern
#endif

#include <Arduino.h>
#include <AsyncTCP.h>

#include <ArduinoJson.h>

void startWiFi(void);
void startWebserver(void);
void setWebhandle(void);


#undef GLOBAL
#endif
