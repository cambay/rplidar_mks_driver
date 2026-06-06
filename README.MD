
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

Connect the MKS Servo42C to an external 12V power supply (minimum 12V required).

---

#### 3. Identify serial device

In your terminal:

```bash
ls /dev/ttyUSB*
```

This will show which USB device the motor is attached to.

---

#### 4. (Optional but recommended) fix serial mode

You can set the port to raw mode to ensure consistent communication:

```bash
stty -F /dev/ttyUSB0 38400 raw -echo
```

---

#### 5. Build and run

Compile:

```bash
g++ main.cpp identifyDevices.cpp -o main -ludev -lserial
```

Run:

```bash
./main
```

This will execute a simple calibration, start, and stop sequence.

You can modify the commands, add inputs via `std::cin`, or expand control logic as needed.

Future work includes integration with the RPLidar C1 and synchronization between rotation and scanning.

---

## References

### MKS SERVO42C commands

[https://github.com/makerbase-mks/MKS-SERVO42C/wiki/Serial-communication-description](https://github.com/makerbase-mks/MKS-SERVO42C/wiki/Serial-communication-description)

### SLAMTEC RPLIDAR C1 protocol

[http://bucket.download.slamtec.com/b42b54878a603e13c76a0a0500b53595846614c6/LR001_SLAMTEC_rplidar_protocol_v1.1_en.pdf](http://bucket.download.slamtec.com/b42b54878a603e13c76a0a0500b53595846614c6/LR001_SLAMTEC_rplidar_protocol_v1.1_en.pdf)

---

