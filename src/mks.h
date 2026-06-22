#include "commandLink.h"
#include "identifyDevices.h"

class Motor : public deviceFunctions{
private:
    //SEND Commands 
    uint8_t uartMode[4] = {0xE0, 0x82, 0x02, 0x63}; 
    uint8_t calibrateServo[4] = {0xE0,0x80, 0x00, 0x60}; 
    uint8_t startServo[4] = {0xE0, 0xF6, 0x90, 0x66}; 
    uint8_t forwardSlow[4] = {0xE0, 0xF6, 0x01, 0xD7};
    uint8_t reverseSlow[4] = {0xE0, 0xF6, 0x81, 0x57}; 
    uint8_t stopServo[3] = {0xE0, 0xF7, 0xD7};
    uint8_t M_Step_16[4] = {0xE0, 0x84, 0x10, 0x74}; 
    uint8_t M_Step_32[4] = {0xE0, 0x84, 0x20, 0x84}; 
    uint8_t goToZero[4] = {0xE0, 0x91, 0x00, 0x71};
    uint8_t go180[8] = {0xE0, 0xFD, 0x02, 0x00, 0x00, 0x06, 0x40, 0x31};
    uint8_t motorCW[4] = {0xE0, 0x86, 0x00, 0x66}; 
    uint8_t motorCCW[4] = {0xE0, 0x86, 0x01, 0x67};
    
    //Send command with 32bit return values 
    uint8_t readAngle[3] = {0xE0, 0x36, 0x16}; //Returns e0 , angle(uint32_t),rCHK.
                                               //uint32_t can be:  0~65535 for 1 full rotation 
public:
    Motor(CommandLink& l);
    void start();
    void stop();
    void calibrate_Servo();
    int32_t getCurrentAngle(); 
    void moveToAngle(int32_t angle, char dir); 
    void gotoZero(); 
    void syncMotor(bool newScan, uint32_t desiredAngle, char direction);

};