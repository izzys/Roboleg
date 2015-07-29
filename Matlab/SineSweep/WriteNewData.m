function WriteNewData(SERIAL, event)
global out_index out zeroed Emergency_stop_flag

out_index=out_index+1;
%my_dispcallback(SERIAL, event)
out{out_index} = fscanf(SERIAL);
if strcmp(out{out_index},'emergency stop')
    disp 'Emergency stop!'
    Emergency_stop_flag = 1;
end
end
        
