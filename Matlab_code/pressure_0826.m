clear all; clc; close all;

pressures = readmatrix("0826testpressures.csv");
Fs = 20; % sampling rate
ts = 1:length(pressures);
t = ts/Fs; % convert to seconds

% find start and end
ind_start = find(t==5138);
ind_end = find(t==49358);

pressures = pressures(ind_start:ind_end);
pressures_highpass = highpass(pressures,0.5,Fs);
pressures = pressures_highpass; % select filter
t = t(ind_start:ind_end);
pressures_first = pressures(1:454040);
pressures_second = pressures(493500:end);

% fft of first section
yf = fft(pressures_first);
mf = abs(yf);
yf(mf < 1e-6) = 0;
ff = (0:length(yf)-1)*20/length(yf); 

% fft of second section
ys = fft(pressures_second);

%p_filtered = medfilt1(pressures,2);
%ipt = findchangepts(pressures);

figure(1);
hold on;
plot(t,pressures);
xlabel('time (s)');
ylabel('pressure (mmHg)');
legend('original');
hold off;

figure(2);
plot(pressures_first);

figure(3);
plot(pressures_second);

figure(4);
plot(ff,mf);