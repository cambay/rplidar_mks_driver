#pragma once
#include <iostream>
#include <string>
#include <libudev.h>
#include <libserial/SerialPort.h>
#include <string>

using namespace LibSerial; 
using namespace std::literals;
using LibSerial::BaudRate;  
using LibSerial::SerialPort;
extern SerialPort motor_Port;
extern SerialPort lidar_Port;

struct USBdevice {
    std::string serial;
    std::string devNode;
    uint32_t baud_rate;
};

extern USBdevice lidar;
extern USBdevice motor;

void identifyUSBDevices();