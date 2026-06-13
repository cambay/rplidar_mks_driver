#include "mks.h"
#include "commandLink.h"


Motor::Motor(CommandLink& l) : deviceFunctions(l){
        std::cout << "Motor created on port:" << link.getPort() << "\n";
    }
    
void Motor::start() {
        std::cout << "Starting motor" << "\n"; 
        //link.sendCommand(uartMode, sizeof(uartMode));
        link.sendCommand(startServo, sizeof(startServo)); 
    }
void Motor::stop() {
        //std::cout << "Stopping motor on " << link.getPort() << "\n";
        link.sendCommand(stopServo, sizeof(stopServo)); 
    }

void Motor::calibrate_Servo(){
        //std::cout << "Reading buffers" << "\n"; 
        std::cout << "Calibrating..." << "\n";
        link.sendCommand(calibrateServo, sizeof(calibrateServo)); 
        link.readCommand(0xE1); 
    }
void Motor::test(){
        link.sendCommand(Servotest, sizeof(Servotest)); 
    }
