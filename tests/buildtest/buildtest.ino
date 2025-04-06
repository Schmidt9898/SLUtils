
#define __USE_SERIAL__

#include "SLUtils.h"
#include "assert.h"
SluLed led_green(A1);
SluLed led_red(A0);


void abort()
{
    led_red.turn(true);
    while (1)
    {
        delay(100);
    }
};

SluLed myled1(8);

Button mybutton1(9);

struct StorageTester
{
    int int_value = int(randomCompileSeedID);
    float float_value = float(randomCompileSeedID);
    bool bool_value = randomCompileSeedID % 2 == 0;
};

STORED_DATA(StorageTester, st);



void setup()
{

    InitSerial();

    println("Slu startup!");
    //delay(2000);
    println("Start test");


    assert_eq(SluIsFirstRun() == true); // Check if this is the first run
    assert_eq(SluIsFirstRun() == false); // should return false after first call

    assert_eq(st.data.int_value != int(randomCompileSeedID));
    assert_eq(st.data.float_value != float(randomCompileSeedID));
    // assert_eq(st.data.bool_value != (randomCompileSeedID % 2 == 0)); coinflip

    st.data.int_value = int(randomCompileSeedID);
    st.data.float_value = float(randomCompileSeedID);
    st.data.bool_value = (randomCompileSeedID % 2 == 0);

    SluSaveAllData();
    st.load(); // Load the data from EEPROM

    assert_eq(st.data.int_value == int(randomCompileSeedID));
    assert_eq(st.data.float_value == float(randomCompileSeedID));
    assert_eq(st.data.bool_value == (randomCompileSeedID % 2 == 0));


    led_green.turn(true);
    println("All tests passed!");
}

void loop()
{
}