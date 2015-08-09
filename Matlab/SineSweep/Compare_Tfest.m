%% TODO HERE!
%% ReadMe

% Load files:
% * Tfest_xxx.mat
% * tvec.mat
% * uvec.mat
% * f.mat
%TFest_sys = 13*H;
% IMPORTANT ! Remember the order of loading, no legeneds described

%files = uigetfile('*.mat',  'Tfest, tvec, uvec (*.mat)','MultiSelect','on');

%% defines
Tf_Color = [rand(1) rand(1) rand(1)];
Bode_color = [rand(1) rand(1) rand(1)];

sample_diff = diff(tvec);
Tmean = mean(sample_diff);

%% Comapring Tfest 
timeRange = 0:Tmean:max(tvec);
sysSimulation = lsim(TFest_sys,uvec,timeRange);



%% Compare bode:

% Bode things
% bode_ind = find((0.1<f)&(f<10)); % Picking by range
% [Gain2,Phase2] = bode(TFest_sys,f(bode_ind)*2*pi);
% Gain2 = squeeze(Gain2);
% Phase2 = squeeze(Phase2);
% 
% 
% figure(333)
% hold on
% subplot 211
% hold on
% title('BODE PLOT: Comapring Different Tfest ');
% semilogx(f(bode_ind),20*log10(Gain2),'Color',Bode_color,'LineWidth',1,'LineStyle','-')
% hold on
% ylabel('Gain')
% grid on
% 
% subplot 212
% hold on
% semilogx(f(bode_ind),Phase2,'Color',Bode_color,'LineWidth',1,'LineStyle','-')
% xlabel('frequency [Hz]')
% ylabel('Phase [deg]')


%% Compare output:

figure(334)
hold on
subplot 211
hold on
plot(timeRange,uvec, 'Color',Tf_Color);
plot(tvec,uvec, 'Color',Tf_Color,'LineStyle','--');
ylabel 'u'

subplot 212
hold on
plot(timeRange,sysSimulation, 'Color',Tf_Color);
plot(timeRange,theta1-theta2, 'Color',Tf_Color,'LineStyle','--');
title('Output: Comapring Different Tfest files');
legend('Simulation','Experiment')
xlabel 'Time [sec]'
ylabel '\Delta \theta'