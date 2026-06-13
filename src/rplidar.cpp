#include "rplidar.h"
#include <cmath>
#include <iomanip> 
#include <chrono>


Lidar::Lidar(CommandLink& l) : deviceFunctions(l){
    std::cout << "Lidar created on port:" << link.getPort() << "\n"; 
    }
void Lidar::start(){
    std::cout << "Starting lidar" << "\n"; 
    link.sendCommand(start_scan, sizeof(start_scan)); 
    //code that spits out the angle readings in real time, like the SDK it came with 
    }
void Lidar::express(){
    std::cout << "Starting Express scan" << "\n"; 
    link.sendCommand(express_scan, sizeof(express_scan)); 
    }
void Lidar::getHealth(){
    std::cout << "Getting health: " << "\n" ;
    link.sendCommand(get_health,sizeof(get_health)); 
    }
void Lidar::stop(){
//std::cout << "Stopping lidar" << "\n"; 
    link.sendCommand(stop_scan, sizeof(stop_scan)); 
    }


void Lidar::parseHex(char x, char y, uint timer) {
    char b0, b1;
    lidar_Port.ReadByte(b0);
    while (true) {
        lidar_Port.ReadByte(b1);
        if ((uint8_t)b0 == (uint8_t)x && (uint8_t)b1 == (uint8_t)y) {
            char discard;
            for (int i = 0; i < 5; i++) {
                lidar_Port.ReadByte(discard);}
            break;}
        b0 = b1;
    }
    auto start = std::chrono::steady_clock::now();
    while (true) {
        auto end = std::chrono::steady_clock::now();
        auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        uint8_t data_pkt[5];
        for (int j = 0; j < 5; j++) {
            char b;
            lidar_Port.ReadByte(b);
            data_pkt[j] = (uint8_t)b;}
        uint8_t quality    = (data_pkt[0] >> 2) & 0x3F;
        uint8_t start_flag = data_pkt[0] & 0x03;
        bool new_scan      = (start_flag == 0x01);
        uint16_t angle_raw = ((uint16_t)data_pkt[2] << 8) | data_pkt[1];
        float angle        = std::fmod((angle_raw >> 1) / 64.0f, 360.0f);
        float distance_mm  = ((data_pkt[4] << 8) | data_pkt[3]) / 4.0f;

        if (quality > 0 && distance_mm > 0) {
            std::cout << (new_scan ? "S " : "  ")
                      << "Theta: " << std::fixed << std::setprecision(2) << angle
                      << "  Dist: " << distance_mm << " mm"
                      << "  Q: " << (int)quality << "\n";}
        else if (time_elapsed.count() >= timer)
        {
            std::cout << "Timer up. " << timer << "ms has elapsed." << "\n"; 
            break;
        }
        
    }
}
