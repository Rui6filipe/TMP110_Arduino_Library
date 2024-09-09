#include "TMP110.h"

#define TEMP_RESULT 0x00
#define CONFIGURATION 0x01
#define TLOW_LIMIT 0x02
#define THIGH_LIMIT 0x03
#define RESOLUTION 0.0625
#define RESET 0x06
#define ALERT_CAUSE 0x19


bool TMP110::begin(uint8_t Address, TwoWire &wirePort = Wire){

    _i2cPort = WirePort // Saves I2C Port
    _address = (Address > 3) ? (Address + 0x40) : (Address + 0x44); // Saves target adress

    // Test if I2C device is online
    _i2cPort->begin();
    _i2cPort->beginTransmission(_address); 
    uint8_t error = _i2cPort->endTransmission();

    // If I2C did not work or the adress specified is wrong return false
    if (error != 0 || Address < 0 || Address > 7){
        return false;
    } 
    return true;
}


int16_t TMP110::readRegister(uint8_t registerAddress){
    
    // Read MSB and LSB from specified register
    _i2cPort->requestFrom(registerAddress, (uint8_t)2);

    if (_i2cPort->available() >= 2) { // If data available read from register
        uint8_t msb = _i2cPort->read();
        uint8_t lsb = _i2cPort->read();
        uint16_t byte = (msb << 8) | lsb; 
        return byte; 
    } else {
        return -1; // If data not available return error code
    }
}


uint8_t TMP110::writeRegister(uint8_t registerAddress, uint16_t byte){
    
    // Write MSB and LSB to specified register
    _i2cPort->beginTransmission(_address);
    _i2cPort->write(registerAddress);
    _i2cPort->write(byte >> 8);
    _i2cPort->write(byte & 0xFF);
    uint8_t error = _i2cPort->endTransmission();

    return error
}


bool TMP110::checkPolarityBit(){

    uint16_t config = readRegister(CONFIGURATION); // Read the configuration register
       
    bool polarity = (config & 0x400) != 0; // Check polarity bit

    return polarity
}


bool TMP110::setConversionRate(uint8_t rate){

    uint16_t config = readRegister(CONFIGURATION); // Read the configuration register

    config &= 0xFF3F; // Clear conversion rate bits
    config |= (rate << 6); // Set them to the desired

    uint8_t error = writeRegister(CONFIGURATION, config); // Write to config register

    if (error != 0 || rate < 0 || rate > 4){
        return false;
    } 
    return true;
}


bool TMP110::setFault(uint8_t fault){

    uint16_t config = readRegister(CONFIGURATION); // Read the configuration register

    config &= 0xE7FF; // Clear fault bits
    config |= (fault << 11); // Set them to the desired

    uint8_t error = writeRegister(CONFIGURATION, config); // Write to config register

    if (error!= 0 || fault < 0 || fault > 4){
        return false;
    } 
    return true;
}


bool TMP110::setPolarity(uint8_t polarity){

    uint16_t config = readRegister(CONFIGURATION); // Read the configuration register
       
    config &= 0xFBFF; // Clear polarity bit
    config |= (polarity << 10); // Set it to the desired

    uint8_t error = writeRegister(CONFIGURATION, config); // Write to config register

    if (error!= 0 || polarity < 0 || polarity > 1){
        return false;
    } 
    return true;
}


bool TMP110::setAlertMode(uint8_t mode){

    uint16_t config = readRegister(CONFIGURATION); // Read the configuration register

    config &= 0xFDFF; // Clear alert mode bit
    config |= (mode << 9); // Set it to the desired

    uint8_t error = writeRegister(CONFIGURATION, config); // Write to config register

    if (error!= 0 || mode < 0 || mode > 1){
        return false;
    } 
    return true;
}


bool TMP110::setExtendedMode(uint8_t mode, float high, float low){

    uint16_t config = readRegister(CONFIGURATION); // Read the configuration register

    config &= 0xFFEF; // Clear extended mode bit
    config |= (mode << 4); // Set it to the desired

    uint8_t error = writeRegister(CONFIGURATION, config); // Write to config register

    // Got to update the value of the limit registers
    bool high_error = setHighTemp(high);
    bool low_error = setLowTemp(low);

    if (error!= 0 || mode < 0 || mode > 1 || high_error == 0 || low_error == 0){
        return false;
    } 
    return true;    
    
} 


bool TMP110::setHighTemp(float high){ 

    bool mode = (readRegister(CONFIGURATION) & 0x10) != 0; // In normal mode, mode var is False

    int16_t limit = high/RESOLUTION; // Read high temperature limit 
    uint16_t config = readRegister(THIGH_LIMIT); // Read the HighTemp register

    if (mode==0){ // 12 bit mode
        config &= 0x000F; // Clear temperature bits
        config |= (limit << 4); // Sets them to the desired
    } 
    else{ // 13 bit mode
        config &= 0x0007; // Clear temperature bits
        config |= (limit << 3); // Sets them to the desired
    }

    uint8_t error = writeRegister(THIGH_LIMIT, config); // Write to HighTemp register

    if (error!= 0){
        return false;
    } 
    return true;  
}


