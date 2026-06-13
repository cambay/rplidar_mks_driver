#include <iostream>
#include <string>
#include <libudev.h>
#include <libserial/SerialPort.h>
#include "identifyDevices.h"


//Need to enumerate the dev on /dev/ttyUSB* with the correct serial number, once returned, we can compare it against known values.
//identify for both the MKS Servo42C & Slamtec RPLidar C1

USBdevice lidar; 
USBdevice motor; //create instances of lidar and motor with type USBdevice 

void identifyUSBDevices() {
    
    struct udev *udev = udev_new();
    struct udev_enumerate *enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, "tty");
    udev_enumerate_add_match_sysname(enumerate, "ttyUSB*");
    
    udev_enumerate_scan_devices(enumerate);

    //const char* udev_device_get_devtype(struct udev_device *udev_device);
    struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);
    for (struct udev_list_entry *dev_list_entry = devices; dev_list_entry != NULL; dev_list_entry = udev_list_entry_get_next(dev_list_entry)) {
        const char *path = udev_list_entry_get_name(dev_list_entry);
        struct udev_device *dev = udev_device_new_from_syspath(udev, path);

        
        const char* dev_Type = udev_device_get_devnode(dev); //get device
        const char* serial_num = udev_device_get_property_value(dev, "ID_SERIAL");
        std::string devType = dev_Type ? dev_Type : "N/A"; 
        std::string serialnum = serial_num ? serial_num : "N/A"; 

        if(serialnum == "Silicon_Labs_CP2102N_USB_to_UART_Bridge_Controller_405341f8295df011b26358401045c30f"){
            //here you would have identified which device ( /dev/ttyUSB0 or /dev/ttyUSB1 for example) is the lidar or motor, then you'd attach that to the respective baudrate and serial port 
            lidar.serial = serialnum; 
            lidar.devNode = devType;  //"/dev/ttyUSB*..."
            lidar.baud_rate = 460800; //not technically even needed 
            std::cout << " **Lidar recognized** " <<"\n" << " Serial number: " << lidar.serial << "\n" << " Serial port: " << lidar.devNode << "\n" << " **END LIDAR** " << "\n"; 
        }
        else if(serialnum == "1a86_USB_Serial"){
            motor.serial = serial_num; 
            motor.devNode = devType; 
            motor.baud_rate = 38400; 
            std::cout << " **Motor recognized** " <<"\n" << " Serial number: " << motor.serial << "\n" << " Serial port: " << motor.devNode << "\n" << " **END MOTOR** " << "\n"; 
        }
        else{
            std::cout << "Not a recognized device" << "\n"; //default behavior 
        }
        udev_device_unref(dev);
    }
    udev_enumerate_unref(enumerate);
    udev_unref(udev);
}

