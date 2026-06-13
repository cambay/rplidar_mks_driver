// commandLink.h
#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <libserial/SerialPort.h>
#include "identifyDevices.h"

class CommandLink {
protected:
    uint8_t* command;
    BaudRate baud;
    size_t data_;
    SerialPort& serialDevice;
    std::string port;

public:
    CommandLink(SerialPort& c, std::string p, BaudRate b);

    const std::string& getPort() const;

    void sendCommand(uint8_t* command_, size_t length);
    int readCommand(uint8_t checkSum);
    void openPort();
    void closePort();
};

class deviceFunctions {
protected:
    CommandLink& link;

public:
    deviceFunctions(CommandLink& l);

    void read(uint8_t checkSum);
    void write(uint8_t* command, size_t c);
    void open();
    void close();
};