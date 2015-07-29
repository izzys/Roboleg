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


while strcmp(SERIAL.Status, 'open')

switch state
 
  case 1

   
         pause(5);

 
  case 3
      
   break
end 

end


