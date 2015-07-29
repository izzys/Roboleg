function [] = CB_Stop_motors()
%Sends a command to turn off the motors.

global SERIAL state

%Send the command to stop
fprintf(SERIAL, '0');
state = 3;
pause(0.01)

disp 'motors stopped'

