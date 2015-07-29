%This program runs the main test for the Roboleg project. It will change
%the frequency of the leg and sample data on how the system reacts to 
%each frequency.
clc; clear all; %close all;
global SERIAL freq COM_num out_index out serial_open zeroed state ...
    startFreq stopFreq FreqInterval numTests maxNumTests Sample_frequency ...
    firstIndex lastIndex  FreqDataIndex FreqDataArray CPR  STATUS_OK

COM_num =  'COM6';  
% startFreq = 0.1;
% stopFreq = 10;
% FreqInterval = 0.05;
Sample_frequency = 200;
%serial_open = false;
FreqDataArray ={};
CPR = 14400;

% State 1 = start and sample
% State 2 = exit
state = 1;

numTests = 0;
maxNumTests = stopFreq/FreqInterval;

%Open the serial port
Start_serial()
Set_GUI

freq = 0.1; 