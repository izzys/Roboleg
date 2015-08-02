clc;%close all

load Tests_SineSweep/log_8min_50amp_01_10freq_200hz/uvec
load Tests_SineSweep/log_8min_50amp_01_10freq_200hz/tvec
load Tests_SineSweep/log_8min_50amp_01_10freq_200hz/theta1
load Tests_SineSweep/log_8min_50amp_01_10freq_200hz/theta2

% end_ind = 50083;
% uvec = uvec(1:end_ind);
% tvec = tvec(1:end_ind);
% theta1 = theta1(1:end_ind);
% theta2 = theta2(1:end_ind);

sample_diff = diff(tvec);
Ts = mean(sample_diff);

Delta = theta1-theta2;

% freq. domain:
Bode_color = [rand(1) rand(1) rand(1)];
Fs = 1/Ts;
L = length(tvec);
NFFT = 2^nextpow2(L);

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

Gain=Yd./Yu;

Phase_chopped = angle(Ydelta(1:bin))-angle(Ycomm(1:bin));
Phase = wrapToPi(Phase_chopped);

%filter data:
[b,a] = butter(2,0.1,'low');
filt = tf(b,a,Ts);
Gain = filtfilt(b,a,Gain);
Phase = filtfilt(b,a,Phase);

bode_ind = find((0.01<f)&(f<10));

% plot exp. bode: 
figure(132)
subplot 311
semilogx(f(bode_ind),20*log10(Gain(bode_ind)),'Color',Bode_color,'LineWidth',2)
hold on
ylabel('Gain')
grid on

subplot 312
semilogx(f(bode_ind),180*Phase(bode_ind)/pi,'Color',Bode_color,'LineWidth',2)
hold on
xlabel('frequency [Hz]')
ylabel('Phase [deg]')
grid on


% create iddada object:
data = iddata(Delta,uvec,Ts);

% Identify the transfer function:




TFEST_sys = tfest(data,4 ,2) 

[mag,phase] = bode(TFEST_sys,f(bode_ind)*2*pi);

figure(132)
subplot 311
semilogx(f(bode_ind),20*log10(squeeze(mag)),'Color',Bode_color,'LineWidth',3,'LineStyle','--')

subplot 312
semilogx(f(bode_ind),squeeze(phase),'Color',Bode_color,'LineWidth',3,'LineStyle','--')
tvec1 = linspace(0,tvec(end),length(tvec));
[y,t] = lsim(TFEST_sys,uvec,tvec1);

% [R,lags]  = xcorr(Delta,y,'coeff');
subplot 313
title correlation
% semilogx(tvec1,R)

optCVA = n4sidOptions('Focus','simulation','Display','on','N4weight','auto');
% optSSARX = n4sidOptions('Focus','prediction','Display','on','N4weight','SSARX');
% optMOESP = n4sidOptions('Focus','simulation','Display','on','N4weight','MOESP');
sysCVA = n4sid(data,4,'Ts',0,optCVA)
% sysSSARX = n4sid(data,4,'Ts',0,optSSARX)
% sysMOESP = n4sid(data,4,'Ts',0,optMOESP)
compare(data, sysCVA, TFEST_sys);
%ident

step(sysCVA,TFEST_sys)
