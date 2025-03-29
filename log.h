#ifndef SLU_LOG_H
#define SLU_LOG_H

#if defined(__USE_SERIAL__)
#define InitSerial() Serial.begin(9600)
#define println(...) Serial.println(__VA_ARGS__)
#define print(...) Serial.print(__VA_ARGS__)
# else
#define InitSerial()
#define println(...)
#define print(...)
#endif

#endif



