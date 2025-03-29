
#include "SLUtils.h"

SLed myled1(8);

Button mybutton1(9);

struct StorageTester
{
    int a = 1;
    float b = 2.0f;
    bool c = false;
};

STORED_DATA(StorageTester, st);

void setup()
{

    Serial.begin(9600);

    Serial.println("Slu startup!");
    Serial.println(randomCompileSeedID);

    Serial.print("Addres of md");
    Serial.println(int(&_flash_slu_management_data));

    if (SluIsFirstRun())
    {
        // First run, initialize the data in eeprom
        Serial.println("First run!");

        Serial.println(st.data.a);
        Serial.println(st.data.b);
        Serial.println(st.data.c);
        st.data.a = 50;
        st.data.b = 50.0f;
        st.data.c = true;
        // st.save();
        SluSaveAllData();
    }
    else
    {
        Serial.println("Not first run!");

        Serial.println(st.data.a);
        Serial.println(st.data.b);
        Serial.println(st.data.c);
        st.data.a = 10;
        st.data.b = 20.0f;
        st.data.c = true;
        st.save();
    }
}

void loop()
{
}