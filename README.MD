
---

## rplidar_mks_driver

### A custom driver built for the Slamtec RPLidar C1 and MKS Servo42C devices

The aim of this project is to allow control of both devices to enable synchronous in-phase movement, with the long-term goal of building a simple, replicable 3D LiDAR system from a 2D LiDAR.

---

## Hardware installation & motor testing

*(RPLidar integration not yet implemented)*

### Required parts/materials

1. Slamtec RPLidar C1
2. MKS Servo42C/D
3. 12V external power supply
4. CH340 USB-to-TTL adapter
5. JST/JST-XH connectors & crimping kit (if needed)

---

### Assembly

#### 1. CH340 wiring

It is strongly recommended that you follow these guides:

* [https://github.com/makerbase-mks/MKS-SERVO42C/wiki/Serial-communication-wiring](https://github.com/makerbase-mks/MKS-SERVO42C/wiki/Serial-communication-wiring)
* [https://www.youtube.com/watch?v=BZu_7QPjx4A](https://www.youtube.com/watch?v=BZu_7QPjx4A)

---

#### 2. Power and connection

* Connect the MKS Servo42C to an external 12V power supply (minimum 12V required)
* Attach the Slamtec Rplidar C1 via the physical USB 2.0 port. 

---

#### 3. Identify serial device (For reference)

In your terminal:

```bash
ls /dev/ttyUSB*
```

This will show which USB device both the lidar and motor is attached to.

---

#### 4. Build and run

Compile:

```bash
g++ main.cpp identifyDevices.cpp commandLink.cpp mks.cpp rplidar.cpp tools.cpp -o main -ludev -lserial


```

Run:

```bash
./main
```

This will currently compile the main file with all driver files, prompt you to enter a duration to turn the lidar on for, begin spinning the lidar and outputting real-time angle and distance data from the C1 to the terminal.

You can modify the commands, add inputs via `std::cin`, or expand control logic as needed.

Future work includes integration with the RPLidar C1 and synchronization between rotation and scanning via the MKS Servo42C.

---

## References

### MKS SERVO42C commands

[https://github.com/makerbase-mks/MKS-SERVO42C/wiki/Serial-communication-description](https://github.com/makerbase-mks/MKS-SERVO42C/wiki/Serial-communication-description)

### SLAMTEC RPLIDAR C1 protocol

[http://bucket.download.slamtec.com/b42b54878a603e13c76a0a0500b53595846614c6/LR001_SLAMTEC_rplidar_protocol_v1.1_en.pdf](http://bucket.download.slamtec.com/b42b54878a603e13c76a0a0500b53595846614c6/LR001_SLAMTEC_rplidar_protocol_v1.1_en.pdf)

---