bool TMP110::setLowTemp(float low){

    bool mode = (readRegister(CONFIGURATION) & 0x10) != 0; // In normal mode, mode var is False

    int16_t limit = low/RESOLUTION; // Read high temperature limit 
    uint16_t config = readRegister(TLOW_LIMIT); // Read the HighTemp register

    if (mode==0){ // 12 bit mode
        config &= 0x000F; // Clear temperature bits
        config |= (limit << 4); // Sets them to the desired
    } 
    else{ // 13 bit mode
        config &= 0x0007; // Clear temperature bits
        config |= (limit << 3); // Sets them to the desired
    }

    uint8_t error = writeRegister(TLOW_LIMIT, config); // Write to HighTemp register

    if (error!= 0){
        return false;
    } 
    return true;  
}


bool TMP110::reset(){

    _i2cPort->beginTransmission(_address);
    _i2cPort->write(TEMP_RESULT); // Transmit general call address (which is the same as the temperature register address)
    _i2cPort->write(RESET); // Transmit reset code
    uint8_t error = _i2cPort->endTransmission();

    if (error!= 0){
        return false;
    } 
    return true;  
}


bool TMP110::checkAlert(){

    uint16_t config = readRegister(CONFIGURATION); // Read config register
    bool alertFlag = (config & 0x20) != 0; // Check alert bit
    bool polarity = checkPolarityBit(); // Check polarity bit

    // If polarity is 0 and alert bit is 1 the alert is not active
    // If polarity is 1 and alert bit is 1 the alert is active
    alertFlag *= polarity; 

    return alertFlag;  
}


uint8_t TMP110::alertCause(){

    _i2cPort->beginTransmission(_address);
    _i2cPort->write(ALERT_CAUSE); // Transmit alert response address
    _i2cPort->endTransmission(false);

    _i2cPort->requestFrom(_address, (uint8_t)1); // Request 1 byte of data
    if (_i2cPort->available()) { 
        uint8_t alert = _i2cPort->read(); // Read the byte from the register
        bool alertBit = (alert & 0x01) != 0; // Check alert bit
        bool polarity = checkPolarityBit(); // Check polarity bit
        alertBit = (polarity ^ alertBit) == 0; // XOR
        return alertBit;
    } else{
        return -1; // If data not available return error code
    }
}


bool TMP110::shutdown(){
        
    uint16_t config = readRegister(CONFIGURATION); // Read the configuration register

    config |= 0x100; // Set shutdown bit to 1

    uint8_t error = writeRegister(CONFIGURATION, config); // Write to config register

    if (error!= 0){
        return false;
    } 
    return true;
}


bool TMP110::oneShot(){

    bool shut = shutdown(); // Swhitch to shutdown mode

    uint16_t config = readRegister(CONFIGURATION); // Read the configuration register

    config |= 0x8000; // Set one shot bit to 1

    uint8_t error = writeRegister(CONFIGURATION, config); // Write to config register

    if (error!= 0 || shut == 0){
        return false;
    } 
    return true;
}


bool TMP110::continuousConversion(){

    uint16_t config = readRegister(CONFIGURATION); // Read the configuration register

    config &= 0xFEFF; // Set shutdown bit to 0

    uint8_t error = writeRegister(CONFIGURATION, config); // Write to config register

    if (error!= 0){
        return false;
    } 
    return true;
}


float TMP110::readTemperature(){

    bool mode = (readRegister(CONFIGURATION) & 0x10) != 0; // In normal mode, mode var is False

    float tempC = 0;

    if (mode == 0){ // 12 bit mode

        uint16_t temp = (readRegister(TEMP_RESULT) >> 4); // Read temperature bits
        
        if ((temp & 0x800) == 0) { 
            tempC = temp * RESOLUTION; // If temp is positive multiply by resolution
        } else{
            temp = (~temp + 1) & 0xFFF; // If temp is negative do 2's complement, clear the 4 upper bits
            tempC = - temp * RESOLUTION; // Multiply by resolution and put negative sign
        }
    } 
    else{ // 13 bit mode

        uint16_t temp = (readRegister(TEMP_RESULT) >> 3); // Read temperature bits
        
        if ((temp & 0x1000) == 0) { 
            tempC = temp * RESOLUTION; // If temp is positive multiply by resolution
        } else{
            temp = (~temp + 1) & 0x1FFF; // If temp is negative do 2's complement, clear the 3 upper bits
            tempC = - temp * RESOLUTION; // Multiply by resolution and put negative sign
        }
    }
    
    return tempC;
}