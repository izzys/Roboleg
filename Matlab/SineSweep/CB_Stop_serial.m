function [] = CB_Stop_serial()
global  SERIAL out_index COM_num
%checks what ports are availible and only if the port is not availible
%(serial conected it disconects
STATUS_OK = false;
s_info  = instrhwinfo('serial');
s_list = s_info.AvailableSerialPorts;
if(find(strcmp(s_list, COM_num)))
    STATUS_OK = true;
end; 
if(~STATUS_OK)
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
else
    disp 'Serial was already closed'
end