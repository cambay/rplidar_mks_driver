// commandLink.cpp
#include "commandLink.h"
#include <iostream>
#include <vector>

CommandLink::CommandLink(SerialPort& c, std::string p, BaudRate b)
    : serialDevice(c), port(p), baud(b) {}

const std::string& CommandLink::getPort() const {
    return port;
}
void CommandLink::sendCommand(uint8_t* command_, size_t length) {
    for (size_t j = 0; j < length; j++) {
        serialDevice.WriteByte(command_[j]);
    }
    std::cout << std::dec << "\n";
}

int CommandLink::readCommand(uint8_t checkSum) {
    std::vector<uint8_t> commandBuffer;
    char commandByte;

    while (commandBuffer.empty() || commandBuffer.back() != checkSum){
        serialDevice.ReadByte(commandByte);
        commandBuffer.push_back(static_cast<uint8_t>(commandByte));}
    for (size_t k = 0; k < commandBuffer.size(); k++) {
        std::cout << std::hex << int(commandBuffer[k]) << " ";}
    return (checkSum == 0xA1) ? 1 : 0;
}

void CommandLink::openPort() {
    try {
        serialDevice.Open(port);
        std::cout << "Connecting Serial port: " << port << "\n";
        serialDevice.SetBaudRate(baud);
    } 
    catch (const LibSerial::OpenFailed&) {
        std::cerr << "Serial failure";
        return;
    }
    std::cout << "Port open on: " << port << "\n";
}

void CommandLink::closePort() {
    serialDevice.Close();
    std::cout << "Closing Serial Port: " << port << "\n";
}

deviceFunctions::deviceFunctions(CommandLink& l)
    : link(l) {}

void deviceFunctions::read(uint8_t checkSum) {
    link.readCommand(checkSum);
}

void deviceFunctions::write(uint8_t* command, size_t c) {
    link.sendCommand(command, c);
}

void deviceFunctions::open() {
    link.openPort();
}

void deviceFunctions::close() {
    link.closePort();
}