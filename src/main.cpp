#include <iostream>
#include <string>
#include <libudev.h>
#include <libserial/SerialPort.h>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <iomanip>
#include <cmath>

//headers
#include "commandLink.h"
#include "mks.h"
#include "rplidar.h"
#include "identifyDevices.h"

SerialPort motor_Port;
SerialPort lidar_Port;


//set stty handler 
void setRaw(std::string mks, std::string c1){
    std::string stty_mks = "stty -F " + mks + " 38400 raw -echo";
    std::system(stty_mks.c_str());
    std::string stty_c1 = "stty -F " + c1 + " 460800 raw -echo";
    std::system(stty_c1.c_str());
}

 
int main(){

    //find correct port devices are on
    identifyUSBDevices(); 

    //lidar initialization 
    CommandLink lidar_link(lidar_Port, lidar.devNode, BaudRate::BAUD_460800);
    deviceFunctions device(lidar_link); 
    Lidar c1(lidar_link); 

    //servo initialization 
    CommandLink motor_link(motor_Port, motor.devNode, BaudRate::BAUD_38400); 
    deviceFunctions devices(motor_link); 
    Motor mks(motor_link);

    mks.open(); 
    c1.open(); 
    setRaw(motor.devNode, lidar.devNode);
     
    
    for(int i=0; i<10; ++i){
        std::cout << "." << std::flush ;
        std::this_thread::sleep_for(500ms);
        }
    std::cout << "\n"; 


//mks.start(); 
c1.start();
std::this_thread::sleep_for(500ms);

c1.parseHex(0xA5, 0x5A); //parses hex formatted for start_scan
    
//mks.stop(); 
c1.stop(); 
std::cout << "\n" << "Try satisfied with no catch, closing serial port." << std::endl;
mks.close(); 
c1.close();

}

