classdef Led
    %UNTITLED5 Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        status
        serial
    end
    
    methods
        function y = Led(PORT, BoudRate)
            Led.status = false;
            Led.serial = mySerial(PORT,BoudRate);
            Led.serial.Open();
        end
        % 1 -> on; 0 -> off
        function y = On(y)
            Led.status = true;
            Led.serial.Write('1');
        end
        
        function y = Off(y)
            Led.status = false;
            Led.serial.Write('0');
        end
    end
    
end

