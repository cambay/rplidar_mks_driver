#include "commandLink.h"
#include "identifyDevices.h"

class Motor : public deviceFunctions{
private:
    //Servo Commands 
    uint8_t uartMode[4] = {0xE0, 0x82, 0x02, 0x63}; //UART mode
    uint8_t calibrateServo[4] = {0xE0,0x80, 0x00, 0x60}; //must wait for calibration to complete first 
    uint8_t startServo[4] = {0xE0, 0xF6, 0x90, 0x66}; //starts 
    uint8_t Servotest[4] = {0xE0, 0xF6, 0x01, 0xD7}; //right now this command i have reverses the servo max speed
    uint8_t stopServo[3] = {0xE0, 0xF7, 0xD7}; //stops motor 

public:
    Motor(CommandLink& l);

    //functions 
    void start();
    void stop();
    void calibrate_Servo();
    void test();
};