#pragma once
#include <iostream>
#include <string>
#include <libudev.h>
#include <libserial/SerialPort.h>

#include <string>

struct USBdevice {
    std::string serial;
    std::string devNode;
    uint32_t baud_rate;
};

extern USBdevice lidar;
extern USBdevice motor;

void identifyUSBDevices();