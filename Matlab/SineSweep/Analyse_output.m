function [] = Analyse_output()
global h out_index out Sample_frequency save_data  previous_data_string load_previous_data Emergency_stop_flag CPR
%global h out_index out  Sample_frequency Furier_PLOT Sine_PLOT  Amp Phase save_data Save_Data_checkbox

if save_data
    
Output_strc.out = out;
%Output_strc.Sample_frequency = Sample_frequency;
Output_strc.out_index = out_index;
Output_strc.Emergency_stop_flag = Emergency_stop_flag;

date_and_hour = datestr(now);
Hour = hour(date_and_hour);
Minute = minute(date_and_hour);
Seconds = second(date_and_hour);
save(['out__' datestr(now,'dd-mmm-yyyy') '_'  num2str(Hour) '_' num2str(Minute) '_' num2str(Seconds) ],'Output_strc')

end

if load_previous_data
load(previous_data_string)
out = Output_strc.out;
out_index = Output_strc.out_index;
%Sample_frequency = Output_strc.Sample_frequency;
Emergency_stop_flag = Output_strc.Emergency_stop_flag;
% out = Output_strc.out;
% out_index = 1559;
% Sample_frequency = 100;

end

if ~isempty(out)
    
start_intex = 2;
if Emergency_stop_flag
    end_index = out_index-1;
else
     end_index = out_index-1;
end

for j =start_intex:end_index
    
    output_line(j-start_intex+1,:) =str2num( out{j} );
    
end
disp 'output done'


%  figure(11)
%  hold on
%  plot(output_line(:,4),output_line(:,1),'g','LineWidth',2) %Enc1
% % stairs(output_line(:,4),output_line(:,3),'b','LineWidth',2) %Command
% grid on
% xlabel('Time [sec]')


tvec = output_line(:,4)*1e-6;
theta1 = output_line(:,1)*360/CPR; % TODO
theta2 = output_line(:,2)*360/CPR;
uvec = output_line(:,3); % PWM, Voltage, Command
Delta = theta1-theta2;

stairs(h.Sine_PLOT,tvec,theta1,'g','LineWidth',2) %Enc1
stairs(h.Sine_PLOT,tvec,theta2,'m','LineWidth',2) %Enc2
stairs(h.Sine_PLOT,tvec,uvec,'b','LineWidth',2) %Command
stairs(h.Sine_PLOT,tvec,Delta,'c','LineWidth',2) %Delta
legend(h.Sine_PLOT,'Enc 1','Enc 2','command','Delta')

% Furier Analysis:
sample_diff = diff(tvec);
Tmean = mean(sample_diff);
tvar = std(sample_diff);
Fs = 1/Tmean;
L = length(tvec);

NFFT = 2^nextpow2(L); % Next power of 2 from length of y
%NFFT = L;
% 
% enc1_out =output_line(:,1) ;
% enc2_out =output_line(:,2) ;%- mean(output_line(:,1));  % to chance to check around the proper frequency !!!!!!!
% comm_out =output_line(:,3) ;

Yenc1 = fft(theta1,NFFT)/L;
Yenc2 = fft(theta2,NFFT)/L;
Ycomm = fft(uvec,NFFT)/L;
Ydelta = fft(Delta,NFFT)/L;

bin = NFFT/2+1;
f = Fs/2*linspace(0,1,bin);

Y1 = 2*abs(Yenc1(1:bin));
Y2 = 2*abs(Yenc2(1:bin));
Yu = 2*abs(Ycomm(1:bin));
Yd = 2*abs(Ydelta(1:bin));

% Plot single-sided amplitude spectrum.
plot(h.Furier_PLOT,f,Y1,'g','LineWidth',2) ;
plot(h.Furier_PLOT,f, Y2,'m','LineWidth',2) ;
plot(h.Furier_PLOT,f,Yu ,'b','LineWidth',2) ;
plot(h.Furier_PLOT,f,Yd ,'c','LineWidth',2) ;

title(h.Furier_PLOT,'Single-Sided Amplitude Spectrum of Enc1,Enc2 and Command')
xlabel(h.Furier_PLOT,'Frequency (Hz)')
ylabel(h.Furier_PLOT,'|Y(f)|')
legend(h.Furier_PLOT,'Enc1','Enc2','Comm','Delta')


%Bode:
Bode_color = [rand(1) rand(1) rand(1)];


Gain = Yd./Yu;
Phase_chopped = angle(Ydelta(1:bin))-angle(Ycomm(1:bin));

