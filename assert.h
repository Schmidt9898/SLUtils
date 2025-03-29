#ifndef _ASSERT_H
#define _ASSERT_H

#include "log.h"

void abort();

#define assert_eq(cond) \
    if(!(cond)){\
    print(F("Assert failed: ")); \
    println(F(#cond));\
    abort();}

#endif


