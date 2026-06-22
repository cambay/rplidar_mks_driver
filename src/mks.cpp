#include "mks.h"
#include "commandLink.h"
#include <thread>
#include <vector>
#include <cmath>

#include <string>
#include "tools.h"



Motor::Motor(CommandLink& l) : deviceFunctions(l){
        std::cout << "Motor created on port:" << link.getPort() << "\n";
    }
    
void Motor::start() {
        std::cout << "Starting motor" << "\n"; 
        //link.sendCommand(uartMode, sizeof(uartMode));
        link.sendCommand(startServo, sizeof(startServo)); 
    }
void Motor::stop() {
        std::cout << "\n" << "Stopping motor on " << link.getPort();
        link.sendCommand(stopServo, sizeof(stopServo)); 
    }

void Motor::calibrate_Servo(){
        //std::cout << "Reading buffers" << "\n"; 
        std::cout << "Calibrating..." << "\n";
        link.sendCommand(calibrateServo, sizeof(calibrateServo)); 
        link.readCommand(0xE1); 
    }

int32_t Motor::getCurrentAngle(){

        auto currentAngle_raw = link.parseMKShex(6, 0xE0); 
        int32_t raw_angle = 
                static_cast<int32_t>(((uint32_t)currentAngle_raw[1] << 24) |
                          ((uint32_t)currentAngle_raw[2] << 16) |
                          ((uint32_t)currentAngle_raw[3] << 8)  |
                          ((uint32_t)currentAngle_raw[4]));
            float currentAngle = (static_cast<float>(raw_angle % 65536) * 360.0f) / 65536.0f;
            currentAngle = abs(currentAngle);
            std::cout << " Raw Angle: " << raw_angle << "ticks" << "\n"; 
            std::cout << " Current Angle: " << currentAngle << "deg" << "\n"; 
        return raw_angle; 
}

void Motor::moveToAngle(int32_t angle, char dir){
    const int stepsPerRev = 200; //# of steps per full rev (1.8deg motor)
    const int M_step = 16; //subdivisions  
    auto currentAngle1 = getCurrentAngle(); //int32_t 
    int32_t angleInTicks = angleToTicks(angle); 
    std::this_thread::sleep_for(25ms); 
    std::vector<uint8_t> goToCommand = {0xE0, 0xFD}; 
    uint8_t forwardSpeedByte = 0x08; //dir forward, gear 8 
    uint8_t reverseSpeedByte1 = 0x88; //dir reverse
    uint8_t direction{0}; 

    switch (dir)
    {
    case ('F'):
        direction = forwardSpeedByte; 
        break; 
    case ('R'):
        direction = reverseSpeedByte1; 
        break;
    default:
        //base case 
        break;
    }

    goToCommand.push_back(direction);
    int32_t delta = (int32_t)angleInTicks - currentAngle1; 
    uint32_t pulse = (delta * stepsPerRev * M_step) / 360; 
    uint8_t pulse_byte[4];
    pulse_byte[0] = (uint8_t)(pulse >> 24) & 0xFF;
    pulse_byte[1] = (uint8_t)(pulse >> 16) & 0xFF;
    pulse_byte[2] = (uint8_t)(pulse >> 8) & 0xFF;
    pulse_byte[3] = (uint8_t)(pulse & 0xFF);
    for(int j=0; j<4; j++){goToCommand.push_back(pulse_byte[j]);} 

    uint8_t tCHK{0}; 
    for(int j=0; j<7; j++){
        tCHK = tCHK + goToCommand[j]; 
    }
    tCHK = (tCHK % 256) & 0xFF; 
    goToCommand.push_back(tCHK); //append with checksum 
    link.sendCommand(goToCommand.data(), goToCommand.size());
    std::this_thread::sleep_for(250ms); 
    link.sendCommand(stopServo,sizeof(stopServo)); //stop servo once angle is reached
    std::this_thread::sleep_for(1000ms); 
}

void Motor::syncMotor(bool newScan, uint32_t desiredAngle, char direction){    
    moveToAngle(desiredAngle, direction); 

}

void Motor::gotoZero()
{
    int32_t currentAngle = getCurrentAngle();
    if (currentAngle == 0) {
        return;
    }
    if (currentAngle > 180) {
        moveToAngle(360, 'F');} 
    else {
        moveToAngle(0, 'R');}
}
