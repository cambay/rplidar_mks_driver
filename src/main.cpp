#include <iostream>
#include <string>
#include <libudev.h>
#include <libserial/SerialPort.h>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include "identifyDevices.h"


using namespace LibSerial; 
using namespace std::literals;
using LibSerial::BaudRate;  
using LibSerial::SerialPort;
SerialPort motor_Port; 
SerialPort lidar_Port;

class CommandLink{
    protected:
    uint8_t* command;
    BaudRate baud; 
    size_t data_; //size of command
    SerialPort& serialDevice; //SerialPort device reference (initialized elsewhere)
    std::string port; //port initilizsation 

    public:
    CommandLink(SerialPort& c, std::string p, BaudRate b) : serialDevice(c), port(p), baud(b) {}

    //To avoid having to pass all same params into other class instantiations 
    const std::string& getPort() const {
        return port; 
    }

    void sendCommand(uint8_t* command_, size_t length){

        for(int j = 0; j < length; j++){
            serialDevice.WriteByte(command_[j]);}

        std::cout << std::dec << "\n";
    }
    int readCommand(uint8_t checkSum){
        std::vector<uint8_t> commandBuffer = {}; 
        char commandByte; 
        while (commandBuffer.empty() || commandBuffer.back() != checkSum) {
            serialDevice.ReadByte(commandByte); 
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

    void openPort(){
        try{
        serialDevice.Open(port); 
        std::cout << "Connecting Serial port:" << port << "\n"; 
        serialDevice.SetBaudRate(baud);
            }
        catch(const LibSerial::OpenFailed& e){
        std::cerr << "Serial failure"; 
        return;
            }
        std::cout << "Port open on:" << port << "\n";}
    void closePort(){
        serialDevice.Close();
        std::cout << "Closing Serial Port:" << port << "\n";}
};


class deviceFunctions{

    protected:
    CommandLink& link; 
    
    public:
    deviceFunctions(CommandLink& l) : link(l){}

    void read(uint8_t checkSum){
        link.readCommand(checkSum);
    }
    void write(uint8_t* command, size_t c){
        link.sendCommand(command, c);}
    void open(){
        link.openPort();}
    void close(){
        link.closePort();}

}; 


class Lidar: public deviceFunctions{

    private: 
    //Lidar commands
    uint8_t get_health[2] = {0xA5,0x52}; 
    uint8_t start_scan[2] = {0xA5,0x20}; 
    uint8_t reset_scan[2] = {0xA5,0x40}; 
    uint8_t stop_scan[2]  = {0xA5,0x25}; 
    public:
    
    Lidar(CommandLink& l) : deviceFunctions(l){
        std::cout << "Lidar created on port:" << link.getPort() << "\n"; 
    }
    
    void start(){
        std::cout << "Starting lidar" << "\n"; 
        link.sendCommand(start_scan, sizeof(start_scan)); 
    }

    void getHealth(){
        std::cout << "Getting health: " << "\n" ;
        link.sendCommand(get_health,sizeof(get_health)); 
    }
    void stop(){
        std::cout << "Stopping lidar" << "\n"; 
        link.sendCommand(stop_scan, sizeof(stop_scan)); 
    }


};

class Motor : public deviceFunctions{
private:
    //MKS Commands 
    uint8_t uartMode[4] = {0xE0, 0x82, 0x02, 0x63}; 
    uint8_t calibrateServo[4] = {0xE0,0x80, 0x00, 0x60}; 
    uint8_t startServo[4] = {0xE0, 0xF6, 0x90, 0x66}; 
    uint8_t stopServo[3] = {0xE0, 0xF7, 0xD7}; 


public:
    Motor(CommandLink& l) : deviceFunctions(l){
        std::cout << "Motor created on port:" << link.getPort() << "\n";
    }
    
    void start() {
        std::cout << "Starting motor" << "\n"; 
        link.sendCommand(startServo, sizeof(startServo)); 
    }
    void stop() {
        std::cout << "Stopping motor on " << link.getPort() << "\n";
        link.sendCommand(stopServo, sizeof(stopServo)); 
    }

    void calibrate_Servo(){
        std::cout << "Calibrating..." << "\n";
        link.sendCommand(calibrateServo, sizeof(calibrateServo)); 
        link.readCommand(0xE1); 
        std::cout << "Calibration complete" << "\n"; 

    }


};


int main(){

    identifyUSBDevices(); 
    
    CommandLink lidar_link(lidar_Port, lidar.devNode, BaudRate::BAUD_460800);
    deviceFunctions device(lidar_link); 
    Lidar c1(lidar_link); //rplidar c1

    CommandLink motor_link(motor_Port, motor.devNode, BaudRate::BAUD_38400); 
    deviceFunctions devices(motor_link); 
    Motor mks(motor_link); //mks servo

    //initialize the tty terminal to raw for the lidar & servo 
    std::string stty_mks = "stty -F " + motor.devNode + " 38400 raw -echo";
    std::system(stty_mks.c_str());
    std::string stty_c1 = "stty -F " + lidar.devNode + " 460800 raw -echo";
    std::system(stty_c1.c_str());
    

    //open
    mks.open(); 
    c1.open(); 

    //start
    mks.start(); 
    c1.start(); 

    //Example loop that reads out lidar values in hexadecimal for 8s
    for(int i=0; i<10; ++i){
    std::this_thread::sleep_for(800ms);
    lidar_link.readCommand(0x40);
    }
 
    mks.stop(); 
    c1.stop(); 
    mks.close(); 
    c1.close(); 


}