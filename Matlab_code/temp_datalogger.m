clear all; clc; close all;

baudrate = double(9600);
arduino=serialport("/dev/cu.usbmodem11401",baudrate);
x=linspace(1,100);

data = readline(arduino);