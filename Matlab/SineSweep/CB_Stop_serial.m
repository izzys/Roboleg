function [] = CB_Stop_serial()
global  SERIAL

fprintf(SERIAL, '0'); 
%disp 'Serial closed'
pause(0.1)

fclose(SERIAL);

pause(0.1)

if strcmp(SERIAL.Status,'closed')
disp 'Serial Closed'
else
    
    disp 'Error: Serial not closed properly'
end