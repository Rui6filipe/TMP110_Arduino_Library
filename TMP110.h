#ifndef TMP110_h
#define TMP110_h

#include <stdint.h>
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
        True - no error
        False - error
        */
        bool begin(uint8_t Address, TwoWire &wirePort = Wire); 
        
        float readTemperature(); // Reads temperature in C

        // Performs a one shot conversion. Automatically switches to shutdown mode
        // True - no error
        // False - error
        bool oneShot(); 

        // Switches to shutdown mode
        // True - no error
        // False - error
        bool shutdown(); 

        // Switches to continuous conversion
        // True - no error
        // False - error
        bool continuousConversion();

        // Sets the alarm high temperature limit in C
        // True - no error
        // False - error
        bool setHighTemp(float high); 

        // Sets the alarm low temperature limit in C
        // True - no error
        // False - error
        bool setLowTemp(float low); 
        
        // Enables or disables Extended mode. Needs to take in new high and low temperature limits
        // 0 - Disable (temperature 12bits)
        // 1 - Enable (temperature 13bits)
        bool setExtendedMode(uint8_t mode, float high, float low); 

        // Changes conversion rate
        // 0 - 0.25Hz
        // 1 - 1Hz
        // 2 - 4Hz
        // 3 - 8Hz
        bool setConversionRate(uint8_t rate); 

        // Resets internal registers
        // True - no error
        // False - error
        bool reset(); 

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
        bool setAlertMode(uint8_t mode);  

        // Checks the state of the Alert flag, teking into account the polarity bit
        // True - alert active
        // False - alert not active
        bool checkAlert(); 

        // Returns the cause of the Alert
        // False - Low Limit
        // True - High Limit
        // 255 - error
        uint8_t alertCause(); 

        
    private:

        TwoWire *_i2cPort; // I2C communication port

        int _address; // Address of TMP110

        int16_t readRegister(uint8_t registerAddress); // Reads a register

        uint8_t writeRegister(uint8_t registerAddress); // Writes to a register

        // Reads polarity bit
        // False - 0
        // True - 1
        bool checkPolarityBit(); 

};

#endif