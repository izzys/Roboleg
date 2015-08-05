duration = [2e3,2e3,5e3,2e3,9e3,2e3];
value = [100, -90, 100, -90,-500,0];
types = [0,0,1,0,0];
sampleRate = 0.1; % tick time

t = [sampleRate:sampleRate:sum(duration.*sampleRate)];
y = [];
pos = 1;
count_inside = 1;
samplesCount=1;  

while(samplesCount <= length(t))
    if( count_inside <= duration(pos))
        y(samplesCount) = value(pos);
        samplesCount = samplesCount+1;
        count_inside = count_inside+1;
    else
        pos = pos+1;
        count_inside = 1;
    end
end

figure
plot (t,y)
size(t);
size(y);