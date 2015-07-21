function Start_serial()
global SERIAL out out_index COM_num serial_open

% to find ports use: instrfind
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
%Enter the state machine
%serial_open = true;
