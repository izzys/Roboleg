function [] = CB_Start_motors()
%Starts the motors at frequency freq.
global SERIAL state startFreq stopFreq FreqInterval numTests maxNumTests ...
       freq lastIndex firstIndex out_index FreqDataIndex Sample_frequency ...
       freqDataArray out Emergency_stop_flag

   
disp 'Starting motor ...'
%Send the command to start
fprintf(SERIAL, '1');
Emergency_stop_flag =0;
FreqDataIndex = 1;
stopFreq = 1;

while strcmp(SERIAL.Status, 'open')

switch state
 
  case 1
    if(freq<=stopFreq)
        
        
         pause(5);
%         Last_Data = out{out_index};
%         Last_Data = str2num(Last_Data);
%         freq = 2*Last_Data(5);
%         disp(['Frequency: ' num2str(freq) ' [Hz]...'])

    else

        %if all the tests are completed, exit
        state = 2;
    end

  case 2
      
   %Turn off motors
   CB_Stop_motors();
   CB_Stop_serial();
   break
   
  case 3
      
   break
end 

end


