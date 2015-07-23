classdef mySerial
    %UNTITLED3 Summary of this class goes here
    %   Detailed explanation goes here
    
    properties
        con
    end
    
    methods
        function [SELF] = mySerial(PORT, BoudRate)
        %UNTITLED2 Summary of this function goes here
        %   Detailed explanation goes here
            SELF.con = serial(PORT,'BaudRate',BoudRate); % 'DataBits',7
        end
        
        function SELF = Open(SELF)
            fopen(SELF.con);
        end
        
        function SELF = Write(SELF, command)
            fprintf(SELF.con, command);
        end
        
        function SELF = Close(SELF)
            fclose(SELF.con);
        end
        
        function SELF = Get(SELF)
            SELF = fscanf(s);
        end

    end
    
end

