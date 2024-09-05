#ifndef TMP110_h
#define TMP110_h

#include <stdint.h>
#include <Arduino.h>
#include <Wire.h>


class TMP110{

    public:

		/*
        Sets I2C device address: 
        0 - TMP110D GND
        1 - TMP110D V+
        2 - TMP110D SDA
        3 - TMP110D SCL
        4 - TMP110D0 
        5 - TMP110D1 
        6 - TMP110D2 
        7 - TMP110D3
        Sets the I2C bus (Wire by default)  
        */
        bool begin(uint8_t Address, TwoWire &wirePort = Wire); 
        
        float readTemperature(); // Reads temperature 

        bool oneShot(); // Performs a one shot conversion

        bool continuousConversion(); // Switches to continuous conversion

        bool setHighTemp(int16_t high); // Sets the alarm high temperature limit

        bool setLowTemp(int16_t low); // Sets the alarm low temperature limit
        
        // Enables or disables Extended mode
        // 0 - Disabled (temperature 12bits)
        // 1 - Enabled (temperature 13bits)
        bool setExtendedMode(uint8_t mode); 

        // Changes conversion rate
        // 0 - 0.25Hz
        // 1 - 1Hz
        // 2 - 4Hz
        // 3 - 8Hz
        bool setConversionRate(uint8_t rate); 

        bool reset(); // Resets internal registers

        // Number of consecutive conversions for which the Alert condition exists before Alert
        // 0 - 1 fault
        // 1 - 2 fault
        // 2 - 4 fault
        // 3 - 6 fault
        bool setFault(uint8_t fault);

        // Sets the polarity of Alert
        // 0 - Alert is active low
        // 1 - Alert is active high
        bool setPolarity(uint8_t polarity); 

        // Sets Alert mode
        // 0 - Comparator mode
        // 1 - Alert mode
        bool setAlarmMode(uint8_t mode);  

        bool checkAlert(); // Checks the state of the Alert register

        // Returns the cause of the Alert
        // 0 - Too low temperature
        // 1 - Too high temperature
        bool alertCause(); 

        
    private:

        TwoWire *_i2cPort; // I2C communication port

        int _address; // Address of Temperature sensor

        int16_t readRegister(); // Reads temperature register

};

#endif