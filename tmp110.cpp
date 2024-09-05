#include "TMP110.h"

#define TEMP_RESULT 0x00
#define CONFIGURATION 0x01
#define TLOW_LIMIT 0x02
#define THIGH_LIMIT 0x03
#define RESOLUTION 0.0625

bool TMP110::begin(uint8_t Address, TwoWire &wirePort = Wire){

    _i2cPort = WirePort // Saves I2C Port

    _address = (Address > 3) ? (Address + 0x40) : (Address + 0x44); // Saves target adress

    // Test if I2C device is online
    _i2cPort->beginTransmission(_address); 
    uint8_t error = _i2cPort->endTransmission();

    // If I2C did not work or the adress specified is wrong trow an error
    if (error != 0 || Address < 0 || Address > 7){
        return false;
    } 
    return true;
}


bool TMP110::readRegister(uint8_t registerAddress){
    
    // Read MSB and LSB from specified register
    _i2cPort->requestFrom(registerAddress, 2);
    uint8_t msb = _i2cPort->read();
    uint8_t lsb = _i2cPort->read();

    // Return register value
    uint16_t byte = (msb << 8) | lsb; 
    return byte
}


bool TMP110::setConversionRate(uint8_t rate){

    // Read the configuration register and extract msb and lsb
    uint16_t config = readRegister(CONFIGURATION);
    uint8_t msb = (config >> 8) & 0xFF;
    uint8_t lsb = config & 0xFF;        

    lsb &= 0x3F; // Clear conversion rate bits
    lsb |= (rate << 6); // Set them to the desired

    // Write to register
    _i2cPort->beginTransmission(_address);
    _i2cPort->write(CONFIGURATION);
    _i2cPort->write(msb);
    _i2cPort->write(lsb);
    uint8_t error = _i2cPort->endTransmission();

    if (error!= 0 || rate < 0 || rate > 4){
        return false;
    } 
    return true;
}


bool TMP110::setFault(uint8_t fault){

    // Read the fault register and extract msb and lsb
    uint16_t config = readRegister(CONFIGURATION);
    uint8_t msb = (config >> 8) & 0xFF;
    uint8_t lsb = config & 0xFF;        

    msb &= 0xE7; // Clear fault bits
    msb |= (fault << 3); // Set them to the desired

    // Write to register
    _i2cPort->beginTransmission(_address);
    _i2cPort->write(CONFIGURATION);
    _i2cPort->write(msb);
    _i2cPort->write(lsb);
    uint8_t error = _i2cPort->endTransmission();

    if (error!= 0 || fault < 0 || fault > 4){
        return false;
    } 
    return true;
}


bool TMP110::setPolarity(uint8_t polarity){

    // Read the configuration register and extract msb and lsb
    uint16_t config = readRegister(CONFIGURATION);
    uint8_t msb = (config >> 8) & 0xFF;
    uint8_t lsb = config & 0xFF;        

    msb &= 0xFB; // Clear polarity bit
    msb |= (polarity << 2); // Set it to the desired

    // Write to register
    _i2cPort->beginTransmission(_address);
    _i2cPort->write(CONFIGURATION);
    _i2cPort->write(msb);
    _i2cPort->write(lsb);
    uint8_t error = _i2cPort->endTransmission();

    if (error!= 0 || polarity < 0 || polarity > 1){
        return false;
    } 
    return true;
}


bool TMP110::setAlertMode(uint8_t mode){

    // Read the configuration register and extract msb and lsb
    uint16_t config = readRegister(CONFIGURATION);
    uint8_t msb = (config >> 8) & 0xFF;
    uint8_t lsb = config & 0xFF;        

    msb &= 0xFD; // Clear alert mode bit
    msb |= (mode << 1); // Set it to the desired

    // Write to register
    _i2cPort->beginTransmission(_address);
    _i2cPort->write(CONFIGURATION);
    _i2cPort->write(msb);
    _i2cPort->write(lsb);
    uint8_t error = _i2cPort->endTransmission();

    if (error!= 0 || mode < 0 || mode > 1){
        return false;
    } 
    return true;
}


bool TMP110::setExtendedMode(uint8_t mode){

    // Read the configuration register and extract msb and lsb
    uint16_t config = readRegister(CONFIGURATION);
    uint8_t msb = (config >> 8) & 0xFF;
    uint8_t lsb = config & 0xFF;        

    lsb &= 0xEF; // Clear extendedMode bits
    lsb |= (mode << 4); // Set them to the desired

    // Write to register
    _i2cPort->beginTransmission(_address);
    _i2cPort->write(CONFIGURATION);
    _i2cPort->write(msb);
    _i2cPort->write(lsb);
    uint8_t error = _i2cPort->endTransmission();

    if (error!= 0 || mode < 0 || mode > 1){
        return false;
    } 
    return true;    
    
    // Stuff to change other registers is missing!!!!!!!!!!!!!
} 


bool TMP110::setHighTemp(float high){

    int16_t limit = high/RESOLUTION;
    uint8_t limit_msb = (limit & 0xFF00) >> 8;
    uint8_t limit_lsb = limit & 0xFF;

    // Read the configuration Temperature High Limit and extract msb and lsb
    uint16_t config = readRegister(THIGH_LIMIT);
    uint8_t msb = (config >> 8) & 0xFF;
    uint8_t lsb = config & 0xFF;  

    msb &= 0x00; // Clear msb
    msb |= limit_msb; // Set it to the desired
    lsb &= 0xF8; // Clear bits in lsb
    lsb |= (limit_lsb << 3); // Set them to the desired

    _i2cPort->beginTransmission(_address);
    _i2cPort->write(THIGH_LIMIT);
    _i2cPort->write(msb);
    _i2cPort->write(lsb);
    uint8_t error = _i2cPort->endTransmission();



}


bool TMP110::setLowTemp(float low){


}




float TMP110::readTemperature(){



}