%% TODO HERE!
%% ReadMe

% Load files:
% * Tfest_xxx.mat
% * tvec.mat
% * uvec.mat
% * f.mat

% IMPORTANT ! Remember the order of loading, no legeneds described

uigetfile('*.mat',  'Tfest, tvec, uvec (*.mat)','MultiSelect','on');


%% defines
Tf_Color = [rand(1) rand(1) rand(1)];
Bode_color = [rand(1) rand(1) rand(1)];

sample_diff = diff(tvec);
Tmean = mean(sample_diff);

%% Comapring Tfest 
timeRange = 0:Tmean:max(tvec);
sysStimulate = lsim(TFest_sys,uvec,timeRange);

% Bode things
bode_ind = find((0.1<f)&(f<10)); % Picking by range
[Gain2,Phase2] = bode(TFest_sys,f(bode_ind)*2*pi);
Gain2 = squeeze(Gain2);
Phase2 = squeeze(Phase2);

%% TODO::: Fix graph, it erases when loading another test
figure(333)
subplot 311
title('BODE PLOT: Comapring Different Tfest ');
semilogx(f(bode_ind),20*log10(Gain2),'Color',Bode_color,'LineWidth',1,'LineStyle','-')

subplot 312
semilogx(f(bode_ind),Phase2,'Color',Bode_color,'LineWidth',1,'LineStyle','-')

subplot 313
plot(timeRange,sysStimulate, 'Color',Tf_Color);
title('Output: Comapring Different Tfest files');

hold on
