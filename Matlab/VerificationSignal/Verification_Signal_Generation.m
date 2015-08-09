

clc;clear all;close all

sampleRate =  0.005 *1e6; % (seconds) * 1e6
duration = [1,1,1,1,1,1,0002,.5,.5,.5,.5,.5,.5,0002,.25,.25,.25,.25,.25,.25,0002,3,.5,3,5,10,6,30]*1e6/sampleRate;
value = [50, -50,75, -75, 100,-75,0,50, -50,75, -75, 100,-100,0,75, -75,75, -75, 75,-75,000,.015,100,-.025,-40,.055,-.02,000];
type = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,000,1,0,1,0,1,1,0];


t = [0:sampleRate:sum(duration)*sampleRate];
y = [];
pos = 1;
count_inside = 0;
totalCount=1;  

while(pos <= length(value))
    
    if(count_inside < duration(pos))
        
        if(type(pos) == 0) % block
            y(totalCount) = value(pos);
        else
            y(totalCount) = value(pos)*count_inside + last_value;
        end

        count_inside = count_inside+1;
        totalCount = totalCount+1;
        
    else
        last_value = y(totalCount-1);
        pos = pos+1;
        count_inside = 0;
    end
    
    
end

figure
y=[ y  y(end)  ];
stairs(t,y)
size(t);
size(y);
sum(duration);