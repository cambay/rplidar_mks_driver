#pragma once 
#include "commandLink.h"
#include "identifyDevices.h"

class Lidar: public deviceFunctions{

    private: 
    uint8_t get_health[2] = {0xA5,0x52}; 
    uint8_t start_scan[2] = {0xA5,0x20}; 
    uint8_t reset_scan[2] = {0xA5,0x40}; 
    uint8_t stop_scan[2]  = {0xA5,0x25}; 
    uint8_t express_scan[9] = {0xA5, 0x82, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22};

    public:
    Lidar(CommandLink& l); 
    
    void start();
    void express();
    void getHealth();
    void stop();
    void parseHex(char x, char y, uint timer);

};


