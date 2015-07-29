function Start_serial()
global SERIAL out out_index COM_num serial_open STATUS_OK

%if (SERIAL.available())

% CHECK Serial Availablity
STATUS_OK = false;
s_info  = instrhwinfo('serial');
s_list = s_info.AvailableSerialPorts;
if(find(strcmp(s_list, COM_num)))
    STATUS_OK = true;
end; 

% to find ports use: instrfind
if(~STATUS_OK)
    disp('Serial unavailable');
else
    SERIAL = serial(COM_num, 'BaudRate', 115200);

%Continuously reads from the serial port
SERIAL.ReadAsyncMode = 'continuous';
set(SERIAL, 'BytesAvailableFcn', {'WriteNewData'});
SERIAL.InputBufferSize = 100000;

%Open the serial connection
fopen(SERIAL);

disp(['Opened serial connection at ' COM_num  '. Check  START MOTORS botton on blue box!!!' ]);
disp ' '
disp 'waiting 3 seconds to initialize.....'
pause(3);
disp 'Serial Connected.'
disp ' '

out_index = 0;
out = {};
end;
%else
  %  disp 'Serial Busy'
%end
%Enter the state machine
%serial_open = true;
