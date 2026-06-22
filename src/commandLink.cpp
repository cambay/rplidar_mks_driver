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

std::vector<uint8_t> CommandLink::returnVectorHex(uint8_t responseSize){
    std::vector<uint8_t> commandBuffer;
    char commandByte;
    
    while (commandBuffer.empty() || commandBuffer.size() != responseSize){
        serialDevice.ReadByte(commandByte);
        commandBuffer.push_back(static_cast<uint8_t>(commandByte));}

    return commandBuffer;}

//MKS Servo42C Parser
std::vector<uint8_t> CommandLink::parseMKShex(int8_t responseSize, uint8_t alignByte) {
    std::vector<uint8_t> commandBuffer;
    char commandByte;

    while (commandBuffer.empty() || commandBuffer.size() != responseSize) {
        uint8_t rCHK{0};
        serialDevice.ReadByte(commandByte);
        commandBuffer.push_back(static_cast<uint8_t>(commandByte));
        if (commandBuffer.front() == alignByte) {
            commandBuffer.clear();
            commandBuffer.push_back(alignByte);
            for (int k = 0; k < responseSize - 1; k++) {
                serialDevice.ReadByte(commandByte);
                commandBuffer.push_back(static_cast<uint8_t>(commandByte));}
            for (int j = 0; j < responseSize - 1; j++) {
                rCHK = rCHK + commandBuffer[j];}
            rCHK = (rCHK % 256) & 0xFF;

            if (commandBuffer[responseSize - 1] == rCHK) {
                std::cout << "Checksum success. Buffer verified. " << "\n";
                std::cout << " Angle buffer vector indices: " << std::hex 
                          << (int)commandBuffer[0] << " " 
                          << (int)commandBuffer[1] << " " 
                          << (int)commandBuffer[2] << " " 
                          << (int)commandBuffer[3] << " " 
                          << (int)commandBuffer[4] << " " 
                          << (int)commandBuffer[5] << "\n";
                break;} 
            else {
                std::cout << "Checksum failed. Trying again." << "\n";
                commandBuffer.clear();
            }} 
        else {
            commandBuffer.clear();}}
    return commandBuffer; 
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

