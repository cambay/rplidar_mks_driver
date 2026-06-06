//#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <identifyDevices.h>

//Current examples deliberately exclude Lidar to showcase underlying concepts 
//

using namespace LibSerial; 
using namespace std::literals;
using LibSerial::SerialPort;
using LibSerial::BaudRate;
SerialPort motor_Port; 

class CommandLink{
    private:
    uint8_t* command; 
    size_t data_; //size of command
    SerialPort& serialport; //SerialPort reference 

    public:
    CommandLink(SerialPort& p) : serialport(p) {}

    void sendCommand(uint8_t* command_, size_t length){

        for(int j = 0; j < length; j++){
            serialport.WriteByte(command_[j]);}

        std::cout << std::dec << "\n";
    }
    int readCommand(uint8_t checkSum){
        std::vector<uint8_t> commandBuffer = {}; 
        char commandByte; 
        while (commandBuffer.empty() || commandBuffer.back() != checkSum) {
            serialport.ReadByte(commandByte); 
            commandBuffer.push_back(static_cast<uint8_t>(commandByte)); 
            if(commandBuffer.back() == checkSum) {
                break;}
            else{
                continue;}
            }
            for(int k = 0; k < commandBuffer.size(); k++){
            std::cout << std::hex << int(commandBuffer[k]) << " ";}
            return (checkSum == 0xA1) ? 1 : 0;
    }
};

class Motor {
private:
    std::string port;
    uint32_t baudRate;
    CommandLink& link;
    //Servo Commands 
    uint8_t uartMode[4] = {0xE0, 0x82, 0x02, 0x63}; //UART mode
    uint8_t calibrateServo[4] = {0xE0,0x80, 0x00, 0x60}; //must wait for calibration to complete first 
    uint8_t startServo[4] = {0xE0, 0xF6, 0x90, 0x66}; //starts 
    uint8_t stopServo[3] = {0xE0, 0xF7, 0xD7}; //stops motor 
    //End commands 

public:
    
    Motor(CommandLink& l, std::string p, uint32_t baud) : link(l), port(p), baudRate(baud){
        std::cout << "Motor created on " << p << "\n";
    }
    
    void start() {
        std::cout << "Starting motor" << "\n"; 
        //link.sendCommand(uartMode, sizeof(uartMode));
        link.sendCommand(startServo, sizeof(startServo)); 
    }
    void stop() {
        std::cout << "Stopping motor on " << port << "\n";
        link.sendCommand(stopServo, sizeof(stopServo)); 
    }

    void calibrate_Servo(){
        //std::cout << "Reading buffers" << "\n"; 
        std::cout << "Calibrating..." << "\n";
        link.sendCommand(calibrateServo, sizeof(calibrateServo)); 
        link.readCommand(0xE1); 

    }
};


///Example:
int main(){
    
    //from identifyDevices 
    identifyUSBDevices(); 
    
    //create instance of motor link on the serial port 
    CommandLink motor_link(motor_Port); 
    Motor servo(motor_link, motor.devNode, 38400);



    try{

    //To be added to class in next iteration alongside handling of lidar + sychronization 
    motor_Port.Open(motor.devNode); 
    std::cout << "Connecting Serial port.";
    motor_Port.SetBaudRate(BaudRate::BAUD_38400);

    for(int i=0; i<10; ++i){
        std::cout << "." << std::flush ;
        motor_Port.WriteByte('a');
        std::this_thread::sleep_for(500ms);}
        std::cout << "\n"; 


    //start by calibrating servo 
    servo.calibrate_Servo(); 
    std::cout << "Calibration complete." << "\n"; 
    std::this_thread::sleep_for(200ms); 

    //test the start & stop function
    servo.start(); 
    for(int i=0; i<10; ++i){
        std::this_thread::sleep_for(300ms);}
    servo.stop(); 

    }
    catch (const LibSerial::OpenFailed& e){
        std::cerr << "Serial failure"; // 
        return EXIT_FAILURE; 
    }



    std::cout << "\n" << "Try satisfied with no catch, closing serial port." << std::endl;
    motor_Port.Close();
    return EXIT_SUCCESS;


}