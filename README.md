

Linux Driver for a ESP32S3 TinyUSB based device.

This USB driver is for the TinyUSB based ESP32S3 device.
This device creates a character device which can receive
RGB values to  change the color of the RMT LED in ESP32S3.
A companion project provides the firmware for the ESP32 device. 


The file "user_space_read_file.c" provides an user space program 
to interact with the  driver.