Phase = wrapToPi(Phase_chopped);

[b,a] = butter(2,0.1,'low');
Gain = filtfilt(b,a,Gain);
Phase = filtfilt(b,a,Phase);

% filt = tf(b,a,Tmean);
% figure(2)
% bode(filt)
% 


% Gain = filter(b,a,Gain);
% Phase = filter(b,a,Phase);

bode_ind = find((0.1<f)&(f<10)); % Picking by range

figure(132)
subplot 211
title('Bode Plot of the Data')
semilogx(f(bode_ind),20*log10(Gain(bode_ind)),'Color',Bode_color,'LineWidth',2)
hold on
ylabel('Gain')
grid on


subplot 212
semilogx(f(bode_ind),180*Phase(bode_ind)/pi,'Color',Bode_color,'LineWidth',2)
hold on
xlabel('frequency [Hz]')
ylabel('Phase [deg]')
grid on


%% system identification function [ create iddada object ]
poles = 4;
zeros = 2;
dataStruct = iddata(Delta,uvec,Tmean);
timeRange = 0:Tmean:max(tvec);

TFest_sys = tfest(dataStruct,poles ,zeros) % H(s)
sysStimulate = lsim(TFest_sys,uvec,timeRange);

% draw
% Insight: If the plots match --> Transfer function is pretty accurate
figure(133)
plot(timeRange,sysStimulate, 'r');
hold on
plot(timeRange,Delta, '-b');
title('Transfer Function  vs. Actual Data')
legend('Transfer Function Output','Actual Output Data')


[Gain2,Phase2] = bode(TFest_sys,f(bode_ind)*2*pi);
Gain2 = squeeze(Gain2);
Phase2 = squeeze(Phase2);

figure(132)
subplot 211
semilogx(f(bode_ind),20*log10(Gain2),'Color',Bode_color,'LineWidth',3,'LineStyle','--')

subplot 212
semilogx(f(bode_ind),Phase2,'Color',Bode_color,'LineWidth',3,'LineStyle','--')


%% Save TFest to the right file
org_path = previous_data_string;
temp = strsplit(previous_data_string, '\');
dirname = temp{end-1};
path = strjoin(temp(1:end-1),'\');
filename = ['\Tfest_' dirname];
full_path = [path filename];

% save
disp(['Success! TFest Saved as ' filename]);
save(full_path,'TFest_sys'); %%  save

%%
% Super title (over subplots):
% ha = axes('Position',[0 0 1 1],'Xlim',[0 1],'Ylim',[0  1],'Box','off','Visible','off','Units','normalized', 'clipping' , 'off');
% text(0.55, 0.98,'Bode plot ','HorizontalAlignment','center','VerticalAlignment', 'top','FontSize',15)
save('ExperimentData\f','f')
save('ExperimentData\Y1','Y1')
save('ExperimentData\Y2','Y2')
save('ExperimentData\Yu','Yu')


% figure
% plot(sample_diff,'.')
% this_freq = output_line(5,5);
% start_ind = find(f>this_freq/2, 1, 'first');
% 
% 
% [G1 , I1]  = max(2*abs(Yenc1(start_ind:NFFT/2+1)));
% [G2 , I2] = max(2*abs(Ycomm(start_ind:NFFT/2+1)));
% Amp = G1/G2;
% 
% val1 = Yenc1(I1);
% val2 = Ycomm(I2);
% 
% phase1 = angle(val1);
% phase2 = angle(val2);
% Phase = (phase1-phase2)*180/pi;
% disp ' Amplitude for this freq:'
% disp(Amp)
% disp ' Phase for this freq:'
% disp(Phase)


else
    
    disp 'Output is empty'
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
save('ExperimentData\theta1','theta1')
save('ExperimentData\theta2','theta2')
save('ExperimentData\tvec','tvec')
save('ExperimentData\uvec','uvec')
save('ExperimentData\uvec','uvec')
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% tvec_a = tvec;
% theta1_a = theta1;
% theta2_a = theta2;
% uvec_a = uvec;
% 
% 
% save('theta1_a','theta1_a')
% save('theta2_a','theta2_a')
% save('tvec_a','tvec_a')
% save('uvec_a','uvec_a')
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% tvec_b = tvec;
% theta1_b = theta1;
% theta2_b = theta2;
% uvec_b = uvec;
% 
% 
% save('theta1_b','theta1_b')
% save('theta2_b','theta2_b')
% save('tvec_b','tvec_b')
% save('uvec_b','uvec_b')

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
